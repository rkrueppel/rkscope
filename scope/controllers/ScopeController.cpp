#include "stdafx.h"
#include "ScopeController.h"
#include "devices\xyz\XYZControl.h"

namespace scope {


	ScopeController::ScopeController(const uint32_t& _nareas
		, parameters::Scope& _guiparameters
		, ScopeCounters<SCOPE_NAREAS>& _counters
		, DaqController& _theDaq
		, PipelineController& _thePipeline
		, StorageController& _theStorage
		, DisplayController& _theDisplay
		, std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>>& _daq_to_pipeline
		, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>& _pipeline_to_storage
		, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>& _pipeline_to_display
		, SCOPE_XYZCONTROL_T& _theStage
	)
		: BaseController(1)
		, nareas(_nareas)
		, guiparameters(_guiparameters)
		, ctrlparams(_guiparameters)
		, counters(_counters)
		, theDaq(_theDaq)
		, thePipeline(_thePipeline)
		, theStorage(_theStorage)
		, theDisplay(_theDisplay)
		, daq_to_pipeline(_daq_to_pipeline)
		, pipeline_to_storage(_pipeline_to_storage)
		, pipeline_to_display(_pipeline_to_display)
		, theStage(_theStage)
		, framescannervecs(_nareas)
		, repeat_abort(false)
		, onlineupdate_running(false)
		, time(0)
	{
	}

	ScopeController::~ScopeController() {
		// Do not call stop() here, every controller should stop in its own destructor!
		//Stop();
		DBOUT(L"ScopeController::~ScopeController\n");
	}

