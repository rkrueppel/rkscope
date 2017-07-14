#pragma once

#include "DisplayController.h"
#include "BaseController_p.h"
#include "helpers/SyncQueues.h"
#include "parameters/Scope.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "ScopeDatatypes.h"
#include "gui/ChannelFrame.h"
#include "gui/HistogramFrame.h"
#include "ScopeLogger.h"

namespace scope {

/** The implementation class of the DisplayController. There is only one Run thread (not one for every area since the heavy work is done inside CChannelFrame and CHistogramFrame)
* This passes images arriving from the PipelineController to attached CChannelFrame and CHistogramFrame where the actual calculations
* (overlay, histogram) and renderings take place. */
class DisplayController::Impl
	: public BaseController<1>::Impl {

protected:
	/** Input queue with multi images from the PipelineController */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const input_queue;
	
	/** a ScopeLogger kept handy here */
	ScopeLogger scope_logger;

	/** Vector of CChannelFrame observers */
	std::array<std::vector<gui::CChannelFrame* const>, SCOPE_NAREAS> channelframes;

	/** Mutex to protect access to that vector */
	mutable std::array<std::mutex, SCOPE_NAREAS> channelframes_mutexe;

	/** Vector of CHistogramFrame observers */
	std::array<std::vector<gui::CHistogramFrame* const>, SCOPE_NAREAS> histogramframes;

	/** Mutex to protect access to that vector */
	mutable std::array<std::mutex, SCOPE_NAREAS> histogramframes_mutexe;

protected:
	/** disable copy */
	Impl(const Impl& i);

	/** disable assignment */
	Impl operator=(const Impl& i);

	/** Main function for managing display. It is executed asynchronously.
	* The actual overlay creations, histogram calculations, and rendering are done in the Active's threads inside
	* CChannelFrame/View and CHistogramFrame/View.
	* Only one Run thread, so _area parameter here is always 0. 
	* We dequeue images from the input queue and distribute to the areas CChannelFrame and CHistogramFrame these belong to. */
	ControllerReturnStatus Run(StopCondition* const sc, const uint32_t& _area) override {
		DBOUT(L"DisplayController::Impl::Run beginning\n");
		uint32_t area = 0;
		SCOPE_MULTIIMAGEPTR_T current_frame;
		size_t num_planes = 1;
		ScopeController scope_controller;
		ControllerReturnStatus returnstatus(ControllerReturnStatus::none);
		std::array<uint32_t, SCOPE_NAREAS> framecounts;
		std::fill(std::begin(framecounts), std::end(framecounts), 0);
		// avoid the mutexed access to parameters later on
		DaqMode requested_mode = parameters.requested_mode();
		std::array<uint32_t, SCOPE_NAREAS> requested_frames;

		// Initialize (deferred) locks
		std::array<std::unique_lock<std::mutex>, SCOPE_NAREAS> channelframes_locks;
		std::array<std::unique_lock<std::mutex>, SCOPE_NAREAS> histogramframes_locks;
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			requested_frames[a] =  parameters.areas[a]->daq.requested_frames();
			channelframes_locks[a] = std::unique_lock<std::mutex>(channelframes_mutexe[a],std::defer_lock);
			histogramframes_locks[a] = std::unique_lock<std::mutex>(histogramframes_mutexe[a],std::defer_lock);
		}

		// Send current run state to channel frames
		UpdateStatusInFrames(parameters.run_state());

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
				DBOUT(L"DisplayController::Impl::Run - all requested frames from all areas processed\n");
			}
		}

		if ( sc->IsSet() )
			returnstatus = ControllerReturnStatus(returnstatus || ControllerReturnStatus::stopped);

		UpdateStatusInFrames(RunStateHelper::Stopped);
		DBOUT(L"DisplayController::Impl::Run ended\n");
		return returnstatus;
	}

	/** Calls CChannelFrame::UpdateStatus and CHistogramFrame::UpdateStatus in all attached frames*/
	void UpdateStatusInFrames(const RunState& _rs) {
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			std::lock_guard<std::mutex> lockc(channelframes_mutexe[a]);
			for( auto cframe : channelframes[a] )
				cframe->UpdateStatus(_rs);
			std::lock_guard<std::mutex> lockh(histogramframes_mutexe[a]);
			for( auto hframe : histogramframes[a] )
				hframe->UpdateStatus(_rs);
		}
	}

