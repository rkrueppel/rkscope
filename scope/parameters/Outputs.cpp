#include "stdafx.h"
#include "Outputs.h"
#include "devices/daqmx/DAQmxTask.h"
#include "helpers/ScopeException.h"

namespace scope {

namespace parameters {

// Save some typing here...
using namespace boost::property_tree;

Outputs::Outputs(void)
	: range(5, 0, 10, L"OutputRange_V")
	, minoutputscanner(-1, -10, 10, L"MinOutputScanner_V")
	, maxoutputscanner(1, -10, 10, L"MaxOutputScanner_V")
	, minoutputpockels(0.0, 0.0, 2.0, L"MinOutputPockels_V")
	, maxoutputpockels(2.0, 0.0, 2.0, L"MaxOutputPockels_V"){

}

void Outputs::Load(const wptree& _pt) {
	range.SetFromPropertyTree(_pt);
	minoutputscanner.SetFromPropertyTree(_pt);
	maxoutputscanner.SetFromPropertyTree(_pt);
	minoutputpockels.SetFromPropertyTree(_pt);
	maxoutputpockels.SetFromPropertyTree(_pt);
}

void Outputs::Save(wptree& _pt) const {
	range.AddToPropertyTree(_pt);
	minoutputscanner.AddToPropertyTree(_pt);
	maxoutputscanner.AddToPropertyTree(_pt);
	minoutputpockels.AddToPropertyTree(_pt);
	maxoutputpockels.AddToPropertyTree(_pt);
}

void Outputs::SetReadOnlyWhileScanning(const RunState& _runstate) {
	// too lazy to do it...
}

std::unique_ptr<Outputs> Outputs::Factory(const OutputsType& _type) {
	switch ( _type.t ) {
	case OutputsTypeHelper::OutputsDAQmx:
		return OutputsDAQmx::Create();
	case OutputsTypeHelper::OutputsDAQmxLineClock:
		return OutputsDAQmxLineClock::Create();
	case OutputsTypeHelper::OutputsDAQmxSlave:
		return OutputsDAQmxSlave::Create();
	case OutputsTypeHelper::OutputsDAQmxResonance:
		return OutputsDAQmxResonance::Create();
	default:
		return OutputsDAQmx::Create();
	}
}

OutputsDAQmx::OutputsDAQmx()
	: channelsstring(L"PXI-6259_0/ao0:3", L"ChannelsString")
	, daq_timing(DaqTimingHelper::ReferenceClock, L"DAQTimingMethod")
	, minimumpixeltime(0.8, 0.01, 10.0, L"MinimumPixelTime_us")
	, referenceclocksource(L"PXI_Clk10", L"ReferenceClockSource")
	, referenceclockrate(10000000, 1000000, 100000000, L"ReferenceClockRate_Hz")
	, externalclocksource(L"/PXI-6259_0/PXI_Trig1", L"ExternalClockSource")
	, exportpixelclockterminal(L"/PXI-6259_0/PFI12", L"ExportPixelClockTerminal") {
}

double OutputsDAQmx::CoercedPixeltime(const double& _pixeltime) const {
	return std::max(MinimumPixeltime(), 1/DAQmx::PredictSampleRate(1/_pixeltime, 4, referenceclockrate(), DAQmx_Val_AO));
}

double OutputsDAQmx::MinimumPixeltime() const {
	return minimumpixeltime();
}

void OutputsDAQmx::Load(const wptree& _pt) {
	Outputs::Load(_pt);
	channelsstring.SetFromPropertyTree(_pt);
	daq_timing.SetFromPropertyTree(_pt);
	minimumpixeltime.SetFromPropertyTree(_pt);
	referenceclocksource.SetFromPropertyTree(_pt);
	referenceclockrate.SetFromPropertyTree(_pt);
	externalclocksource.SetFromPropertyTree(_pt);
	exportpixelclockterminal.SetFromPropertyTree(_pt);
}

void OutputsDAQmx::Save(wptree& _pt) const {
	Outputs::Save(_pt);
	channelsstring.AddToPropertyTree(_pt);
	daq_timing.AddToPropertyTree(_pt);
	minimumpixeltime.AddToPropertyTree(_pt);;
	referenceclocksource.AddToPropertyTree(_pt);
	referenceclockrate.AddToPropertyTree(_pt);
	externalclocksource.AddToPropertyTree(_pt);
	exportpixelclockterminal.AddToPropertyTree(_pt);
}

void OutputsDAQmx::SetReadOnlyWhileScanning(const RunState& _runstate) {
	Outputs::SetReadOnlyWhileScanning(_runstate);
}

OutputsDAQmxLineClock::OutputsDAQmxLineClock()
	: xpout(L"PXI-6259_0/ao0:1", L"XPOutString")
	, yzout(L"PXI-6259_1/ao0:1", L"YZOutString")
	, pixel_timing(DaqTimingHelper::ReferenceClock, L"PixelTimingMethod")
	, line_timing(DaqTimingHelper::ReferenceClock, L"LineTimingMethod")
	, minimumpixeltime(0.8, 0.01, 10.0, L"MinimumPixelTime_us")
	, pixel_referenceclocksource(L"PXI_Clk10", L"PixelReferenceClockSource")
	, line_referenceclocksource(L"PXI_Clk10", L"LineReferenceClockSource")
	, pixel_referenceclockrate(10000000, 1000000, 100000000, L"ReferenceClockRate_Hz")
	, line_referenceclockrate(10000000, 1000000, 100000000, L"ReferenceClockRate_Hz")
	, pixel_externalclocksource(L"/PXI-6259_0/PXI_Trig1", L"ExternalPixelClockString")
	, line_externalclocksource(L"/PXI-6259_1/PXI_Trig2", L"ExternalLineClockString") {
}

double OutputsDAQmxLineClock::CoercedPixeltime(const double& _pixeltime) const {
	return std::max(MinimumPixeltime(), 1/DAQmx::PredictSampleRate(1/_pixeltime, 2, pixel_referenceclockrate(), DAQmx_Val_AO));
}

double OutputsDAQmxLineClock::MinimumPixeltime() const {
	return minimumpixeltime();
}

void OutputsDAQmxLineClock::Load(const wptree& _pt) {
	Outputs::Load(_pt);
	xpout.SetFromPropertyTree(_pt);
	yzout.SetFromPropertyTree(_pt);
	pixel_timing.SetFromPropertyTree(_pt);
	line_timing.SetFromPropertyTree(_pt);
	minimumpixeltime.SetFromPropertyTree(_pt);
	pixel_referenceclocksource.SetFromPropertyTree(_pt);
	line_referenceclocksource.SetFromPropertyTree(_pt);
	pixel_referenceclockrate.SetFromPropertyTree(_pt);
	line_referenceclockrate.SetFromPropertyTree(_pt);
	pixel_externalclocksource.SetFromPropertyTree(_pt);
	line_externalclocksource.SetFromPropertyTree(_pt);
}

void OutputsDAQmxLineClock::Save(wptree& _pt) const {
	Outputs::Save(_pt);
	xpout.AddToPropertyTree(_pt);
	yzout.AddToPropertyTree(_pt);
	pixel_timing.AddToPropertyTree(_pt);
	line_timing.AddToPropertyTree(_pt);
	minimumpixeltime.AddToPropertyTree(_pt);
	pixel_referenceclocksource.AddToPropertyTree(_pt);
	line_referenceclocksource.AddToPropertyTree(_pt);
	pixel_referenceclockrate.AddToPropertyTree(_pt);
	line_referenceclockrate.AddToPropertyTree(_pt);
	pixel_externalclocksource.AddToPropertyTree(_pt);
	line_externalclocksource.AddToPropertyTree(_pt);;
}

void OutputsDAQmxLineClock::SetReadOnlyWhileScanning(const RunState& _runstate) {
	Outputs::SetReadOnlyWhileScanning(_runstate);
}


OutputsDAQmxSlave::OutputsDAQmxSlave()
	: zpout(L"PXI-6259_1/ao0:1", L"ZPOutString")
	, timing(DaqTimingHelper::ReferenceClock, L"DAQTimingMethod")
	, minimumpixeltime(0.8, 0.01, 10.0, L"MinimumPixelTime_us")
	, referenceclocksource(L"PXI_Clk10", L"ReferenceClockSource")
	, referenceclockrate(10000000, 1000000, 100000000, L"ReferenceClockRate_Hz")
	, externalclocksource(L"/PXI-6259_0/PXI_Trig1", L"ExternalClockSource") {

}

double OutputsDAQmxSlave::CoercedPixeltime(const double& _pixeltime) const {
	return std::max(MinimumPixeltime(), 1/DAQmx::PredictSampleRate(1/_pixeltime, 2, referenceclockrate(), DAQmx_Val_AO));
}

double OutputsDAQmxSlave::MinimumPixeltime() const {
	return minimumpixeltime();
}

void OutputsDAQmxSlave::Load(const wptree& _pt) {
	Outputs::Load(_pt);
	zpout.SetFromPropertyTree(_pt);
	timing.SetFromPropertyTree(_pt);
	minimumpixeltime.SetFromPropertyTree(_pt);
	referenceclocksource.SetFromPropertyTree(_pt);
	referenceclockrate.SetFromPropertyTree(_pt);
	externalclocksource.SetFromPropertyTree(_pt);
}

void OutputsDAQmxSlave::Save(wptree& _pt) const {
	Outputs::Save(_pt);
	zpout.AddToPropertyTree(_pt);
	timing.AddToPropertyTree(_pt);
	minimumpixeltime.AddToPropertyTree(_pt);
	referenceclocksource.AddToPropertyTree(_pt);
	referenceclockrate.AddToPropertyTree(_pt);
	externalclocksource.AddToPropertyTree(_pt);
}

void OutputsDAQmxSlave::SetReadOnlyWhileScanning(const RunState& _runstate) {
	Outputs::SetReadOnlyWhileScanning(_runstate);
}


OutputsDAQmxResonance::OutputsDAQmxResonance()
	: channelsstring(L"PXI-6259_0/ao0:1", L"ChannelsString")
	, zoomchannelstring(L"PXI-6259_0/port0/line1:2", L"ZoomChannelsString")
	, daq_timing(DaqTimingHelper::ReferenceClock, DaqTimingHelper::OnboardClock, DaqTimingHelper::External, L"DAQTimingMethod")
	, minimumpixeltime(0.02, 0.02, 10.0, L"MinimumPixelTime_us")
	, referenceclocksource(L"PXI_Clk10", L"ReferenceClockSource")
	, referenceclockrate(10000000, 1000000, 100000000, L"ReferenceClockRate_Hz")
	, externalclocksource(L"/PXI-6259_0/PFI1", L"ExternalClockSource")
	, exportpixelclockterminal(L"/PXI-6259_0/PFI12", L"ExportPixelClockTerminal") {
}

double OutputsDAQmxResonance::CoercedPixeltime(const double& _pixeltime) const {
	return std::max(MinimumPixeltime(), 1/DAQmx::PredictSampleRate(1/_pixeltime, 4, referenceclockrate(), DAQmx_Val_AO));
}

double OutputsDAQmxResonance::MinimumPixeltime() const {
	return minimumpixeltime();
}

void OutputsDAQmxResonance::Load(const wptree& _pt) {
	Outputs::Load(_pt);
	channelsstring.SetFromPropertyTree(_pt);
	zoomchannelstring.SetFromPropertyTree(_pt);
	daq_timing.SetFromPropertyTree(_pt);
	minimumpixeltime.SetFromPropertyTree(_pt);
	referenceclocksource.SetFromPropertyTree(_pt);
	referenceclockrate.SetFromPropertyTree(_pt);
	externalclocksource.SetFromPropertyTree(_pt);
	exportpixelclockterminal.SetFromPropertyTree(_pt);
}

void OutputsDAQmxResonance::Save(wptree& _pt) const {
	Outputs::Save(_pt);
	channelsstring.AddToPropertyTree(_pt);
	zoomchannelstring.AddToPropertyTree(_pt);
	daq_timing.AddToPropertyTree(_pt);
	minimumpixeltime.AddToPropertyTree(_pt);;
	referenceclocksource.AddToPropertyTree(_pt);
	referenceclockrate.AddToPropertyTree(_pt);
	externalclocksource.AddToPropertyTree(_pt);
	exportpixelclockterminal.AddToPropertyTree(_pt);
}

void OutputsDAQmxResonance::SetReadOnlyWhileScanning(const RunState& _runstate) {
	Outputs::SetReadOnlyWhileScanning(_runstate);
}


OutputsDAQmxResonanceSlave::OutputsDAQmxResonanceSlave()
	: OutputsDAQmxSlave() {

}

void OutputsDAQmxResonanceSlave::Load(const wptree& _pt) {
	OutputsDAQmxSlave::Load(_pt);
}

void OutputsDAQmxResonanceSlave::Save(wptree& _pt) const {
	OutputsDAQmxSlave::Save(_pt);
}

void OutputsDAQmxResonanceSlave::SetReadOnlyWhileScanning(const RunState& _runstate) {
	OutputsDAQmxSlave::SetReadOnlyWhileScanning(_runstate);
}



}

}