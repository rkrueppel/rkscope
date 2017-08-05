#include "stdafx.h"
#include "ScopeController.h"
#include "ScopeController_p.h"

namespace scope {

std::atomic<bool> ScopeController::instanciated(false);

ScopeController::ScopeController(const uint32_t& _nareas)
	: BaseController(1, parameters::Scope())
	, nareas(_nareas)
	, daq_to_pipeline(_nareas)
	, pipeline_to_storage()
	, pipeline_to_display()
	, framescannervecs(_nareas)
	, theDaq(&daq_to_pipeline, parameters)
	, thePipeline(&daq_to_pipeline, &pipeline_to_storage, &pipeline_to_display, parameters)
	, theStorage(&pipeline_to_storage, parameters)
	, theDisplay(&pipeline_to_display, parameters)
	, repeat_abort(false)
	, onlineupdate_running(false)
	, time(0)
	, initialparametersloaded(false, false, true)
	, currentconfigfile(L"NONE")
	, readonlywhilescanning(false, false, true, L"ReadOnlyWhileScanning")
	, singleframeprogress(_nareas)
	, planecounter(0.0, 0.0, 100.0, L"PlaneCounter")
	, framecounter(_nareas)
	, repeatcounter(0.0, 0.0, 1000.0, L"RepeatCounter")
	, trialcounter(0.0, 0.0, 10000.0, L"TotalTime")
	, totaltime(0.0, 0.0, 10000.0, L"TrialCounter")
	, fpubuttonsvec(_nareas)
	, scanmodebuttonsvec(_nareas) {
	
	//Make sure that ScopeController is instanciated only once
	std::assert(!instanciated);
	
	// Connect buttons to functions
	// QuitButton is connected in CMainDlgFrame
	StartSingleButton.Connect(std::bind(&StartSingle, this));
	StartLiveButton.Connect(std::bind(&StartLive, this));
	StartStackButton.Connect(std::bind(&StartStack, this));
	StartTimeseriesButton.Connect(std::bind(&StartTimeseries, this));
	StartBehaviorButton.Connect(std::bind(&StartBehavior, this));
	StopButton.Connect(std::bind(&Stop, this));

	// We cannot connect directly to XYZStage::SetZero because std::bind needs the class to be copyable (which it may  not)
	StageZeroButton.Connect(std::bind(&SetStageZero, this));

	StackStartHereButton.Connect(std::bind(&StackStartHere, this));
	StackStopHereButton.Connect(std::bind(&StackStopHere, this));

	for (uint32_t a = 0; a < nareas; a++) {
		// Initially choose the first supported scannervector in the list
		SetScanMode(a, *ScannerSupportedVectors::List().begin());

		// Set counters to zero
		framecounter[a] = 0;
		singleframeprogress[a] = 0;

		// Connect the buttons for scan mode switching (if a master area)
		if (!ThisIsSlaveArea(a)) {
			for (auto& b : ScanMode[a].map)
				b.second.Connect(std::bind(&SetScanMode, this, a, b.first));
		}

		// Connect all imaging parameters
		for (auto& sv : GuiParameters.areas[a]->scannervectorframesmap) {
			sv.second->ConnectOnlineUpdate(std::bind(&UpdateAreaParametersFromGui, this, a));
			sv.second->ConnectResolutionChange(std::bind(&ResolutionChange, this, a));
		}

		GuiParameters.areas[a]->daq.pixeltime.ConnectOther(std::bind(&UpdateAreaParametersFromGui, this, a));
		GuiParameters.areas[a]->daq.scannerdelay.ConnectOther(std::bind(&UpdateAreaParametersFromGui, this, a));
		GuiParameters.areas[a]->histrange.ConnectOther(std::bind(&UpdateAreaParametersFromGui, this, a));
		//GuiParameters.areas[a]->frameresonance.yres.ConnectOther(std::bind(&ResolutionChange, this, a));

		// Connect FPU XY movements inside the FPUController!!
	}

	SetGuiCtrlState();
}

ScopeController::~ScopeController() {
	// Do not call stop() here, every controller should stop in its own destructor!
	//Stop();
	DBOUT(L"ScopeController::~ScopeController\n");
}

void ScopeController::LogRun() {
	if (parameters.storage.autosave()) {
		std::wstring msg(L"");
		switch (parameters.run_state().t) {
		case RunStateHelper::RunningContinuous:
			if (parameters.storage.savelive())
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
	theDisplay.Start(parameters);
	theStorage.Start(parameters);
	thePipeline.Start(parameters);
	theDaq.Start(parameters);
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
	theDisplay.WaitForAll();
	theStorage.WaitForAll();
	thePipeline.WaitForAll();
	theDaq.WaitForAll();
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
		framescannervecs[a]->SetParameters(&parameters.areas[a]->daq, &parameters.areas[a]->Currentframe(), &parameters.areas[a]->fpuzstage);
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
	for (auto& a : parameters.areas)
		a->daq.requested_frames = parameters.stack.planes___.size();

	// StorageController and DisplayController saves/displays the number of slices in each area
	theStorage.Start(parameters);
	theDisplay.Start(parameters);

	// At every slice Daq and PipelineController expect 1 frame (they calculate averages themselves)
	//  do not manipulate GuiParameters here, otherwise we have a problem on restarting the stack
	for (auto& ap : parameters.areas)
		ap->daq.requested_frames = 1;

	// Set scale for the progress indicator
	PlaneCounter.SetWithLimits(0, 0, parameters.stack.planes___.size());

	// Go through all the precalculated planes
	for (uint32_t p = 0; p < parameters.stack.planes___.size(); p++) {

		// Move to precalculated position (stage position is the same for all areas, no of slices thus also the same)
		if (parameters.stack.zdevicetype().t == ZDeviceHelper::ZStage) {
			theStage.MoveAbsolute(theStage.CurrentXPosition(), theStage.CurrentYPosition(), parameters.stack.planes___[p][0].position());
			// Wait one second for movement to complete
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		else if (parameters.stack.zdevicetype().t == ZDeviceHelper::FastZ) {
			for (uint32_t a = 0; a < nareas; a++) {
				parameters.areas[a]->Currentframe().fastz = parameters.stack.planes___[p][a].position();
				// Also change GuiParameters so the users sees what is happening during stacking
				GuiParameters.areas[a]->Currentframe().fastz = parameters.stack.planes___[p][a].position();
			}
		}

		// Set pockels to precalculated value
		for (uint32_t a = 0; a < nareas; a++) {
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
		if (repeat_abort.IsSet())
			break;

		// Advance progress indicator
		PlaneCounter = p + 1;
	}

	theDisplay.WaitForAll();
	theStorage.WaitForAll();
	ClearAfterStop();

	PlaneCounter = 0;

	return ControllerReturnStatus::finished;
}

ControllerReturnStatus ScopeController::RunTimeseries(StopCondition* const sc) {
	LogRun();
	ClearAllQueues();
	SetScannerVectorParameters();

	// Set repeat counter limits (for progress bar display)
	RepeatCounter.SetWithLimits(1, 1, parameters.timeseries.repeats());

	// Go through all repeats
	for (uint32_t t = 0; t < parameters.timeseries.repeats(); t++) {
		RepeatCounter = t + 1;

		ClearAllQueues();

		// Start a timeseries
		StartAllControllers();

		// Start waiting, wait from beginning of timeseries until beginning of next one
		std::unique_lock<std::mutex> lock(waitbetweenrepeats_mutex);
		std::chrono::milliseconds waittime(static_cast<long>(1000 * parameters.timeseries.betweenrepeats()));
		// Waits or is notified before by a call to Stop
		waitbetweenrepeats_cond.wait_for(lock, waittime);

		// This should return immediately, because waittime is always longer than duration of one timeseries
		// or because StopAllControllers was called from Stop
		WaitForAllControllers();

		// Alternate planes if planes defined
		if (parameters.timeseries.planes.size() != 0) {
			for (uint32_t a = 0; a < nareas; a++)
				parameters.areas[a]->Currentframe().fastz = parameters.timeseries.planes[t % parameters.timeseries.planes.size()][a].position();
			SetScannerVectorParameters();
		}

		// Break if abort condition set (from Stop)
		if (repeat_abort.IsSet()) {
			RepeatCounter = 1;
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
	GaterDAQmx gater(parameters.behavior.gateline(), sc);

	// Set trial counter limits
	TrialCounter.SetWithLimits(0.0, 0.0, parameters.behavior.unlimited_repeats() ? 10000.0 : parameters.behavior.repeats());

	// Record trials until StopCondition set or requested number of trials is reached
	while (!sc->IsSet() || parameters.behavior.unlimited_repeats() || trials < parameters.behavior.repeats()) {
		// Wait for high signal then start acquisition, on StopCondition break from loop
		if (!gater.WaitFor(true))
			break;

		//Band-Aid Jerry - Forced Behavior Acquisition to run in Live Scan Mode with Live saving enabled
		parameters.time.Set(GetCurrentTimeString());
		parameters.run_state = RunStateHelper::RunningContinuous;
		parameters.storage.savelive = true;

		StartAllControllers();
		// Update time
		TotalTime = static_cast<double>(::timeGetTime() - starttime) / 1000;

		// Wait for low signal and then stop acquisition, on StopCondition break from loop
		if (!gater.WaitFor(false))
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

void ScopeController::ClearAfterStop() {
	DBOUT(L"ScopeController::CleanAfterStop");
	ClearAllQueues();
	repeat_abort.Set(false);
	parameters.run_state = RunStateHelper::Stopped;
	GuiParameters.run_state = RunStateHelper::Stopped;
	SetGuiCtrlState();
}

void ScopeController::PrepareQuit() {
	SetFPUButtonsState(false);
	StopButton.Enable(false);
	theStage.StopPolling();
	for (const auto& s : theFPUs.theXYStages)
		s->StopPolling();
	Stop();
	SetGuiCtrlState();
}

void ScopeController::Version() const {
	// Extract last commit date
	std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
	revstr = L"Scope (Last Git commit " + revstr + L")";
	DBOUT(revstr.c_str());
}

std::wstring ScopeController::CurrentConfigFile() const {
	return currentconfigfile;
}

void ScopeController::InitializeHardware() {
	// Give GuiParameters by reference, so hardware has parameters and can connect to ScopeNumbers
	theStage.Initialize(GuiParameters.stage);
	theFPUs.Initialize(GuiParameters);
}

void ScopeController::StartLive() {
	if (parameters.run_state() == RunStateHelper::Mode::Stopped) {
		GuiParameters.requested_mode.Set(DaqModeHelper::Mode::continuous);
		GuiParameters.run_state.Set(RunStateHelper::Mode::RunningContinuous);
		GuiParameters.date.Set(GetCurrentDateString());
		GuiParameters.time.Set(GetCurrentTimeString());
		parameters = GuiParameters;
		SetGuiCtrlState();
		stops[0].Set(false);
		futures[0] = std::async(std::bind(&RunLive, this, &stops[0]));
		futures[0].wait();				// Wait here, because RunLive should quickly return
	}
}

void ScopeController::StartStack() {
	if (parameters.run_state() == RunStateHelper::Mode::Stopped) {
		GuiParameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
		GuiParameters.date.Set(GetCurrentDateString());
		GuiParameters.time.Set(GetCurrentTimeString());
		GuiParameters.run_state.Set(RunStateHelper::Mode::RunningStack);
		parameters = GuiParameters;
		SetGuiCtrlState();
		stops[0].Set(false);
		futures[0] = std::async(std::bind(&RunStack, this, &stops[0]));
	}
}

void ScopeController::StartSingle() {
	if (parameters.run_state() == RunStateHelper::Mode::Stopped) {
		GuiParameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
		GuiParameters.run_state.Set(RunStateHelper::Mode::RunningSingle);
		GuiParameters.date.Set(GetCurrentDateString());
		GuiParameters.time.Set(GetCurrentTimeString());
		parameters = GuiParameters;
		SetGuiCtrlState();
		stops[0].Set(false);
		futures[0] = std::async(std::bind(&RunSingle, this, &stops[0]));
	}
}

void ScopeController::StartTimeseries() {
	if (parameters.run_state() == RunStateHelper::Mode::Stopped) {
		// Same number of frames for all areas. However, this is non-ideal...
		for (uint32_t a = 1; a < nareas; ++a)
			GuiParameters.timeseries.frames[a] = GuiParameters.timeseries.frames[0].Value();
		GuiParameters.run_state.Set(RunStateHelper::Mode::RunningTimeseries);
		GuiParameters.requested_mode.Set(DaqModeHelper::Mode::nframes);
		GuiParameters.date.Set(GetCurrentDateString());
		GuiParameters.time.Set(GetCurrentTimeString());
		for (uint32_t a = 0; a < nareas; a++)
			GuiParameters.areas[a]->daq.requested_frames = GuiParameters.timeseries.frames[a]();
		parameters = GuiParameters;
		SetGuiCtrlState();
		stops[0].Set(false);
		futures[0] = std::async(std::bind(&RunTimeseries, this, &stops[0]));
	}
}

void ScopeController::StartBehavior() {
	if (parameters.run_state() == RunStateHelper::Mode::Stopped) {
		GuiParameters.run_state.Set(RunStateHelper::Mode::RunningBehavior);
		GuiParameters.requested_mode.Set(DaqModeHelper::Mode::continuous);
		GuiParameters.date.Set(GetCurrentDateString());
		GuiParameters.time.Set(GetCurrentTimeString());
		parameters = GuiParameters;
		SetGuiCtrlState();
		stops[0].Set(false);
		futures[0] = std::async(std::bind(&RunBehavior, this, &stops[0]));
	}
}

void ScopeController::Stop() {
	DBOUT(L"ScopeController::stop()\n");
	stops[0].Set();

	if ((parameters.run_state() == RunStateHelper::RunningStack) || (parameters.run_state() == RunStateHelper::Mode::RunningTimeseries))
		repeat_abort.Set(true);

	// Notify the condition variable waiting betweenrepeats in RunTimeseries
	if (parameters.run_state() == RunStateHelper::Mode::RunningTimeseries)
		waitbetweenrepeats_cond.notify_one();

	if (parameters.run_state() != RunStateHelper::Mode::Stopped) {
		// If we currently running an online update, wait until it is ready before stopping controllers!
		if (onlineupdate_future.valid())
			onlineupdate_future.get();

		StopAllControllers();
		// If we ran live we have to call the ClearAfterStop here (since RunLive does not wait for scanning to finish. How could it...)
		if (static_cast<RunState>(parameters.run_state()) == RunStateHelper::Mode::RunningContinuous) {
			ClearAfterStop();
		}
		//futures[0].wait();
	}
	DBOUT(L"ScopeController::stop end()\n");
}

void ScopeController::UpdateAreaParametersFromGui(const uint32_t& _area) {
	DBOUT(L"ScopeController::update_parameters_from_gui()\n");
	if (parameters.run_state() == RunStateHelper::Mode::Stopped)
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

bool ScopeController::LoadParameters(const std::wstring& _filepath) {
	currentconfigfile = _filepath.substr(_filepath.find_last_of(L'\\') + 1, std::wstring::npos);
	GuiParameters.Load(_filepath);
	parameters = GuiParameters;
	if (initialparametersloaded() == false) {
		InitializeHardware();
		initialparametersloaded = true;
	}
	return true;
}

bool ScopeController::SaveParameters(const std::wstring& _filepath) {
	parameters = GuiParameters;
	parameters.Save(_filepath);
	return true;
}

void ScopeController::SaveCurrentWindowPositions() {
	// This gets parameters::Windows of CChannelFrames and CHistogramFrames that are attached to the display controller
	GuiParameters.frames = theDisplay.GetWindowCollection();

	GuiParameters.frames.AddWindow(L"CLogFrame", 0, ScopeLogger::GetInstance().GetLogFrameWindow());
}

void ScopeController::SetStageZero() {
	theStage.SetZero();
}

void ScopeController::StackStartHere() {
	for (uint32_t a = 0; a < nareas; a++) {
		// If z stage every area has the same start position
		if (GuiParameters.stack.zdevicetype() == ZDeviceHelper::ZStage)
			GuiParameters.stack.startat[a].position = theStage.CurrentZPosition();
		// If fast z then each area can have different positions
		else
			GuiParameters.stack.startat[a].position = GuiParameters.areas[a]->Currentframe().fastz();

		GuiParameters.stack.startat[a].pockels = GuiParameters.areas[a]->Currentframe().pockels();
	}
}

void ScopeController::StackStopHere() {
	for (uint32_t a = 0; a < nareas; a++) {
		// If z stage every area has the same start position
		if (GuiParameters.stack.zdevicetype() == ZDeviceHelper::ZStage)
			GuiParameters.stack.stopat[a].position = theStage.CurrentZPosition();
		// If fast z then each area can have different positions
		else
			GuiParameters.stack.stopat[a].position = GuiParameters.areas[a]->Currentframe().fastz();

		GuiParameters.stack.stopat[a].pockels = GuiParameters.areas[a]->Currentframe().pockels();
	}
}

void ScopeController::ZeroGalvoOutputs() {
	if (parameters.run_state() == RunStateHelper::Mode::Stopped) {
		DBOUT(L"Zeroing galvo outputs\n");
		theDaq.ZeroGalvoOutputs();
	}
}

void ScopeController::OpenCloseShutter(const uint32_t& _area, const bool& _open) {
	theDaq.OpenCloseShutter(_area, _open);
}

bool ScopeController::GetShutterState(const uint32_t& _area) const {
	return theDaq.GetShutterState(_area);
}

void ScopeController::TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
	theDaq.TurnOnOffSwitchResonance(_area, _on);
}

/** @return current state of the first resonance scanner relay  */
bool ScopeController::GetSwitchResonanceState(const uint32_t& _area) const {
	return theDaq.GetSwitchResonanceState(_area);
}

void ScopeController::SetFPUButtonsState(const bool& state) {
	for (auto& b : FPU) {
		b.LeftButton.Enable(state);
		b.RightButton.Enable(state);
		b.UpButton.Enable(state);
		b.DownButton.Enable(state);
	}
}

void ScopeController::SetGuiCtrlState() {
	// Signal other GUI elements (e.g. preset combo box in CFrameScanPage, see CFrameScanPage::SetReadOnlyWhileScanning)
	ReadOnlyWhileScanning.Set((parameters.run_state() == RunStateHelper::Mode::Stopped) ? false : true);

	// This takes care of all the parameters
	GuiParameters.SetReadOnlyWhileScanning(parameters.run_state());

	// Now we set the state of all buttons
	const bool buttonenabler = (parameters.run_state() == RunStateHelper::Mode::Stopped) ? true : false;

	StartSingleButton.Enable(buttonenabler);
	StartLiveButton.Enable(buttonenabler);
	StartStackButton.Enable(buttonenabler);
	StartTimeseriesButton.Enable(buttonenabler);
	StartBehaviorButton.Enable(buttonenabler);

	for (uint32_t a = 0; a < nareas; a++) {
		// Only enable scan mode buttons for master area and only if the mode is supported by builtin scanners
		if (!ThisIsSlaveArea(a)) {
			for (auto& b : scanmodebuttonsvec[a].map)
				b.second.Enable(buttonenabler && ScannerSupportedVectors::IsBuiltinSupported(b.first));
		}
		// Disable slave area's scan mode buttons
		else {
			for (auto& b : scanmodebuttonsvec[a].map)
				b.second.Enable(false);
		}
	}
	// Leave these enabled during live scanning
	StackStartHereButton.Enable(buttonenabler || (parameters.run_state() == RunStateHelper::Mode::RunningContinuous));
	StackStopHereButton.Enable(buttonenabler || (parameters.run_state() == RunStateHelper::Mode::RunningContinuous));
}

void ScopeController::AttachFrame(gui::CChannelFrame* const cframe) {
	theDisplay.AttachFrame(cframe);
}


void ScopeController::DetachFrame(gui::CChannelFrame* const cframe) {
	theDisplay.DetachFrame(cframe);
}


void ScopeController::AttachFrame(gui::CHistogramFrame* const hframe) {
	theDisplay.AttachFrame(hframe);
}


bool ScopeController::HistogramAlreadyAttached(const uint32_t& _area) {
	return theDisplay.HistogramAlreadyAttached(_area);
}


void ScopeController::DetachFrame(gui::CHistogramFrame* const hframe) {
	theDisplay.DetachFrame(hframe);
}

void ScopeController::ResolutionChange(const uint32_t& _area) {
	theDisplay.ResolutionChange(*GuiParameters.areas[_area]);
}

void ScopeController::SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper) {
	theDisplay.SetHistogramLimits(_area, _channel, _lower, _upper);
}

void ScopeController::SetScanMode(const uint32_t& _area, const ScannerVectorType& _mode) {
	// Make sure the choosen mode/scannervector is supported by the built-in scannertype
	if (ScannerSupportedVectors::IsBuiltinSupported(_mode)) {
		// Triggers update of areas.currentframe (connected to areas.ChangeScanMode)
		GuiParameters.areas[_area]->scanmode = _mode;
		parameters = GuiParameters;
		ScannerVectorFillType filltype = (parameters.areas[_area]->isslave()) ? SCOPE_SLAVEFRAMEVECTORFILL : SCOPE_MASTERFRAMEVECTORFILL;
		framescannervecs[_area] = ScannerVectorFrameBasic::Factory(parameters.areas[_area]->scanmode(), filltype);
		theDaq.SetScannerVector(_area, framescannervecs[_area]);
		thePipeline.SetScannerVector(_area, framescannervecs[_area]);
		for (auto& c : scanmodecallbacks)
			c(_area, parameters.areas[_area]->scanmode());
	}
	else {
		throw ScopeException("Choosen ScanMode is not supported by built-in scanner type");
	}
}

void ScopeController::RegisterScanmodeCallback(std::function<void(const uint32_t&, const ScannerVectorType&)> _callback) {
	scanmodecallbacks.push_back(_callback);
}

}