public:
	/** Connects queue and gets parameters*/
	explicit Impl(SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue, const parameters::Scope _parameters)
		: BaseController::Impl(_parameters)
		, input_queue(_iqueue) {
		DBOUT(L"DisplayController::Impl::Impl");
	}

	/** Stops and interrupts thread if necessary*/
	virtual ~Impl() {
		DBOUT(L"DisplayController::Impl::~Impl");
		StopAll();
		WaitForAll(-1);
	}

	/** We need to override here. The dequeue in Run could block/wait since nothing is in the queue.
	* To stop we put a message with abort tag in, it gets dequeued and we break from the while loop. (In addition we call BaseController::Impl::StopOne which sets the
	* StopCondition to true. */
	void StopOne(const uint32_t& _a) override {
		BaseController::Impl::StopOne(_a);
		ScopeMessage<SCOPE_MULTIIMAGEPTR_T> stopmsg(ScopeMessageTag::abort, nullptr);
		input_queue->Enqueue(stopmsg);
	}

	/** Go through all CChannelFrames and request resize to cope with new image size
	* @param[in] _ap contains the new x and y resolution of images*/
	void ResolutionChange(const parameters::Area& _ap) {
		*parameters.areas[_ap.area()] = _ap;			// Update so that we have the current resolution in here
		// Clear the queue, so we don't have frames with the (now) wrong resolution
		input_queue->Clear();
		DBOUT(L"DisplayController::Impl::ResolutionChange to " << parameters.areas[_ap.area()]->Currentframe().xres() << L" " << parameters.areas[_ap.area()]->Currentframe().yres());
	}

	/** Go through all CChannelFrames of that area and set the histogram limits there
	* @param[in] _area,_channel area and channel to set limits
	* @param[in] _lower,_upper new lower and upper histogram limits*/
	void SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper) {
		std::lock_guard<std::mutex> lock(channelframes_mutexe[_area]);
		for ( auto cframe : channelframes[_area] )
			cframe->SetHistogramLimits(_channel, _lower, _upper);
	}

	/** Attaches a CChannelFrame as observer to the DisplayController
	* @param[in] _cframe pointer to the new CChannelFrame observer*/
	void AttachFrame(gui::CChannelFrame* const _cframe) {
		uint32_t area = _cframe->Area();
		std::lock_guard<std::mutex> lock(channelframes_mutexe[area]);
		channelframes[area].push_back(_cframe);
		_cframe->UpdateStatus(parameters.run_state());
	}

	/** Detaches a CChannelFrame as observer from the DisplayController
	* @param[in] _cframe pointer to the CChannelFrame to detach
	* @throws std::exception if _cframe is not found */
	void DetachFrame(gui::CChannelFrame* const _cframe) {
		uint32_t area = _cframe->Area();
		std::lock_guard<std::mutex> lock(channelframes_mutexe[area]);
		std::vector<gui::CChannelFrame* const>::iterator found
			= std::find_if(std::begin(channelframes[area]), std::end(channelframes[area]), [&](const gui::CChannelFrame* const f)
				{ return f->m_hWnd == _cframe->m_hWnd; } );
		if ( found == std::end(channelframes[area]) )
			throw std::exception("DisplayController::Impl::DetachChannelFrame no match");
		else
			channelframes[area].erase(found);
	}

	/** Attaches a CHistogramFrame as observer to the DisplayController
	* @param[in] _hframe pointer to the new CHistogramFrame observer */
	void AttachFrame(gui::CHistogramFrame* const _hframe) {
		uint32_t area = _hframe->Area();
		std::lock_guard<std::mutex> lock(histogramframes_mutexe[area]);
		histogramframes[area].push_back(_hframe);
		_hframe->UpdateStatus(parameters.run_state());
	}

	/** @return true if alreay at least one histogram for that area attached, used to avoid several histograms for the same
	* area. */
	bool HistogramAlreadyAttached(const uint32_t& _area) const {
		std::lock_guard<std::mutex> lock(histogramframes_mutexe[_area]);
		return (histogramframes[_area].size() > 0);
	}

	/** Detaches a CHistogramFrame as observer from the DisplayController
	* @param[in] _hframe pointer to the CHistogramFrame to detach
	* @throws std::exception if _hframe is not found */
	void DetachFrame(gui::CHistogramFrame* const  _hframe) {
		uint32_t area = _hframe->Area();
		std::lock_guard<std::mutex> lock(histogramframes_mutexe[area]);
		std::vector<gui::CHistogramFrame* const>::iterator found 
			= std::find_if(std::begin(histogramframes[area]), std::end(histogramframes[area]), [&](const gui::CHistogramFrame* const f)
				{ return f->m_hWnd == _hframe->m_hWnd; } );
		if ( found == std::end(histogramframes[area]) )
			throw std::exception("DisplayController::Impl::DetachHistogramFrame no match");
		else
			histogramframes[area].erase(found);
	}

	/** Adds frames to a WindowCollection */
	parameters::WindowCollection GetWindowCollection() const {
		parameters::WindowCollection wndcoll;

		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
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
};

}