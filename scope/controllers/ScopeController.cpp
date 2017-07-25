#include "stdafx.h"
#include "ScopeController.h"
#include "ScopeController_p.h"

namespace scope {

parameters::Scope ScopeController::GuiParameters;
ScopeButton ScopeController::StartSingleButton;
ScopeButton ScopeController::StartLiveButton;
ScopeButton ScopeController::StartStackButton;
ScopeButton ScopeController::StartTimeseriesButton;
ScopeButton ScopeController::StartBehaviorButton;
ScopeButton ScopeController::StopButton;
ScopeButton ScopeController::QuitButton;
ScopeButton ScopeController::StackStartHereButton;
ScopeButton ScopeController::StackStopHereButton;
std::array<ScopeNumber<double>, SCOPE_NAREAS> ScopeController::SingleFrameProgress;
ScopeNumber<double> ScopeController::PlaneCounter(0.0, 0.0, 100.0, L"PlaneCounter");
std::array<ScopeNumber<double>, SCOPE_NAREAS> ScopeController::FrameCounter;
ScopeNumber<double> ScopeController::RepeatCounter(0.0, 0.0, 1000.0, L"RepeatCounter");
ScopeNumber<double> ScopeController::TotalTime(0.0, 0.0, 10000.0, L"TotalTime");
ScopeNumber<double> ScopeController::TrialCounter(0.0, 0.0, 10000.0, L"TrialCounter");
ScopeButton ScopeController::StageZeroButton;
ScopeNumber<bool> ScopeController::ReadOnlyWhileScanning(false, false, true, L"ReadOnlyWhileScanning");
std::array<ScopeController::FPUButtons, SCOPE_NAREAS> ScopeController::FPU;
std::array<ScopeController::ScanModeButtons, SCOPE_NAREAS> ScopeController::ScanMode;

ScopeController::Impl& ScopeController::GetImpl() {
	static Impl sImpl;
	return sImpl;
}

ScopeController::Impl* const ScopeController::Pimpl() const {
	return static_cast<Impl*>(BaseController<1, true>::Pimpl());
}

ScopeController::ScopeController()
	: BaseController(&GetImpl()) {
}

ScopeController::~ScopeController() {
	// Do not call stop() here, every controller should stop in its own destructor!
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
	for (uint32_t a = 0; a < SCOPE_NAREAS; a++)
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
			for (uint32_t a = 0; a < SCOPE_NAREAS; a++) {
				parameters.areas[a]->Currentframe().fastz = parameters.stack.planes___[p][a].position();
				// Also change GuiParameters so the users sees what is happening during stacking
				GuiParameters.areas[a]->Currentframe().fastz = parameters.stack.planes___[p][a].position();
			}
		}

		// Set pockels to precalculated value
		for (uint32_t a = 0; a < SCOPE_NAREAS; a++) {
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
			for (uint32_t a = 0; a < SCOPE_NAREAS; a++)
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

}