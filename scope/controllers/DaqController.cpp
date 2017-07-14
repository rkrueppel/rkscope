#include "stdafx.h"
#include "DaqController.h"
#include "DaqController_p.h"

namespace scope {

DaqController::DaqController(std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS>* const _oqueues, const parameters::Scope& _parameters)
	: BaseController( new Impl(_oqueues, _parameters)) {
}

DaqController::~DaqController() {
	DBOUT(L"DaqController::~DaqController\n");
	StopAll();
}

DaqController::DaqController(Impl* newpimpl)
	: BaseController(newpimpl) {
}

DaqController::Impl* const DaqController::Pimpl() const {
	return static_cast<DaqController::Impl*>(BaseController<SCOPE_NAREAS>::Pimpl());
}

void DaqController::OnlineParameterUpdate(const parameters::Area& _areaparameters) {
	Pimpl()->OnlineParameterUpdate(_areaparameters);
}

void DaqController::AbortOnlineParameterUpdate(const uint32_t& _area) {
	Pimpl()->AbortOnlineParameterUpdate(_area);
}

void DaqController::ZeroGalvoOutputs() {
	Pimpl()->ZeroGalvoOutputs();
}

void DaqController::SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv) {
	Pimpl()->SetScannerVector(_area, _sv);
}

void DaqController::OpenCloseShutter(const uint32_t& _area, const bool& _open) {
	Pimpl()->OpenCloseShutter(_area, _open);
}

bool DaqController::GetShutterState(const uint32_t& _area) const {
	return Pimpl()->GetShutterState(_area);
}

void DaqController::TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
	Pimpl()->TurnOnOffSwitchResonance(_area, _on);
}

bool DaqController::GetSwitchResonanceState(const uint32_t& _area) const {
	return Pimpl()->GetSwitchResonanceState(_area);
}

}