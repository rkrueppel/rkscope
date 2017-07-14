#include "StdAfx.h"
#include "DisplayController.h"
#include "DisplayController_p.h"

namespace scope {

DisplayController::DisplayController(SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue, const parameters::Scope& _parameters)
	: BaseController( new Impl(_iqueue, _parameters) ) {
}

DisplayController::Impl* const DisplayController::Pimpl() const {
	return static_cast<DisplayController::Impl*>(BaseController<1>::Pimpl());
}

void DisplayController::SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper) {
	Pimpl()->SetHistogramLimits(_area, _channel, _lower, _upper);
}

void DisplayController::ResolutionChange(const parameters::Area& _parameters) {
	Pimpl()->ResolutionChange(_parameters);
}

void DisplayController::AttachFrame(gui::CChannelFrame* const cframe) {
	Pimpl()->AttachFrame(cframe);
}

void DisplayController::DetachFrame(gui::CChannelFrame* const cframe) {
	Pimpl()->DetachFrame(cframe);
}

void DisplayController::AttachFrame(gui::CHistogramFrame* const hframe) {
	Pimpl()->AttachFrame(hframe);
}

bool DisplayController::HistogramAlreadyAttached(const uint32_t& _area) const {
	return Pimpl()->HistogramAlreadyAttached(_area);
}

void DisplayController::DetachFrame(gui::CHistogramFrame* const hframe) {
	Pimpl()->DetachFrame(hframe);
}

parameters::WindowCollection DisplayController::GetWindowCollection() const {
	return Pimpl()->GetWindowCollection();
}

}