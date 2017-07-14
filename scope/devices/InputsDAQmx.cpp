#include "stdafx.h"
#include "InputsDAQmx.h"
#include "parameters/Inputs.h"
#include "parameters/Scope.h"
#include "ScopeDatatypes.h"
#include "helpers/DaqChunk.h"
#include "helpers/ScopeException.h"

namespace scope {

InputsDAQmx::InputsDAQmx(const uint32_t& _area, const parameters::InputsDAQmx* const _inputparams, const parameters::Scope& _params)
	: Inputs(_area) {
	int32_t samplingtype = (_params.requested_mode() == DaqModeHelper::continuous)?DAQmx_Val_ContSamps:DAQmx_Val_FiniteSamps;
	
	std::wstring commontrig = _params.commontrigger();

	task.CreateTask();
	task.CreateAIVoltageChannel(_inputparams->channelsstring()
		, L"Acquisition"
		, DAQmx_Val_Cfg_Default
		, -(_inputparams->range())
		, (_inputparams->range()));

	// Calculate pixelrate/samplerate if oversampling and number of pixels/samples to acquire
	// Note: Coercion to pixelrates supported by the device is already done in parameters::InputsDAQmx
	double pixelrate = 1/(_params.areas[area]->daq.pixeltime()*1E-6);
	double inputrate = pixelrate;
	if ( _inputparams->oversampling() )
		inputrate = 1/(_inputparams->MinimumPixeltime()*1E-6);

	uint32_t oversampling = round2ui32(inputrate/pixelrate);

	uint32_t pixelsperchan = _params.areas[area]->Currentframe().TotalPixels();

	if ( _params.requested_mode() == DaqModeHelper::nframes)
		pixelsperchan *= _params.areas[area]->daq.requested_frames() * _params.areas[ThisAreaOrMasterArea(area)]->daq.averages();

	requested_samples = oversampling * pixelsperchan;

	// Configure timing
	task.ConfigureSampleTiming(DAQmx::ClockString(_inputparams->daq_timing(), _inputparams->externalclocksource())
		, inputrate
		, static_cast<int32_t>(requested_samples)
		, samplingtype);
	if ( DaqTimingHelper::Mode::ReferenceClock == _inputparams->daq_timing() )
		task.ConfigureReferenceClock(_inputparams->referenceclocksource, _inputparams->referenceclockrate());

	task.ConfigureDigStartTrigger(commontrig, DAQmx_Val_Rising);

	// Size of data chunks to readm should be between 64^2 and 128^2
	standardchunksize = oversampling * std::max(64u*64u, std::min(_params.areas[area]->Currentframe().TotalPixels() >> 2, 128u*128u));
}

InputsDAQmx::~InputsDAQmx() {
	task.Stop();
	task.Clear();
}

void InputsDAQmx::Start() {
	task.Start();
}

void InputsDAQmx::Stop() {
	task.Stop();
}

uint32_t InputsDAQmx::StandardChunkSize() const {
	return standardchunksize;
}

int32_t InputsDAQmx::Read(DaqChunk& _chunk, bool& _timedout, const double& _timeout) {
	int32_t read = 0;
	bool timedout = false;
	try {
		read = task.ReadU16(_chunk.data, _chunk.PerChannel(), _chunk.NChannels(), timedout, _timeout);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	return read;
}

}