	void ScopeController::LogRun() {
		if (ctrlparams.storage.autosave()) {
			std::wstring msg(L"");
			switch (ctrlparams.run_state().t) {
				case RunStateHelper::RunningContinuous:
					if (ctrlparams.storage.savelive())
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
			scope::ScopeLogger::GetInstance().Log(msg, log_info);
		}
	}

	void ScopeController::StartAllControllers() {
		time = ::timeGetTime();
		theDisplay.Start();
		theStorage.Start();
		thePipeline.Start();
		theDaq.Start(ctrlparams);
	}

	void ScopeController::StopAllControllers() {
		theDisplay.StopAll();
		theStorage.StopAll();
		thePipeline.StopAll();
		theDaq.StopAll();

		try {
			ControllerReturnStatus sdisp = theDisplay.WaitForAll(500);
		}
		catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		try {
			ControllerReturnStatus sstor = theStorage.WaitForAll(500);
		}
		catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		try {
			ControllerReturnStatus spipe = thePipeline.WaitForAll(-1); // 5000
		}
		catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		try {
			ControllerReturnStatus sdaq = theDaq.WaitForAll(2000);
		}
		catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		time = ::timeGetTime() - time;
	}

	void ScopeController::WaitForAllControllers() {
		theDisplay.WaitForAll(400);
		theStorage.WaitForAll(400);
		thePipeline.WaitForAll(400);
		theDaq.WaitForAll(400);
	}

	void ScopeController::ClearAllQueues() {
		for (auto& d : daq_to_pipeline)
			d.Clear();
		pipeline_to_display.Clear();
		pipeline_to_storage.Clear();
	}

	void ScopeController::SetScannerVectorParameters() {
		DBOUT(L"ScopeControllerImpl::SetScannerVectorParameters");
		for (uint32_t a = 0; a < nareas; a++)
			framescannervecs[a]->SetParameters(&ctrlparams.areas[a].daq, &ctrlparams.areas[a].Currentframe(), &ctrlparams.areas[a].fpuzstage);
	}

	ControllerReturnStatus ScopeController::RunLive(StopCondition* const sc) {
		ClearAllQueues();
		SetScannerVectorParameters();
		DBOUT(L"ScopeController::RunLive()");
		StartAllControllers();
		return ControllerReturnStatus::finished;
	}

	ControllerReturnStatus ScopeController::RunSingle(StopCondition* const sc) {
		ClearAllQueues();
		LogRun();
		SetScannerVectorParameters();
		StartAllControllers();
		WaitForAllControllers();
		ClearAfterStop();
		return ControllerReturnStatus::finished;
	}

	ControllerReturnStatus ScopeController::RunStack(StopCondition* const sc) {
		ClearAllQueues();
		LogRun();
		SetScannerVectorParameters();

		// We do the same number of planes in each area (if fast z and not that many planes in range in one area, Pockels was set to zero in parameters::stack::UpdatePlanes)
		for (auto& a : ctrlparams.areas)
			a.daq.requested_frames = ctrlparams.stack.planes___.size();

		// StorageController and DisplayController saves/displays the number of slices in each area
		theStorage.Start();
		theDisplay.Start();

		// At every slice Daq and PipelineController expect 1 frame (they calculate averages themselves)
		//  do not manipulate guiparameters here, otherwise we have a problem on restarting the stack
		for (auto& ap : ctrlparams.areas)
			ap.daq.requested_frames = 1;

		// Set scale for the progress indicator
		counters.planecounter.SetWithLimits(0, 0, ctrlparams.stack.planes___.size());

		// Go through all the precalculated planes
		for (uint32_t p = 0; p < ctrlparams.stack.planes___.size(); p++) {

			// Move to precalculated position (stage position is the same for all areas, no of slices thus also the same)
			if (ctrlparams.stack.zdevicetype().t == ZDeviceHelper::ZStage) {
				theStage.MoveAbsolute(theStage.CurrentXPosition(), theStage.CurrentYPosition(), ctrlparams.stack.planes___[p][0].position());
				// Wait one second for movement to complete
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
			else if (ctrlparams.stack.zdevicetype().t == ZDeviceHelper::FastZ) {
				for (uint32_t a = 0; a < nareas; a++) {
					ctrlparams.areas[a].Currentframe().fastz = ctrlparams.stack.planes___[p][a].position();
					// Also change guiparameters so the users sees what is happening during stacking
					guiparameters.areas[a].Currentframe().fastz = ctrlparams.stack.planes___[p][a].position();
				}
			}

			// Set pockels to precalculated value
			for (uint32_t a = 0; a < nareas; a++) {
				ctrlparams.areas[a].Currentframe().pockels = ctrlparams.stack.planes___[p][a].pockels();
				// Also change guiparameters so the users sees what is happening during stacking
				guiparameters.areas[a].Currentframe().pockels = ctrlparams.stack.planes___[p][a].pockels();
			}

			// Calculate scanner vectors
			SetScannerVectorParameters();

			// Start
			theDaq.Start(ctrlparams);
			thePipeline.Start();

			// and wait for them to end
			theDaq.WaitForAll(400);
			thePipeline.WaitForAll(400);

			// Stop the stack if abort requested
			if (repeat_abort.IsSet())
				break;

			// Advance progress indicator
			counters.planecounter = p + 1;
		}

		theDisplay.WaitForAll(400);
		theStorage.WaitForAll(400);
		ClearAfterStop();

		counters.planecounter = 0;

		return ControllerReturnStatus::finished;
	}

	ControllerReturnStatus ScopeController::RunTimeseries(StopCondition* const sc) {
		LogRun();
		ClearAllQueues();
		SetScannerVectorParameters();

		// Set repeat counter limits (for progress bar display)
		counters.repeatcounter.SetWithLimits(1, 1, ctrlparams.timeseries.repeats());

		// Go through all repeats
		for (uint32_t t = 0; t < ctrlparams.timeseries.repeats(); t++) {
			counters.repeatcounter = t + 1;

			ClearAllQueues();

			// Start a timeseries
			StartAllControllers();

			// Start waiting, wait from beginning of timeseries until beginning of next one
			std::unique_lock<std::mutex> lock(waitbetweenrepeats_mutex);
			std::chrono::milliseconds waittime(static_cast<long>(1000 * ctrlparams.timeseries.betweenrepeats()));
			// Waits or is notified before by a call to Stop
			waitbetweenrepeats_cond.wait_for(lock, waittime);

			// This should return immediately, because waittime is always longer than duration of one timeseries
			// or because StopAllControllers was called from Stop
			WaitForAllControllers();

			// Alternate planes if planes defined
			if (ctrlparams.timeseries.planes.size() != 0) {
				for (uint32_t a = 0; a < nareas; a++)
					ctrlparams.areas[a].Currentframe().fastz = ctrlparams.timeseries.planes[t % ctrlparams.timeseries.planes.size()][a].position();
				SetScannerVectorParameters();
			}

			// Break if abort condition set (from Stop)
			if (repeat_abort.IsSet()) {
				counters.repeatcounter = 1;
				break;
			}
		}

		ClearAfterStop();
		return ControllerReturnStatus::finished;
	}

	ControllerReturnStatus ScopeController::RunBehavior(StopCondition* const sc) {
		LogRun();
		ClearAllQueues();
		SetScannerVectorParameters();
		uint32_t trials = 0;
		DWORD starttime = ::timeGetTime();
		GaterDAQmx gater(ctrlparams.behavior.gateline(), sc);

		// Set trial counter limits
		counters.trialcounter.SetWithLimits(0.0, 0.0, ctrlparams.behavior.unlimited_repeats() ? 10000.0 : ctrlparams.behavior.repeats());

		// Record trials until StopCondition set or requested number of trials is reached
		while (!sc->IsSet() || ctrlparams.behavior.unlimited_repeats() || trials < ctrlparams.behavior.repeats()) {
			// Wait for high signal then start acquisition, on StopCondition break from loop
			if (!gater.WaitFor(true))
				break;

			//Band-Aid Jerry - Forced Behavior Acquisition to run in Live Scan Mode with Live saving enabled
			ctrlparams.time.Set(GetCurrentTimeString());
			ctrlparams.run_state = RunStateHelper::RunningContinuous;
			ctrlparams.storage.savelive = true;

			StartAllControllers();
			// Update time
			counters.totaltime = static_cast<double>(::timeGetTime() - starttime) / 1000;

			// Wait for low signal and then stop acquisition, on StopCondition break from loop
			if (!gater.WaitFor(false))
				break;
			StopAllControllers();
			ClearAllQueues();
			// Update counters and time
			trials++;
			counters.trialcounter = trials;
			counters.totaltime = static_cast<double>(::timeGetTime() - starttime) / 1000;
		}

		ClearAfterStop();
		return ControllerReturnStatus::finished;
	}

	void ScopeController::ClearAfterStop() {
		DBOUT(L"ScopeController::CleanAfterStop");
		ClearAllQueues();
		repeat_abort.Set(false);
		ctrlparams.run_state = RunStateHelper::Stopped;
		guiparameters.run_state = RunStateHelper::Stopped;
		//SetGuiCtrlState();
	}

	void ScopeController::StartLive() {
		if (ctrlparams.run_state() == RunStateHelper::Mode::Stopped) {
			guiparameters.requested_mode.Set(DaqModeHelper::Mode::continuous);
			guiparameters.run_state.Set(RunStateHelper::Mode::RunningContinuous);
			guiparameters.date.Set(GetCurrentDateString());
			guiparameters.time.Set(GetCurrentTimeString());
			ctrlparams = guiparameters;
			//SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&ScopeController::RunLive, this, &stops[0]));
			futures[0].wait();				// Wait here, because RunLive should quickly return
		}
	}

	void ScopeController::StartStack() {
		if (ctrlparams.run_state() == RunStateHelper::Mode::Stopped) {
			guiparameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
			guiparameters.date.Set(GetCurrentDateString());
			guiparameters.time.Set(GetCurrentTimeString());
			guiparameters.run_state.Set(RunStateHelper::Mode::RunningStack);
			ctrlparams = guiparameters;
			//SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&ScopeController::RunStack, this, &stops[0]));
		}
	}

	void ScopeController::StartSingle() {
		if (ctrlparams.run_state() == RunStateHelper::Mode::Stopped) {
			guiparameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
			guiparameters.run_state.Set(RunStateHelper::Mode::RunningSingle);
			guiparameters.date.Set(GetCurrentDateString());
			guiparameters.time.Set(GetCurrentTimeString());
			ctrlparams = guiparameters;
			//SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&ScopeController::RunSingle, this, &stops[0]));
		}
	}

	void ScopeController::StartTimeseries() {
		if (ctrlparams.run_state() == RunStateHelper::Mode::Stopped) {
			// Same number of frames for all areas. However, this is non-ideal...
			for (uint32_t a = 1; a < nareas; ++a)
				guiparameters.timeseries.frames[a] = guiparameters.timeseries.frames[0].Value();
			guiparameters.run_state.Set(RunStateHelper::Mode::RunningTimeseries);
			guiparameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
			guiparameters.date.Set(GetCurrentDateString());
			guiparameters.time.Set(GetCurrentTimeString());
			for (uint32_t a = 0; a < nareas; a++)
				guiparameters.areas[a].daq.requested_frames = guiparameters.timeseries.frames[a]();
			
			ctrlparams = guiparameters;
			//SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&ScopeController::RunTimeseries, this, &stops[0]));
		}
	}

	void ScopeController::StartBehavior() {
		if (ctrlparams.run_state() == RunStateHelper::Mode::Stopped) {
			guiparameters.run_state.Set(RunStateHelper::Mode::RunningBehavior);
			guiparameters.requested_mode.Set(DaqModeHelper::Mode::continuous);
			guiparameters.date.Set(GetCurrentDateString());
			guiparameters.time.Set(GetCurrentTimeString());
			ctrlparams = guiparameters;
			//SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&ScopeController::RunBehavior, this, &stops[0]));
		}
	}

	void ScopeController::Stop() {
		DBOUT(L"ScopeController::stop()\n");
		stops[0].Set();

		if ((ctrlparams.run_state() == RunStateHelper::RunningStack) || (ctrlparams.run_state() == RunStateHelper::Mode::RunningTimeseries))
			repeat_abort.Set(true);

		// Notify the condition variable waiting betweenrepeats in RunTimeseries
		if (ctrlparams.run_state() == RunStateHelper::Mode::RunningTimeseries)
			waitbetweenrepeats_cond.notify_one();

		if (ctrlparams.run_state() != RunStateHelper::Mode::Stopped) {
			// If we currently running an online update, wait until it is ready before stopping controllers!
			if (onlineupdate_future.valid())
				onlineupdate_future.get();

			StopAllControllers();
			// If we ran live we have to call the ClearAfterStop here (since RunLive does not wait for scanning to finish. How could it...)
			if (static_cast<RunState>(ctrlparams.run_state()) == RunStateHelper::Mode::RunningContinuous) {
				ClearAfterStop();
			}
			//futures[0].wait();
		}
		DBOUT(L"ScopeController::stop end()\n");
	}

	void ScopeController::OnlineUpdate(const uint32_t& _area) {
		DBOUT(L"ScopeController::update_parameters_from_gui()\n");
		if (ctrlparams.run_state() == RunStateHelper::Mode::Stopped)
			return;

		// Check if online update is commencing right now, abort it (i.e. abort Outputs::Write in DaqController, since this is
		// the bottleneck! Then wait for the future.
		if (onlineupdate_running) {
			DBOUT(L"ScopeController::UpdateAreaParametersFromGui aborting previous online update");
			// Signal the DaqControllerImpl to abort current update of scannervec in device buffer
			theDaq.AbortOnlineParameterUpdate(_area);
			// Now wait for the future of the online update (see async lambda below) to make sure the "old" online update
			// is done/aborted
			if (onlineupdate_future.valid())
				onlineupdate_future.get();
			// Now the old async online_update is successfully aborted and we can start the current/new one
		}

		// If no online update is running right now, start a new one asynchronically
		if (!onlineupdate_running) {
			onlineupdate_future = std::async([&]() {
				DBOUT(L"ScopeController::UpdateAreaParametersFromGui starting new online update");
				onlineupdate_running = true;
				try {
					ctrlparams.areas[_area] = guiparameters.areas[_area];
					DBOUT(L"ScopeController::UpdateAreaParametersFromGui guioffset " << guiparameters.areas[_area].Currentframe().xoffset() << L" " << guiparameters.areas[_area].Currentframe().yoffset());
					// This is the expensive step, the recalculation of the scannervector
					framescannervecs[_area]->SetParameters(&ctrlparams.areas[_area].daq, &ctrlparams.areas[_area].Currentframe(), &ctrlparams.areas[_area].fpuzstage);

					// Fix for online pockel cell update: somehow in this function the currentframe is always framesaw, so I added an if statement to go straight for frameresonance parameters in the resonance scanmode (Karlis)
					//framescannervecs[_area]->SetParameters(&parameters.areas[_area]->daq, SCOPE_USE_RESONANCESCANNER?&parameters.areas[_area]->frameresonance:parameters.areas[_area]->currentframe, &parameters.areas[_area]->fpuzstage);
					// This returns only after the updated scannervec is written to the device buffer or the whole update write is aborted
					theDaq.OnlineParameterUpdate(ctrlparams.areas[_area]);

					theDisplay.ResolutionChange(ctrlparams.areas[_area]);
				}
				catch (...) {
					ScopeExceptionHandler(__FUNCTION__);
				}
				// Whatever exceptions happen, we have to set this to false!
				onlineupdate_running = false;
			});
		}
	}


}