#include "StdAfx.h"
#include "DisplayController.h"
#include "gui\HistogramFrame.h"

namespace scope {

	DisplayController::DisplayController(const uint32_t& _nactives, const parameters::Scope& _parameters, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue)
		: BaseController(_nactives)
		, input_queue(_iqueue)
		, channelframes(_nactives)
		, channelframes_mutexe(_nactives)
		, histogramframes(_nactives)
		, histogramframes_mutexe(_nactives)
		, ctrlparams(_parameters) {
	}

	/** Stops and interrupts thread if necessary*/
	DisplayController::~DisplayController() {
		DBOUT(L"DisplayController::~DisplayController");
		StopAll();
		WaitForAll(-1);
	}

	/** Main function for managing display. It is executed asynchronously.
	* The actual overlay creations, histogram calculations, and rendering are done in the Active's threads inside
	* CChannelFrame/View and CHistogramFrame/View.
	* Only one Run thread, so _area parameter here is always 0. 
	* We dequeue images from the input queue and distribute to the areas CChannelFrame and CHistogramFrame these belong to. */
	ControllerReturnStatus DisplayController::Run(StopCondition* const sc, const uint32_t& _area) {
		DBOUT(L"DisplayController::Run beginning\n");
		uint32_t area = 0;
		SCOPE_MULTIIMAGEPTR_T current_frame;
		size_t num_planes = 1;
		
		ControllerReturnStatus returnstatus(ControllerReturnStatus::none);
		std::vector<uint32_t> framecounts(nactives);
		std::fill(std::begin(framecounts), std::end(framecounts), 0);
		// avoid the mutexed access to parameters later on
		DaqMode requested_mode = ctrlparams.requested_mode();
		std::vector<uint32_t> requested_frames(nactives);

		// Initialize (deferred) locks
		std::vector<std::unique_lock<std::mutex>> channelframes_locks(nactives);
		std::vector<std::unique_lock<std::mutex>> histogramframes_locks(nactives);
		for ( uint32_t a = 0 ; a < nactives ; a++ ) {
			requested_frames[a] = ctrlparams.areas[a].daq.requested_frames();
			channelframes_locks[a] = std::unique_lock<std::mutex>(channelframes_mutexe[a],std::defer_lock);
			histogramframes_locks[a] = std::unique_lock<std::mutex>(histogramframes_mutexe[a],std::defer_lock);
		}

		// Send current run state to channel frames
		UpdateStatusInFrames(ctrlparams.run_state());

		// Dequeue and distribute loop
		while ( !sc->IsSet() ) {
			// Dequeue
			ScopeMessage<SCOPE_MULTIIMAGEPTR_T> msg(input_queue->Dequeue());

			// If message has abort tag, break from while loop
			if ( msg.tag == ScopeMessageTag::abort ) {
				returnstatus = ControllerReturnStatus::stopped;
				break;
			}

			current_frame = msg.cargo;
			area = current_frame->Area();
			//if ( SCOPE_USE_RESONANCESCANNER )
			//	num_planes = scope_controller.GuiParameters.areas[area]->FrameResonance().planes.size()?scope_controller.GuiParameters.areas[area]->FrameResonance().planes.size():1;

			// To resize the Displays according to number of planes defined in the GUI
			for ( auto cframe : channelframes[area] )
				cframe->OnMultSize(1, num_planes);	

			// Increase framecount if a complete frame was received
			if ( current_frame->IsCompleteFrame() && current_frame->IsCompleteAvg() ) {
				framecounts[area]++;
				DBOUT(L"DisplayController framecount area " << area << L": " << framecounts[area]);
			}

			// Distribute current_frame to attached CChannelFrames and render the stuff in CChannelFrame's Active's thread
			channelframes_locks[area].lock();
			for ( auto cframe : channelframes[area] )
				//cframe->OnMultSize(1, num_planes);
				cframe->LayOverAndRender(current_frame);					
			channelframes_locks[area].unlock();

			// Distribute current_frame to attached CHistogramFrame and calculate and render stuff in CHistogramFrame's Active's thread
			// (only for complete frames so save time)
			if ( current_frame->IsCompleteFrame() ) {
				histogramframes_locks[area].lock();
				for ( auto hframe : histogramframes[area] )
					hframe->HistoGramAndRender(current_frame);
				histogramframes_locks[area].unlock();
			}

			// Check if in nframes mode if we have already distributed all requested frames for all areas to display
			if ( (requested_mode == DaqModeHelper::nframes)
				&& (std::equal(std::begin(requested_frames), std::end(requested_frames), std::begin(framecounts))) ) {	
				// if yes we want to stop
				sc->Set(true);
				returnstatus = ControllerReturnStatus::finished;
				DBOUT(L"DisplayController::Run - all requested frames from all areas processed\n");
			}
		}

		if ( sc->IsSet() )
			returnstatus = ControllerReturnStatus(returnstatus || ControllerReturnStatus::stopped);

		UpdateStatusInFrames(RunStateHelper::Stopped);
		DBOUT(L"DisplayController::Run ended\n");
		return returnstatus;
	}

