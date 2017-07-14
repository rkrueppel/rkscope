#pragma once

#include "ScopeController.h"

namespace scope {

class ScopeControllerModeBasic
	: public ScopeController {
		
};


/** The implementation class of the ScopeController. */
class ScopeController::ModeLiveImpl
	: public ScopeControllerModeBasic {

	/** Starts live scanning by running RunLive asynchronously */
	void Start() {
		/*if ( parameters.run_state() == RunStateHelper::Mode::Stopped ) {
			GuiParameters.requested_mode.Set(DaqModeHelper::Mode::continuous);
			GuiParameters.run_state.Set(RunStateHelper::Mode::RunningContinuous);
			GuiParameters.date.Set(GetCurrentDateString());
			GuiParameters.time.Set(GetCurrentTimeString());
			parameters = GuiParameters;
			SetGuiCtrlState();
			stops[0].Set(false);
			futures[0] = std::async(std::bind(&ModeLiveImpl::Run, this, &stops[0]));
			futures[0].wait();				// Wait here, because RunLive should quickly return
		}*/
	}

	/** Worker function to control live scanning (basically only starting everything up) */
	ControllerReturnStatus Run(StopCondition* const sc) {
		//ClearAllQueues();
		//SetScannerVectorParameters();
		DBOUT(L"ScopeController::RunLive()");
		//StartAllControllers();
		return ControllerReturnStatus::finished;
	}

	
};

}