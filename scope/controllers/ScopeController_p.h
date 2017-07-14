#pragma once

#include "ScopeController.h"
#include "ScopeControllerModes.h"
#include "ScopeDefines.h"
#include "version.h"
#include "scanmodes/ScannerVectorFrameBasic.h"
#include "scanmodes/ScannerVectorFrameSaw.h"
#include "scanmodes/ScannerVectorFrameBiDi.h"
#include "scanmodes/ScannerVectorFramePlaneHopper.h"
#include "scanmodes/ScannerVectorFrameResonanceBiDi.h"
#include "scanmodes/ScannerVectorFrameResonanceHopper.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "DaqController.h"
#include "PipelineController.h"
#include "DisplayController.h"
#include "StorageController.h"
#include "helpers/ScopeException.h"
#include "FPUController.h"
#include "ScopeLogger.h"
#include "gui/ChannelFrame.h"
#include "gui/HistogramFrame.h"
#include "devices/xyz/XYZControl.h"
#include "devices/xyz/XYZControlGalil.h"
#include "devices/xyz/XYZControlSutter.h"
#include "devices/GaterDAQmx.h"

namespace scope {

/** The implementation class of the ScopeController. */
class ScopeController::Impl
	: public BaseController<1, true>::Impl {

protected:
	/** queues from the daqs to the pipelines */
	std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS> daq_to_pipeline;
	
	/** queue from the pipelines to the storage */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>> pipeline_to_storage;

	/** queue from the pipelines to the display */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>> pipeline_to_display;

	/** Scanner vectors
	* @{ */
	/** The scanner vector for frame scanning */
	std::array<ScannerVectorFrameBasicPtr, SCOPE_NAREAS> framescannervecs;
	/** @} */

	/** @name Dataflow controllers
	* @{ */
	DaqController theDaq;
	PipelineController thePipeline;
	StorageController theStorage;
	DisplayController theDisplay;
	/** @} */

	/** thread-safe bool to signal a requested abort of a stack scan */
	StopCondition repeat_abort;

	/** for abortable waiting between timeseries repeats */
	std::condition_variable waitbetweenrepeats_cond;

	/** for abortable waiting between timeseries repeats */
	std::mutex waitbetweenrepeats_mutex;

	/** future for the online update thread */
	std::future<void> onlineupdate_future;

	/** set if an online update is currently running, to prevent to simultaneous online update threads */
	std::atomic<bool> onlineupdate_running;
	
	/** stores the time to run a scan */
	DWORD time;

	/** true after parameter set on program start is loaded */
	ScopeNumber<bool> initialparametersloaded;

	/** currently loaded config */
	std::wstring currentconfigfile;

	/** list of callbacks for scan mode switching */
	std::vector<std::function<void(const uint32_t&, const ScannerVectorType&)>> scanmodecallbacks;

	/** a logger kept handy */
	ScopeLogger scope_logger;

protected:
	/** disable copy */
	Impl(const Impl& i);

	/** disable assignment */
	Impl operator=(const Impl& i);

	/** Logs the current scan (not live scans) */
	void LogRun() {
		if ( parameters.storage.autosave() ) {
			std::wstring msg(L"");
			switch ( parameters.run_state().t ) {
			case RunStateHelper::RunningContinuous:
				if ( parameters.storage.savelive() )
					msg += L"Live scan (saving)";
				break;
			case RunStateHelper::RunningSingle:
				msg += L"Single scan";
				break;
			case RunStateHelper::RunningStack:
				msg += L"Stack scan";
				break;
			case RunStateHelper::RunningTimeseries:
				msg = L"Timeseries scan";
				break;
			case RunStateHelper::RunningBehavior:
				msg = L"Behavior scan";
				break;
			}
			scope_logger.Log(msg, log_info);
		}
	}

	/** Starts all controllers with the current parameters */
	void StartAllControllers() {
		time = ::timeGetTime();
		theDisplay.Start(parameters);
		theStorage.Start(parameters);
		thePipeline.Start(parameters);
		theDaq.Start(parameters);
	}

	/** Stops all controllers in the correct order and waits for them to finish. */
	void StopAllControllers() {
		theDisplay.StopAll();
		theStorage.StopAll();
		thePipeline.StopAll();
		theDaq.StopAll();
	
		try {
			ControllerReturnStatus sdisp = theDisplay.WaitForAll(500);
		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		try {
			ControllerReturnStatus sstor = theStorage.WaitForAll(500);
		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		try {
			ControllerReturnStatus spipe = thePipeline.WaitForAll(-1); // 5000
		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		try {
			ControllerReturnStatus sdaq = theDaq.WaitForAll(2000);
		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		time = ::timeGetTime() - time;
	}

	/** Waits for all controllers to intrinsically finish. Only good for nframes runmode */
	void WaitForAllControllers() {
		theDisplay.WaitForAll();
		theStorage.WaitForAll();
		thePipeline.WaitForAll();
		theDaq.WaitForAll();
	}

	/** Clear all queues between controllers */
	void ClearAllQueues() {
		for ( auto& d : daq_to_pipeline )
			d.Clear();
		pipeline_to_display.Clear();
		pipeline_to_storage.Clear();		
	}

	/** Set parameters for scanner vectors, since these are shared_ptr in DaqControllerDAQmx and PipelineController they get updated there too */
	void SetScannerVectorParameters() {
		DBOUT(L"ScopeControllerImpl::SetScannerVectorParameters");
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
			framescannervecs[a]->SetParameters(&parameters.areas[a]->daq, &parameters.areas[a]->Currentframe(), &parameters.areas[a]->fpuzstage);
	}

	/** Worker function to control live scanning (basically only starting everything up) */
	ControllerReturnStatus RunLive(StopCondition* const sc) {
		ClearAllQueues();
		SetScannerVectorParameters();
		DBOUT(L"ScopeController::RunLive()");
		StartAllControllers();
		return ControllerReturnStatus::finished;
	}

	/** Worker function to control a single scan */
	ControllerReturnStatus RunSingle(StopCondition* const sc) {
		ClearAllQueues();
		LogRun();
		SetScannerVectorParameters();
		StartAllControllers();
		WaitForAllControllers();
		ClearAfterStop();
		return ControllerReturnStatus::finished;
	}

	/** Worker function to control a stack scan */
	ControllerReturnStatus RunStack(StopCondition* const sc) {
		ClearAllQueues();
		LogRun();
		SetScannerVectorParameters();

		// We do the same number of planes in each area (if fast z and not that many planes in range in one area, Pockels was set to zero in parameters::stack::UpdatePlanes)
		for ( auto& a : parameters.areas )
			a->daq.requested_frames = parameters.stack.planes___.size();

		// StorageController and DisplayController saves/displays the number of slices in each area
		theStorage.Start(parameters);
		theDisplay.Start(parameters);

		// At every slice Daq and PipelineController expect 1 frame (they calculate averages themselves)
		//  do not manipulate GuiParameters here, otherwise we have a problem on restarting the stack
		for ( auto& ap : parameters.areas )
			ap->daq.requested_frames = 1;
		
		// Set scale for the progress indicator
		PlaneCounter.SetWithLimits(0, 0, parameters.stack.planes___.size());

		// Go through all the precalculated planes
		for ( uint32_t p = 0 ; p < parameters.stack.planes___.size() ; p++ ) {

			// Move to precalculated position (stage position is the same for all areas, no of slices thus also the same)
			if ( parameters.stack.zdevicetype().t == ZDeviceHelper::ZStage ) {
				theStage.MoveAbsolute(theStage.CurrentXPosition(), theStage.CurrentYPosition(), parameters.stack.planes___[p][0].position());
				// Wait one second for movement to complete
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
			else if ( parameters.stack.zdevicetype().t == ZDeviceHelper::FastZ ) {
				for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
					parameters.areas[a]->Currentframe().fastz = parameters.stack.planes___[p][a].position();
					// Also change GuiParameters so the users sees what is happening during stacking
					GuiParameters.areas[a]->Currentframe().fastz = parameters.stack.planes___[p][a].position();
				}
			}

			// Set pockels to precalculated value
			for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
				parameters.areas[a]->Currentframe().pockels = parameters.stack.planes___[p][a].pockels();
				// Also change GuiParameters so the users sees what is happening during stacking
				GuiParameters.areas[a]->Currentframe().pockels = parameters.stack.planes___[p][a].pockels();
			}

			// Calculate scanner vectors
			SetScannerVectorParameters();

			// Start
			theDaq.Start(parameters);
			thePipeline.Start(parameters);

			// and wait for them to end
			theDaq.WaitForAll();
			thePipeline.WaitForAll();
			
			// Stop the stack if abort requested
			if ( repeat_abort.IsSet() )
				break;

			// Advance progress indicator
			PlaneCounter = p+1;
		}

		theDisplay.WaitForAll();
		theStorage.WaitForAll();
		ClearAfterStop();

		PlaneCounter = 0;

		return ControllerReturnStatus::finished;
	}

	/** Worker function to control a timeseries scan */
	ControllerReturnStatus RunTimeseries(StopCondition* const sc) {
		LogRun();
		ClearAllQueues();
		SetScannerVectorParameters();

		// Set repeat counter limits (for progress bar display)
		RepeatCounter.SetWithLimits(1, 1, parameters.timeseries.repeats());

		// Go through all repeats
		for ( uint32_t t = 0 ; t < parameters.timeseries.repeats() ; t++ ) {
			RepeatCounter = t+1;
		
			ClearAllQueues();

			// Start a timeseries
			StartAllControllers();

			// Start waiting, wait from beginning of timeseries until beginning of next one
			std::unique_lock<std::mutex> lock(waitbetweenrepeats_mutex);
			std::chrono::milliseconds waittime(static_cast<long>(1000*parameters.timeseries.betweenrepeats()));
			// Waits or is notified before by a call to Stop
			waitbetweenrepeats_cond.wait_for(lock, waittime);
			
			// This should return immediately, because waittime is always longer than duration of one timeseries
			// or because StopAllControllers was called from Stop
			WaitForAllControllers();

			// Alternate planes if planes defined
			if ( parameters.timeseries.planes.size() != 0 ) {
				for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
					parameters.areas[a]->Currentframe().fastz = parameters.timeseries.planes[t % parameters.timeseries.planes.size()][a].position();
				SetScannerVectorParameters();
			}

			// Break if abort condition set (from Stop)
			if ( repeat_abort.IsSet() ) {
				RepeatCounter = 1;
				break;
			}
		}

		ClearAfterStop();
		return ControllerReturnStatus::finished;
	}

	/** Worker function to control a behavior triggered scan */
	ControllerReturnStatus RunBehavior(StopCondition* const sc) {
		LogRun();
		ClearAllQueues();
		SetScannerVectorParameters();
		uint32_t trials = 0;
		DWORD starttime = ::timeGetTime();
		GaterDAQmx gater(parameters.behavior.gateline(), sc);

		// Set trial counter limits
		TrialCounter.SetWithLimits(0.0, 0.0, parameters.behavior.unlimited_repeats()?10000.0:parameters.behavior.repeats());

		// Record trials until StopCondition set or requested number of trials is reached
		while ( !sc->IsSet() || parameters.behavior.unlimited_repeats() || trials < parameters.behavior.repeats() ) {
			// Wait for high signal then start acquisition, on StopCondition break from loop
			if ( !gater.WaitFor(true) )
				break;

			//Band-Aid Jerry - Forced Behavior Acquisition to run in Live Scan Mode with Live saving enabled
			parameters.time.Set(GetCurrentTimeString());
			parameters.run_state = RunStateHelper::RunningContinuous;
			parameters.storage.savelive = true;

			StartAllControllers();
			// Update time
			TotalTime = static_cast<double>(::timeGetTime() - starttime) / 1000;
	
			// Wait for low signal and then stop acquisition, on StopCondition break from loop
			if ( !gater.WaitFor(false) )
				break;
			StopAllControllers();
			ClearAllQueues();
			// Update counters and time
			trials++;
			TrialCounter = trials;
			TotalTime = static_cast<double>(::timeGetTime() - starttime) / 1000;
		}

		ClearAfterStop();
		return ControllerReturnStatus::finished;
	}

	/** Clears queues, resets run state, and reenables controls */
	void ClearAfterStop() {
		DBOUT(L"ScopeControllerImpl::CleanAfterStop");
		ClearAllQueues();
		repeat_abort.Set(false);
		parameters.run_state = RunStateHelper::Stopped;
		GuiParameters.run_state = RunStateHelper::Stopped;
		SetGuiCtrlState();
	}

public:
	/** @name Hardware controllers
	* @{ */
	FPUController theFPUs;
	SCOPE_XYZCONTROL_T theStage;
	/** @} */

public:
	/** Initializes and connects stuff */
	explicit Impl()
		: BaseController::Impl(parameters::Scope())
		, daq_to_pipeline()
		, pipeline_to_display()
		, pipeline_to_storage()
		, theDaq(&daq_to_pipeline, parameters)
		, thePipeline(&daq_to_pipeline, &pipeline_to_storage, &pipeline_to_display, parameters)
		, theStorage(&pipeline_to_storage, parameters)
		, theDisplay(&pipeline_to_display, parameters)
		, repeat_abort(false)
		, onlineupdate_running(false)
		, time(0)
		, initialparametersloaded(false, false, true)
		, currentconfigfile(L"NONE")
		, scope_logger() {
		// Connect static buttons in ScopeController to ScopeController::Impl functions
		// QuitButton is connected in CMainDlgFrame
		StartSingleButton.Connect(std::bind(&Impl::StartSingle, this));
		StartLiveButton.Connect(std::bind(&Impl::StartLive, this));
		StartStackButton.Connect(std::bind(&Impl::StartStack, this));
		StartTimeseriesButton.Connect(std::bind(&Impl::StartTimeseries, this));
		StartBehaviorButton.Connect(std::bind(&Impl::StartBehavior, this));
		StopButton.Connect(std::bind(&Impl::Stop, this));
		
		// We cannot connect directly to XYZStage::SetZero because std::bind needs the class to be copyable (which it may  not)
		StageZeroButton.Connect(std::bind(&Impl::SetStageZero, this));

		StackStartHereButton.Connect(std::bind(&Impl::StackStartHere, this));
		StackStopHereButton.Connect(std::bind(&Impl::StackStopHere, this));

		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			// Initially choose the first supported scannervector in the list
			SetScanMode(a, *ScannerSupportedVectors::List().begin());

			// Set counters to zero
			FrameCounter[a] = 0;
			SingleFrameProgress[a] = 0;

			// Connect the buttons for scan mode switching (if a master area)
			if ( !ThisIsSlaveArea(a) ) {
				for ( auto& b : ScanMode[a].map )
					b.second.Connect(std::bind(&Impl::SetScanMode, this, a, b.first));
			}

			// Connect all imaging parameters
			for ( auto& sv : GuiParameters.areas[a]->scannervectorframesmap ) {
				sv.second->ConnectOnlineUpdate(std::bind(&Impl::UpdateAreaParametersFromGui, this, a));
				sv.second->ConnectResolutionChange(std::bind(&Impl::ResolutionChange, this, a));
			}
			
			GuiParameters.areas[a]->daq.pixeltime.ConnectOther(std::bind(&Impl::UpdateAreaParametersFromGui, this, a));
			GuiParameters.areas[a]->daq.scannerdelay.ConnectOther(std::bind(&Impl::UpdateAreaParametersFromGui, this, a));
			GuiParameters.areas[a]->histrange.ConnectOther(std::bind(&Impl::UpdateAreaParametersFromGui, this, a));
			//GuiParameters.areas[a]->frameresonance.yres.ConnectOther(std::bind(&ScopeControllerImpl::ResolutionChange, this, a));

			// Connect FPU XY movements inside the FPUController!!
		}

		SetGuiCtrlState();
	}

	/** Stop whatever is running */
	~Impl() {
		Stop();
	}

	/** Called via CMainDialogFrame::QuitApplication.
	* Stops running threads etc, avoids destruction of these when the static pimpl gets destructed (probably very late or undefined). */
	void PrepareQuit() {
		SetFPUButtonsState(false);
		StopButton.Enable(false);
		theStage.StopPolling();
		for ( const auto& s : theFPUs.theXYStages )
			s->StopPolling();
		Stop();
		SetGuiCtrlState();
	}

	/** Prints out the last Git commit date of the currently running version (see description at ScopeController::Version) */
	void Version() const {
		// Extract last commit date
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Last Git commit " + revstr + L")";
		DBOUT(revstr.c_str());
	}

	/** @return the currently loaded config file */
	std::wstring CurrentConfigFile() const {
		return currentconfigfile;
	}

	/** (Re-)Initializes ScopeControllers own hardware components (XYControl/Stage, ZControl, and FPU stuff) */
	void InitializeHardware() {
		// Give GuiParameters by reference, so hardware has parameters and can connect to ScopeNumbers
		theStage.Initialize(GuiParameters.stage);
		theFPUs.Initialize(GuiParameters);
	}

	/** Starts live scanning by running RunLive asynchronously */
	void StartLive() {
		if ( parameters.run_state() == RunStateHelper::Mode::Stopped ) {
			GuiParameters.requested_mode.Set(DaqModeHelper::Mode::continuous);
			GuiParameters.run_state.Set(RunStateHelper::Mode::RunningContinuous);
			GuiParameters.date.Set(GetCurrentDateString());
			GuiParameters.time.Set(GetCurrentTimeString());
			parameters = GuiParameters;
			SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&Impl::RunLive, this, &stops[0]));
			futures[0].wait();				// Wait here, because RunLive should quickly return
		}
	}

