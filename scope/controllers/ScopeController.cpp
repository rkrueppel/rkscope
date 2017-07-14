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

void ScopeController::PrepareQuit() {
	Pimpl()->PrepareQuit();
}

bool ScopeController::LoadParameters(const std::wstring& filepath) {
	return Pimpl()->LoadParameters(filepath);
}

bool ScopeController::SaveParameters(const std::wstring& filepath) {
	return Pimpl()->SaveParameters(filepath);
}

void ScopeController::ZeroGalvoOutputs() {
	Pimpl()->ZeroGalvoOutputs();
}

void ScopeController::OpenCloseShutter(const uint32_t& _area, const bool& _open) {
	Pimpl()->OpenCloseShutter(_area, _open);
}

bool ScopeController::GetShutterState(const uint32_t& _area) const {
	return Pimpl()->GetShutterState(_area);
}

void ScopeController::TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
	Pimpl()->TurnOnOffSwitchResonance(_area, _on);
}

bool ScopeController::GetSwitchResonanceState(const uint32_t& _area) const {
	return Pimpl()->GetSwitchResonanceState(_area);
}

void ScopeController::Version() {
	Pimpl()->Version();
}

std::wstring ScopeController::CurrentConfigFile() const {
	return Pimpl()->CurrentConfigFile();
}

void ScopeController::AttachFrame(gui::CChannelFrame* const cframe) {
	Pimpl()->AttachFrame(cframe);
}

void ScopeController::DetachFrame(gui::CChannelFrame* const cframe) {
	Pimpl()->DetachFrame(cframe);
}

void ScopeController::AttachFrame(gui::CHistogramFrame* const hframe) {
	Pimpl()->AttachFrame(hframe);
}

bool ScopeController::HistogramAlreadyAttached(const uint32_t& _area) {
	return Pimpl()->HistogramAlreadyAttached(_area);
}

void ScopeController::DetachFrame(gui::CHistogramFrame* const hframe) {
	Pimpl()->DetachFrame(hframe);
}

void ScopeController::SaveCurrentWindowPositions() {
	Pimpl()->SaveCurrentWindowPositions();
}

void ScopeController::SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper) {
	Pimpl()->SetHistogramLimits(_area, _channel, _lower, _upper);
}

void ScopeController::UpdateResonancePlanes(const uint32_t& _area) {
	Pimpl()->UpdateAreaParametersFromGui(_area);
}

void ScopeController::RegisterScanmodeCallback(std::function<void(const uint32_t&, const ScannerVectorType&)> _callback) {
	Pimpl()->RegisterScanmodeCallback(_callback);
}


}