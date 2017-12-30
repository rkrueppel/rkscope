#include "stdafx.h"
#include "PipelineController.h"

namespace scope {

	PipelineController::PipelineController(
		const uint32_t& _nactives
		, parameters::Scope& _guiparameters
		, ScopeCounters<config::nareas>& _counters
		, std::array<SynchronizedQueue<ScopeMessage<config::DaqMultiChunkPtrType>>, config::threads_daq>* const _iqueues
		, SynchronizedQueue<ScopeMessage<config::MultiImagePtrType>>* const _squeue
		, SynchronizedQueue<ScopeMessage<config::MultiImagePtrType>>* const _dqueue
	)
		: BaseController(_nactives)
		, guiparameters(_guiparameters)
		, counters(_counters)
		, input_queues(_iqueues)
		, storage_queue(_squeue)
		, display_queue(_dqueue)
		, scannervecs(_nactives)
		, online_update_mutexe(_nactives)
		, online_updates(_nactives)
	{
		DBOUT(L"PipelineController::PipelineController");
	}

	PipelineController::~PipelineController() {
		StopAll();
		WaitForAll(-1);
	}

	ControllerReturnStatus PipelineController::Run(StopCondition* const sc, const uint32_t& _masterarea) {
		ATLTRACE(L"PipelineController::Run beginning\n");
		uint32_t framecount = 0;
		uint32_t avgcount = 0;

		std::unique_lock<std::mutex> online_update_lock(online_update_mutexe[_masterarea], std::defer_lock);
		online_updates[_masterarea] = false;
		ControllerReturnStatus returnstatus(ControllerReturnStatus::none);
		PixelmapperResult pixelmapper_result(Error);

		const uint32_t downsampling = (guiparameters.areas[_masterarea].daq.inputs->oversampling())?round2ui32(guiparameters.areas[_masterarea].daq.pixeltime() / guiparameters.areas[_masterarea].daq.inputs->MinimumPixeltime()):1;
		// Get some values as locals, avoid the mutexed access to parameters later on (really necessary??)
		const DaqMode requested_mode = guiparameters.requested_mode();
		const uint32_t requested_frames = guiparameters.areas[_masterarea].daq.requested_frames();
		const uint32_t requested_averages = guiparameters.areas[config::MyMaster(_masterarea)].daq.averages();
		const double totalframepixels = guiparameters.areas[_masterarea].Currentframe().XTotalPixels() * guiparameters.areas[_masterarea].Currentframe().YTotalLines();

		//size_t num_planes = 1;
		//if ( SCOPE_USE_RESONANCESCANNER )
		//	num_planes = guiparameters.areas[_area].frameresonance.planes.size()?guiparameters.areas[_area].frameresonance.planes.size():1;

		std::vector<config::MultiImagePtrType> current_frames(0);
		current_frames.reserve(config::areaspermaster);
		for (uint32_t a = 0; a < config::areaspermaster; a++) {
			current_frames.push_back(std::make_shared<config::MultiImageType>(_masterarea+a
				, guiparameters.areas[_masterarea + a].daq.inputs->channels()
				, guiparameters.areas[_masterarea + a].Currentframe().yres()	// * num_planes
				, guiparameters.areas[_masterarea + a].Currentframe().xres()));
		}

		std::vector<config::MultiImagePtrType> next_frames(current_frames);

		for ( auto& cf : current_frames )
			cf->SetAvgMax(requested_averages);

		//current_frame->InitializeCurrentLineData(5*guiparameters.areas[_area]->currentframe->XTotalPixels());

		std::unique_ptr<PixelmapperBasic<>> pixel_mapper(PixelmapperBasic<config::areaspermaster>::Factory(config::scannerselect, guiparameters.areas[_masterarea].scanmode()));
		pixel_mapper->SetLookupVector(scannervecs[_masterarea]->GetLookupVector());
		pixel_mapper->SetParameters(scannervecs[_masterarea]->GetSVParameters());
		pixel_mapper->SetCurrentFrames(current_frames);

		/*ScopeMultiImagePtr current_averaged_frame;
		ScopeMultiImagePtr next_averaged_frame;
		if ( SCOPE_USE_RESONANCESCANNER ) {
			current_averaged_frame = std::make_shared<ScopeMultiImage>(_area
				, guiparameters.areas[_area].daq.inputs->channels()
				, guiparameters.areas[_area].currentframe->yres()*num_planes
				, guiparameters.areas[_area].currentframe->xres());
			next_averaged_frame = current_averaged_frame;
			current_averaged_frame->SetAvgMax(requested_averages);
			dynamic_cast<PixelmapperFrameResonance*>(pixel_mapper.get())->SetCurrentAveragedFrame(current_averaged_frame);
		}*/

		counters.framecounter[_masterarea].SetWithLimits(0, 0, requested_frames);
		counters.singleframeprogress[_masterarea].SetWithLimits(0, 0, 100);

		// Dequeue and pixelmap loop
		while ( !sc->IsSet() ) {
			// Dequeue
			ScopeMessage<config::DaqMultiChunkPtrType> msg(input_queues->at(_masterarea).Dequeue());

			// If message has abort tag, break from while loop
			if ( msg.tag == ScopeMessageTag::abort ) {
				returnstatus = stopped;
				break;
			}
			auto chunk = msg.cargo;

			// If we oversampled during acquisition, now downsample to pixeltime
			chunk->Downsample(downsampling);

			// Map until the whole data chunk is mapped (could be overlapping the end of a frame)
			do {
				// Map the chunk into the current_frame
				// Attention: this blocks ScopeOverlay::Create in CChannelFrame since we need write access to the image here!
				pixelmapper_result = pixel_mapper->LookupChunk(chunk, (uint16_t)avgcount);

				// Set progress and frame properties
				counters.singleframeprogress[_masterarea] += 100.0 * chunk->PerChannel() / totalframepixels;
				for (auto& cf : current_frames) {
					cf->SetPercentComplete(counters.singleframeprogress[_masterarea].Value());
					cf->SetAvgCount(avgcount + 1);
					cf->SetImageNumber(framecount + 1);
				}

				// Put current_frames in outgoing messages
				std::array<ScopeMessage<config::MultiImagePtrType>, config::areaspermaster> outmsgs;
				for ( uint32_t a = 0; a < config::areaspermaster ; a++)
					outmsgs[a].cargo = current_frames[a];

				// If one frame is mapped completely...
				if ( (pixelmapper_result & FrameComplete) != 0 ) {
					for (auto& cf : current_frames)
						cf->SetCompleteFrame(true);
					counters.singleframeprogress[_masterarea] = 0.0;
					
					// If all the averages for one frame have been done...
					if ( ++avgcount == requested_averages ) {							
						avgcount = 0;
						for (auto& cf : current_frames)
							cf->SetCompleteAvg(true);
						
						// the next frame is a copy of the old (allows for continuous updating effect, no black pixels in new frame)
						for (uint32_t a = 0; a < config::areaspermaster; a++)
							next_frames[a] = std::make_shared<config::MultiImageType>(*current_frames[a]);

						// Enqueue frame for storage
						for ( auto& o : outmsgs)
							storage_queue->Enqueue(o);

						// Increase frame counters
						framecount++;
						counters.framecounter[_masterarea] += 1;

						// Set frame properties
						for (auto& nf : next_frames) {
							nf->SetCompleteAvg(false);
							nf->SetAvgMax(requested_averages);
							nf->SetCompleteFrame(false);
							nf->SetPercentComplete(0.0);
						}

						// Give pixelmapper the new frame
						pixel_mapper->SetCurrentFrames(next_frames);
					}
				}

				// enqueue always for display, for running update
				for (auto& o : outmsgs)
					display_queue->Enqueue(o);

				// Let current_frame pointer point to next one (the actual image persists since the shared_ptr in msg is still pointing to it)
				current_frames = next_frames;

			// while loop ends with end of  chunk of if stop condition is set (this allows abort during mapping of one chunk)
			} while ( ((pixelmapper_result & EndOfChunk) != EndOfChunk) && !sc->IsSet() );

			// Check if an online parameter update was requested and we have to update the LookupVector etc
			// Only really needed for scannerdelay change (since zoom etc. leave the lookup vector untouched)
			online_update_lock.lock();
			if ( online_updates[_masterarea] && (requested_mode == DaqModeHelper::continuous) ) {
				DBOUT(L"PipelineController::Run online update\n");
				// Give pixelmapper the current lookup vector
				pixel_mapper->SetLookupVector(scannervecs[_masterarea]->GetLookupVector());
				pixel_mapper->SetParameters(scannervecs[_masterarea]->GetSVParameters());
				online_updates[_masterarea] = false;
			}
			online_update_lock.unlock();

			// Check if we read enough samples (if not live scanning)
			if ( (requested_mode == DaqModeHelper::nframes) && (requested_frames == framecount) ) {	// are we done?
				returnstatus = finished;
				sc->Set(true);
				DBOUT(L"PipelineController all requested frames from masterarea " << _masterarea << L" processed\n");
			}
		}

		if ( sc->IsSet() )
			returnstatus = ControllerReturnStatus(returnstatus || ControllerReturnStatus::stopped);

		DBOUT(L"PipelineController::Run end\n");
		return returnstatus;
	}
		
	void PipelineController::StopOne(const uint32_t& _a) {
		// This sets stop condition
		BaseController::StopOne(_a);
		// This enqueues abort message (just to be sure)
		ScopeMessage<config::DaqMultiChunkPtrType> stopmsg(ScopeMessageTag::abort, nullptr);
		input_queues->at(_a).Enqueue(stopmsg);
	}

	void PipelineController::OnlineParameterUpdate(const parameters::Area& _areaparameters) {
		const uint32_t area = _areaparameters.area();
		std::lock_guard<std::mutex> lock(online_update_mutexe[area]);		// lock, thus worker thread waits and we can safely update parameters
		// update parameters not needed since we have a reference to TheScope's guiparameters
		//*parameters.areas[area] = _areaparameters;
		online_updates[area] = true;	
	}

	void PipelineController::SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv) {
		scannervecs[_area] = _sv;
	}
	
}