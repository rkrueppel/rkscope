#include "stdafx.h"
#include "OutputsDAQmx.h"
#include "parameters/Outputs.h"
#include "parameters/Scope.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeException.h"

namespace scope  {

OutputsDAQmx::OutputsDAQmx(const uint32_t& _area, const parameters::OutputsDAQmx& _outputparams, const parameters::Scope& _params)
	: Outputs(_area) {

	int32_t samplingtype = (_params.requested_mode()==DaqModeHelper::continuous)?DAQmx_Val_ContSamps:DAQmx_Val_FiniteSamps;
	
	std::wstring commontrig = _params.commontrigger();

	task.CreateTask();
	task.CreateAOVoltageChannel(_outputparams.channelsstring()
		, L"XYZPOut"
		, -_outputparams.range()
		, _outputparams.range());

	// Calculate pixelrate and number of pixels to generate
	double pixelrate = 1/(_params.areas[area]->daq.pixeltime()*1E-6);
	int32_t pixelsperchan = _params.areas[area]->Currentframe().TotalPixels();


	if ( _params.requested_mode() == DaqModeHelper::nframes )
		//DBOUT(L" _params.requested_mode() == DaqModeHelper::nframes");
			pixelsperchan = _params.areas[area]->Currentframe().TotalPixels() * _params.areas[area]->daq.requested_frames() * _params.areas[ThisAreaOrMasterArea(area)]->daq.averages();

	// Configure timing (if using ReferenceClock timing ClockString gives "")
	task.ConfigureSampleTiming(DAQmx::ClockString(_outputparams.daq_timing(), _outputparams.externalclocksource())
		, pixelrate
		, pixelsperchan
		, samplingtype);
	if ( DaqTimingHelper::Mode::ReferenceClock ==_outputparams.daq_timing() )
		//DBOUT(L"DaqTimingHelper::Mode::ReferenceClock ==_outputparams.daq_timing()");
		task.ConfigureReferenceClock(_outputparams.referenceclocksource(), _outputparams.referenceclockrate());

	task.ConfigureBuffer( _params.areas[area]->Currentframe().TotalPixels());

	// Regenerate frame samples if we are in nframes mode
	if ( _params.requested_mode() == DaqModeHelper::nframes )
		//DBOUT(L"_params.requested_mode() == DaqModeHelper::nframes");
		task.SetRegeneration(true);

	// Export the pixel/sample clock
	task.ExportSignal(DAQmx_Val_SampleClock, _outputparams.exportpixelclockterminal());

	// All (exept the first aouttask) wait for the first tasks start trigger
	if ( area != 0 ) {
		//DBOUT(L"area != 0 ");
		task.ConfigureDigStartTrigger(commontrig, DAQmx_Val_Rising);
	}
	// If T-Series triggered first output task waits for external trigger (and all other tasks waiting for first output task)
	else if ( _params.timeseries.triggered() )
		//DBOUT(L"_params.timeseries.triggered()");
		task.ConfigureDigStartTrigger(_params.timeseries.triggerchannel(), DAQmx_Val_Rising);
}

OutputsDAQmx::~OutputsDAQmx() {
	Stop();
	task.Clear();
}

void OutputsDAQmx::Start() {
	task.Start();
}

void OutputsDAQmx::Stop() {
	writeabort = true;
	task.Stop();
}

int32_t OutputsDAQmx::Write(std::vector<int16_t>& _xyzp, const uint32_t& _blocks) {
	int32_t written = 0;
	assert(_blocks!=0);
	int32_t sizeperchannel = static_cast<int32_t>(_xyzp.size() / 4);			// number of samples in each output channel
	int32_t blocksize = sizeperchannel/_blocks;
	int32_t lastblocksize = blocksize + (sizeperchannel - blocksize*_blocks);	// adjust size of last block if sizeperchannel is not a multiple of blocksize
	DBOUT(L"OutputsDAQmx::Write blocksize " << blocksize << L" last block " << lastblocksize);
	DBOUT(L"OutputsDAQmx::Write blocks " << _blocks << L" sizeperchannel " << sizeperchannel << L" totblocksize " << (_blocks-1)*blocksize+lastblocksize);
	try {
		for ( uint32_t b = 0 ; b < _blocks ; b++ ) {
			if ( writeabort == true ) {
				DBOUT(L"OutputsDAQmx::Write writeabort");
				break;
			}
			DBOUT(L"OutputsDAQmx::Write block " << b);
			task.SetWriteOffset(b*blocksize);						// set write offset in the device buffer
			written += task.WriteAnalogI16((_xyzp.data()+4*b*blocksize)	// set offset in the data vector
				, ((b==_blocks-1)?lastblocksize:blocksize), false, 2, DAQmx_Val_GroupByScanNumber);								// data layout is by sample
		}
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	writeabort = false;
	return written;
}

ZeroOutputsDAQmx::ZeroOutputsDAQmx(const parameters::OutputsDAQmx& _params) {
	DAQmx::CDAQmxAnalogOutTask task;
	try {
		task.CreateTask();
		task.CreateAOVoltageChannel(_params.channelsstring()
			, L"XYZPOut"
			, -_params.range()
			, _params.range());
		task.WriteAnalogI16(std::make_shared<std::vector<int16>>(4,(int16)0)->data(), 1, true);
		task.WaitUntilDone(500);
		task.Clear();
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

}