	void DisplayController::UpdateStatusInFrames(const RunState& _rs) {
		for ( uint32_t a = 0 ; a < nactives ; a++ ) {
			std::lock_guard<std::mutex> lockc(channelframes_mutexe[a]);
			for( auto cframe : channelframes[a] )
				cframe->UpdateStatus(_rs);
			std::lock_guard<std::mutex> lockh(histogramframes_mutexe[a]);
			for( auto hframe : histogramframes[a] )
				hframe->UpdateStatus(_rs);
		}
	}

	void DisplayController::StopOne(const uint32_t& _a) {
		BaseController::StopOne(_a);
		ScopeMessage<SCOPE_MULTIIMAGEPTR_T> stopmsg(ScopeMessageTag::abort, nullptr);
		input_queue->Enqueue(stopmsg);
	}


	void DisplayController::ResolutionChange(const parameters::Area& _ap) {
		ctrlparams.areas[_ap.area()] = _ap;			// Update so that we have the current resolution in here
		// Clear the queue, so we don't have frames with the (now) wrong resolution
		input_queue->Clear();
		DBOUT(L"DisplayController::ResolutionChange to " << ctrlparams.areas[_ap.area()].Currentframe().xres() << L" " << ctrlparams.areas[_ap.area()].Currentframe().yres());
	}

	void DisplayController::SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper) {
		std::lock_guard<std::mutex> lock(channelframes_mutexe[_area]);
		for ( auto cframe : channelframes[_area] )
			cframe->SetHistogramLimits(_channel, _lower, _upper);
	}

	void DisplayController::AttachFrame(gui::CChannelFrame* const _cframe) {
		uint32_t area = _cframe->Area();
		std::lock_guard<std::mutex> lock(channelframes_mutexe[area]);
		channelframes[area].push_back(_cframe);
		_cframe->UpdateStatus(ctrlparams.run_state());
	}

	void DisplayController::DetachFrame(gui::CChannelFrame* const _cframe) {
		uint32_t area = _cframe->Area();
		std::lock_guard<std::mutex> lock(channelframes_mutexe[area]);
		std::vector<gui::CChannelFrame*>::iterator found
			= std::find_if(std::begin(channelframes[area]), std::end(channelframes[area]), [&](gui::CChannelFrame* const f)
				{ return f->m_hWnd == _cframe->m_hWnd; } );
		if ( found == std::end(channelframes[area]) )
			throw std::exception("DisplayController::DetachChannelFrame no match");
		else
			channelframes[area].erase(found);
	}

	void DisplayController::AttachFrame(gui::CHistogramFrame* const _hframe) {
		uint32_t area = _hframe->Area();
		std::lock_guard<std::mutex> lock(histogramframes_mutexe[area]);
		histogramframes[area].push_back(_hframe);
		_hframe->UpdateStatus(ctrlparams.run_state());
	}

	bool DisplayController::HistogramAlreadyAttached(const uint32_t& _area) const {
		std::lock_guard<std::mutex> lock(histogramframes_mutexe[_area]);
		return (histogramframes[_area].size() > 0);
	}

	void DisplayController::DetachFrame(gui::CHistogramFrame* const  _hframe) {
		uint32_t area = _hframe->Area();
		std::lock_guard<std::mutex> lock(histogramframes_mutexe[area]);
		std::vector<gui::CHistogramFrame*>::iterator found 
			= std::find_if(std::begin(histogramframes[area]), std::end(histogramframes[area]), [&](gui::CHistogramFrame* const f)
				{ return f->m_hWnd == _hframe->m_hWnd; } );
		if ( found == std::end(histogramframes[area]) )
			throw std::exception("DisplayController::DetachHistogramFrame no match");
		else
			histogramframes[area].erase(found);
	}

	parameters::WindowCollection DisplayController::GetWindowCollection() const {
		parameters::WindowCollection wndcoll;

		for ( uint32_t a = 0 ; a < nactives ; a++ ) {
			{
				std::lock_guard<std::mutex> lock(histogramframes_mutexe[a]);
				for ( const auto& h : histogramframes[a] ) {
					wndcoll.AddWindow(L"CHistogramFrame", a, h->m_hWnd);
				}
			}

			{
				std::lock_guard<std::mutex> lock(channelframes_mutexe[a]);
				for ( const auto& c : channelframes[a] ) {
					wndcoll.AddWindow(L"CChannelFrame", a, c->m_hWnd);
				}
			}
		}
		return wndcoll;
	}


}