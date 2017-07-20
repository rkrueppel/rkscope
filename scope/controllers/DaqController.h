#pragma once

#include "ScopeDefines.h"
#include "BaseController.h"

namespace scope {

/** @ingroup ScopeControl
* The DaqController controls the data acquisition hardware, both outputs for scanners as well as input from PMTs.
* Uses the pimpl pattern to hide the implementation\n
* When working with National Instruments DAQmx library:\n
* See http://www.ni.com/white-paper/6411/en#toc5 for DAQmx thread-safety\n
* => "You can execute two different analog inputs from two different boards from separate threads within the same program,
* and they both run simultaneously without blocking" */
class DaqController
	: public BaseController {

public:
	/** Connect queues and get scanner vector and parameters */
	DaqController(std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS>* const _oqueues, const parameters::Scope& _parameters);

	/** disable copy */
	DaqController(DaqController& other) = delete;

	/** disable assignment */
	DaqController& operator=(DaqController& other) = delete;

	/** Stops all */
	~DaqController();

	/** Changes daq parameters during live scan. Calls DaqController::Impl::OnlineParameterUpdate. */
	void OnlineParameterUpdate(const parameters::Area& _areaparameters);

	/** Aborts a potentially currently running online update. Calls DaqController::Impl::AbortOnlineParametersUpdate. */
	void AbortOnlineParameterUpdate(const uint32_t& _area);

	/** Sets all galvos to zero position, needed for  microscope alignment. Calls DaqController::Impl::ZeroGalvoOutputs. */
	void ZeroGalvoOutputs();

	/** Sets a scanner vector. Only called on startup. Calls DaqController::Impl::SetScannerVector */
	void SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv);

	/** Opens/closes the shutter. Calls DaqController::Impl::OpenCloseShutter */
	void OpenCloseShutter(const uint32_t& _area, const bool& _open);

	/** @return current shutter state */
	bool GetShutterState(const uint32_t& _area) const;

	/** Turns the resonance scanner relay on and off. Calls DaqControllerImpl::TurnOnOffSwitchResonance */
	void TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on);

	/** @return current resonance scanner relay state */
	bool GetSwitchResonanceState(const uint32_t& _area) const;
};

}