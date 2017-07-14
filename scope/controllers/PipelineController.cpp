#include "stdafx.h"
#include "PipelineController.h"
#include "PipelineController_p.h"

namespace scope {

PipelineController::PipelineController(std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS>* const _iqueues
	, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _squeue
	, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _dqueue
	, const parameters::Scope& _parameters)
	: BaseController( new PipelineController::Impl(_iqueues, _squeue, _dqueue, _parameters)) {
	DBOUT(L"PipelineController::PipelineController");
}

PipelineController::~PipelineController() {
	DBOUT(L"PipelineController::~PipelineController");
}

PipelineController::Impl* const PipelineController::Pimpl() const {
	return static_cast<PipelineController::Impl*>(BaseController<SCOPE_NAREAS>::Pimpl());
}

void PipelineController::SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv) {
	Pimpl()->SetScannerVector(_area, _sv);
}

}