	/** Start a stack scan by running RunStack asynchronously */
	void StartStack() {
		if ( parameters.run_state() == RunStateHelper::Mode::Stopped ) {
			GuiParameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
			GuiParameters.date.Set(GetCurrentDateString());
			GuiParameters.time.Set(GetCurrentTimeString());
			GuiParameters.run_state.Set(RunStateHelper::Mode::RunningStack);
			parameters = GuiParameters;
			SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&Impl::RunStack, this, &stops[0]));
		}
	}
	
	/** Starts a single frame scan by running RunSingle asynchronously */
	void StartSingle() {
		if ( parameters.run_state() == RunStateHelper::Mode::Stopped ) {
				GuiParameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
			GuiParameters.run_state.Set(RunStateHelper::Mode::RunningSingle);
			GuiParameters.date.Set(GetCurrentDateString());
			GuiParameters.time.Set(GetCurrentTimeString());
			parameters = GuiParameters;
			SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&Impl::RunSingle, this, &stops[0]));
		}
	}

	/** Starts a timeseries by running RunTimeseries asynchronously */
	void StartTimeseries() {
		if ( parameters.run_state() == RunStateHelper::Mode::Stopped ) {
			// Same number of frames for all areas. However, this is non-ideal...
			for ( uint32_t a = 1 ; a < SCOPE_NAREAS ; ++a )
				GuiParameters.timeseries.frames[a] = GuiParameters.timeseries.frames[0].Value();
			GuiParameters.run_state.Set(RunStateHelper::Mode::RunningTimeseries);
			GuiParameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
			GuiParameters.date.Set(GetCurrentDateString());
			GuiParameters.time.Set(GetCurrentTimeString());
			for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
				GuiParameters.areas[a]->daq.requested_frames = GuiParameters.timeseries.frames[a]();
			parameters = GuiParameters;
			SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&Impl::RunTimeseries, this, &stops[0]));
		}
	}

	/** Starts a behavior triggered acquisition by running RunBehavior asynchronously */
	void StartBehavior() {
		if ( parameters.run_state() == RunStateHelper::Mode::Stopped ) {
			GuiParameters.run_state.Set(RunStateHelper::Mode::RunningBehavior);
			GuiParameters.requested_mode.Set(DaqModeHelper::Mode::continuous);
			GuiParameters.date.Set(GetCurrentDateString());
			GuiParameters.time.Set(GetCurrentTimeString());
			parameters = GuiParameters;
			SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&Impl::RunBehavior, this, &stops[0]));
		}
	}

	/** Stops whatever scanning is going on and clears the queues */
	void Stop() {
		DBOUT(L"ScopeController::stop()\n");
		stops[0].Set();

		if ( (parameters.run_state() == RunStateHelper::RunningStack) || (parameters.run_state() == RunStateHelper::Mode::RunningTimeseries) )
			repeat_abort.Set(true);
		
		// Notify the condition variable waiting betweenrepeats in RunTimeseries
		if ( parameters.run_state() == RunStateHelper::Mode::RunningTimeseries )
			waitbetweenrepeats_cond.notify_one();

		if ( parameters.run_state() != RunStateHelper::Mode::Stopped ) {
			// If we currently running an online update, wait until it is ready before stopping controllers!
			if ( onlineupdate_future.valid() )
				onlineupdate_future.get();

			StopAllControllers();
			// If we ran live we have to call the ClearAfterStop here (since RunLive does not wait for scanning to finish. How could it...)
			if ( static_cast<RunState>(parameters.run_state()) == RunStateHelper::Mode::RunningContinuous ) {
				ClearAfterStop();
			}
			//futures[0].wait();
		}
		DBOUT(L"ScopeController::stop end()\n");
	}

	/** Updates area parameters and scanner vectors in the controllers from the GuiParameters set without stopping scanning */
	void UpdateAreaParametersFromGui(const uint32_t& _area) {
		DBOUT(L"ScopeController::update_parameters_from_gui()\n");
		if ( parameters.run_state() == RunStateHelper::Mode::Stopped )
			return;

		// Check if online update is commencing right now, abort it (i.e. abort Outputs::Write in DaqController, since this is
		// the bottleneck! Then wait for the future.
		if ( onlineupdate_running ) {
			DBOUT(L"ScopeController::UpdateAreaParametersFromGui aborting previous online update");
			// Signal the DaqControllerImpl to abort current update of scannervec in device buffer
			theDaq.AbortOnlineParameterUpdate(_area);
			// Now wait for the future of the online update (see async lambda below) to make sure the "old" online update
			// is done/aborted
			if ( onlineupdate_future.valid() )
				onlineupdate_future.get();
			// Now the old async online_update is successfully aborted and we can start the current/new one
		}

		// If no online update is running right now, start a new one asynchronically
		if ( !onlineupdate_running ) {
			onlineupdate_future = std::async([&]() {
				DBOUT(L"ScopeController::UpdateAreaParametersFromGui starting new online update");
				onlineupdate_running = true;
				try {
					*parameters.areas[_area] = *GuiParameters.areas[_area];
					DBOUT(L"ScopeController::UpdateAreaParametersFromGui guioffset " << GuiParameters.areas[_area]->Currentframe().xoffset() << L" " << GuiParameters.areas[_area]->Currentframe().yoffset());
					// This is the expensive step, the recalculation of the scannervector
					framescannervecs[_area]->SetParameters(&parameters.areas[_area]->daq, &parameters.areas[_area]->Currentframe(), &parameters.areas[_area]->fpuzstage);

					// Fix for online pockel cell update: somehow in this function the currentframe is always framesaw, so I added an if statement to go straight for frameresonance parameters in the resonance scanmode (Karlis)
					//framescannervecs[_area]->SetParameters(&parameters.areas[_area]->daq, SCOPE_USE_RESONANCESCANNER?&parameters.areas[_area]->frameresonance:parameters.areas[_area]->currentframe, &parameters.areas[_area]->fpuzstage);
					// This returns only after the updated scannervec is written to the device buffer or the whole update write is aborted
					theDaq.OnlineParameterUpdate(*parameters.areas[_area]);
	
					theDisplay.ResolutionChange(*parameters.areas[_area]);
				}
				catch (...) {
					ScopeExceptionHandler(__FUNCTION__);
				}
				// Whatever exceptions happen, we have to set this to false!
				onlineupdate_running = false;
			});
		}
	}

	/** Loads current parameter set from disk
	* @param[in] _filepath path and name of parameters on disk */
	bool LoadParameters(const std::wstring& _filepath) {
		currentconfigfile = _filepath.substr(_filepath.find_last_of(L'\\')+1, std::wstring::npos);
		GuiParameters.Load(_filepath);
		parameters = GuiParameters;
		if ( initialparametersloaded() == false ) {
			InitializeHardware();
			initialparametersloaded = true;
		}
		return true;
	}

	/** Saves the current parameter set to disk
	* @param[in] _filepath path and name to save to */
	bool SaveParameters(const std::wstring& _filepath) {
		parameters = GuiParameters;
		parameters.Save(_filepath);
		return true;
	}

	/** Saves current positions of windows by adding frames to WindowCollection of ScopeController::GuiParameters. */
	void SaveCurrentWindowPositions() {
		// This gets parameters::Windows of CChannelFrames and CHistogramFrames that are attached to the display controller
		GuiParameters.frames = theDisplay.GetWindowCollection();

		GuiParameters.frames.AddWindow(L"CLogFrame", 0, scope_logger.GetLogFrameWindow());
	}

	/** Sets the current xyz stage position as zero */
	void SetStageZero() {
		theStage.SetZero();
	}

	/** Sets the start stack position to the current z position */
	void StackStartHere() {
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			// If z stage every area has the same start position
			if ( GuiParameters.stack.zdevicetype() == ZDeviceHelper::ZStage )
				GuiParameters.stack.startat[a].position = theStage.CurrentZPosition();
			// If fast z then each area can have different positions
			else
				GuiParameters.stack.startat[a].position = GuiParameters.areas[a]->Currentframe().fastz();

			GuiParameters.stack.startat[a].pockels = GuiParameters.areas[a]->Currentframe().pockels();
		}
	}

	/** Sets the stop stack position to the current z position */
	void StackStopHere() {
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			// If z stage every area has the same start position
			if ( GuiParameters.stack.zdevicetype() == ZDeviceHelper::ZStage )
				GuiParameters.stack.stopat[a].position = theStage.CurrentZPosition();
			// If fast z then each area can have different positions
			else
				GuiParameters.stack.stopat[a].position = GuiParameters.areas[a]->Currentframe().fastz();

			GuiParameters.stack.stopat[a].pockels = GuiParameters.areas[a]->Currentframe().pockels();
		}
	}

	/** Calls the DaqController to set Galvo outputs to zero */
	void ZeroGalvoOutputs() {
		if ( parameters.run_state() == RunStateHelper::Mode::Stopped ) {
			DBOUT(L"Zeroing galvo outputs\n");
			theDaq.ZeroGalvoOutputs();
		}
	}

	/** Opens/closes the shutter */
	void OpenCloseShutter(const uint32_t& _area, const bool& _open) {
		theDaq.OpenCloseShutter(_area, _open);
	}

	/** @return current state of the shutter  */
	bool GetShutterState(const uint32_t& _area) const {
		return theDaq.GetShutterState(_area);
	}

	/** Turns the resonance scanner relay on and off. */
	void TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
		theDaq.TurnOnOffSwitchResonance(_area, _on);
	}

	/** @return current state of the first resonance scanner relay  */
	bool GetSwitchResonanceState(const uint32_t& _area) const {
		return theDaq.GetSwitchResonanceState(_area);
	}

	/** Sets the state of the GUI buttons for FPU control (via the corresponding ScopeButtons). True = enabled, false = disabled.  */
	void SetFPUButtonsState(const bool& state) {
		for ( auto& b : FPU ) {
			b.LeftButton.Enable(state);
			b.RightButton.Enable(state);
			b.UpButton.Enable(state);
			b.DownButton.Enable(state);
		}
	}

	/** Sets the state of GUI controls (via the corresponding ScopeNumbers). True = read&write/enabled, false = readonly/disabled.
	* Other GUI elements can connect to ReadOnlyWhileScanning. */
	void SetGuiCtrlState() {
		// Signal other GUI elements (e.g. preset combo box in CFrameScanPage, see CFrameScanPage::SetReadOnlyWhileScanning)
		ReadOnlyWhileScanning.Set((parameters.run_state()==RunStateHelper::Mode::Stopped)?false:true);

		// This takes care of all the parameters
		GuiParameters.SetReadOnlyWhileScanning(parameters.run_state());

		// Now we set the state of all buttons
		const bool buttonenabler = (parameters.run_state()==RunStateHelper::Mode::Stopped)?true:false;

		StartSingleButton.Enable(buttonenabler);
		StartLiveButton.Enable(buttonenabler);
		StartStackButton.Enable(buttonenabler);
		StartTimeseriesButton.Enable(buttonenabler);
		StartBehaviorButton.Enable(buttonenabler);

		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			// Only enable scan mode buttons for master area and only if the mode is supported by builtin scanners
			if ( !ThisIsSlaveArea(a) ) {
				for ( auto& b : ScanMode[a].map ) 
					b.second.Enable(buttonenabler && ScannerSupportedVectors::IsBuiltinSupported(b.first));
			}
			// Disable slave area's scan mode buttons
			else {
				for ( auto& b : ScanMode[a].map ) 
					b.second.Enable(false);
			}
		}
		// Leave these enabled during live scanning
		StackStartHereButton.Enable(buttonenabler || (parameters.run_state() == RunStateHelper::Mode::RunningContinuous));
		StackStopHereButton.Enable(buttonenabler || (parameters.run_state() == RunStateHelper::Mode::RunningContinuous));
	}

	/** Attach a CChannelFrame as observer */
	void AttachFrame(gui::CChannelFrame* const cframe) {
		theDisplay.AttachFrame(cframe);
	}

	/** Detach an observing CChannelFrame */
	void DetachFrame(gui::CChannelFrame* const cframe) {
		theDisplay.DetachFrame(cframe);
	}

	/** Attach a CHistogramFrame as observer */
	void AttachFrame(gui::CHistogramFrame* const hframe) {
		theDisplay.AttachFrame(hframe);
	}

	/** @return true if at least one histogram already attached to theDisplay for that area. */
	bool HistogramAlreadyAttached(const uint32_t& _area)  {
		return theDisplay.HistogramAlreadyAttached(_area);
	}

	/** Detach an observing CHistogramFrame */
	void DetachFrame(gui::CHistogramFrame* const hframe) {
		theDisplay.DetachFrame(hframe);
	}

	/** Called via change of GuiParameters.areas[x].framesaw.scanvec.xres etc. */
	void ResolutionChange(const uint32_t& _area) {
		theDisplay.ResolutionChange(*GuiParameters.areas[_area]);
	}

	/** Sets the color limits for displaying imgages, calls DisplayController::SetHistogramLimits */
	void SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper) {
		theDisplay.SetHistogramLimits(_area, _channel, _lower, _upper);
	}

	/** Sets the type of scanning. Called via ScopeController ScanModeButtons.
	* Calls the scanmode callbacks.
	* Updates/recreates the scanvectors from a different type and sets them in theDaq and thePipeline */
	void SetScanMode(const uint32_t& _area, const ScannerVectorType& _mode) {
		// Make sure the choosen mode/scannervector is supported by the built-in scannertype
		if ( ScannerSupportedVectors::IsBuiltinSupported(_mode) ) {
			// Triggers update of areas.currentframe (connected to areas.ChangeScanMode)
			GuiParameters.areas[_area]->scanmode = _mode;
			parameters = GuiParameters;
			ScannerVectorFillType filltype = (parameters.areas[_area]->isslave())?SCOPE_SLAVEFRAMEVECTORFILL:SCOPE_MASTERFRAMEVECTORFILL;
			framescannervecs[_area] = ScannerVectorFrameBasic::Factory(parameters.areas[_area]->scanmode(), filltype);
			theDaq.SetScannerVector(_area, framescannervecs[_area]);
			thePipeline.SetScannerVector(_area, framescannervecs[_area]);
			for ( auto& c : scanmodecallbacks )
				c(_area, parameters.areas[_area]->scanmode());
		} else {
			throw ScopeException("Choosen ScanMode is not supported by built-in scanner type");
		}
	}

	/** Registers a function to call when scanmode is changed.
	* Usually CScanSettingsSheet registers for switching between FrameScan property pages. */
	void RegisterScanmodeCallback(std::function<void(const uint32_t&, const ScannerVectorType&)> _callback) {
		scanmodecallbacks.push_back(_callback);
	}
};

}