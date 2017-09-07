#include "stdafx.h"
#include "OutputsDAQmxSlave.h"
#include "parameters/Outputs.h"
#include "parameters/Scope.h"

namespace scope {

OutputsDAQmxSlave::OutputsDAQmxSlave(const uint32_t& _area, const parameters::OutputsDAQmxSlave& _outputparams, const parameters::Scope& _params)
	: Outputs(_area) {

	if ( _area == 0 )
		throw ScopeException("The first area must not be a slave area! Reconfigure!!!!");

	int32_t samplingtype = (_params.requested_mode()==DaqModeHelper::continuous)?DAQmx_Val_ContSamps:DAQmx_Val_FiniteSamps;

	std::wstring commontrig = _params.commontrigger();

	zpout_task.CreateTask();
	zpout_task.CreateAOVoltageChannel(_outputparams.zpout()
		, L"ZPOut"
		, -_outputparams.range()
		, _outputparams.range());

	// Calculate pixelrate and number of pixels to generate
	double pixelrate = 1/(_params.areas[ThisAreaOrMasterArea(area)].daq.pixeltime()*1E-6); // Pixelrate/Pixeltime to be the same as Master area
	
	// Slave outputs only one line (repeated)
	int32_t pixelsperchan = _params.areas[area].Currentframe().XTotalPixels();

	if ( _params.requested_mode() == DaqModeHelper::nframes )
		pixelsperchan = _params.areas[area].Currentframe().TotalPixels() * _params.areas[area].daq.requested_frames() * _params.areas[area].daq.averages();

	// Configure timing (if using ReferenceClock timing ClockString gives "")
	zpout_task.ConfigureSampleTiming(DAQmx::ClockString(_outputparams.timing(), _outputparams.externalclocksource())
		, pixelrate
		, pixelsperchan
		, samplingtype);
	if ( DaqTimingHelper::Mode::ReferenceClock ==_outputparams.timing() )
		zpout_task.ConfigureReferenceClock(_outputparams.referenceclocksource(), _outputparams.referenceclockrate());

	zpout_task.ConfigureBuffer(_params.areas[area].Currentframe().XTotalPixels());

	// Regenerate frame samples if we are in nframes mode
	if ( _params.requested_mode() == DaqModeHelper::nframes )
		zpout_task.SetRegeneration(true);

	// All (exept the first aouttask) wait for the first tasks start trigger
	if ( area != 0 ) {
		zpout_task.ConfigureDigStartTrigger(commontrig, DAQmx_Val_Rising);
	}
}

OutputsDAQmxSlave::~OutputsDAQmxSlave(void) {
	Stop();
	zpout_task.Clear();
}

void OutputsDAQmxSlave::Start() {
	zpout_task.Start();
}

void OutputsDAQmxSlave::Stop() {
	writeabort = true;
	zpout_task.Stop();
}

int32_t OutputsDAQmxSlave::Write(std::vector<int16_t>& _zp, const uint32_t& _blocks) {
	int32_t written = 0;
	assert(_blocks!=0);
	int32_t sizeperchannel = static_cast<int32_t>(_zp.size() / 2);			// number of samples in each output channel
	int32_t blocksize = sizeperchannel/_blocks;
	int32_t lastblocksize = blocksize + (sizeperchannel - blocksize*_blocks);	// adjust size of last block if sizeperchannel is not a multiple of blocksize
	DBOUT(L"OutputsDAQmxSlave::Write blocksize " << blocksize << L" last block " << lastblocksize);
	DBOUT(L"OutputsDAQmxSlave::Write blocks " << _blocks << L" sizeperchannel " << sizeperchannel << L" totblocksize " << (_blocks-1)*blocksize+lastblocksize);

	try {
		for ( uint32_t b = 0 ; b < _blocks ; b++ ) {
			if ( writeabort == true ) {
				DBOUT(L"OutputsDAQmxSlave::Write writeabort");
				break;
			}
			DBOUT(L"OutputsDAQmxSlave::Write block " << b);
			zpout_task.SetWriteOffset(b*blocksize);						// set write offset in the device buffer
			written += zpout_task.WriteAnalogI16((_zp.data()+2*b*blocksize)	// set offset in the data vector
				, ((b==_blocks-1)?lastblocksize:blocksize), false, 2, DAQmx_Val_GroupByScanNumber);								// data layout is by sample
		}
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	writeabort = false;

	return written;
}

ZeroOutputsDAQmxSlave::ZeroOutputsDAQmxSlave(const parameters::OutputsDAQmxSlave& _params) {
	DAQmx::CDAQmxAnalogOutTask task;
	try {
		task.CreateTask();
		task.CreateAOVoltageChannel(_params.zpout()
			, L"XYZPOut"
			, -_params.range()
			, _params.range());
		task.WriteAnalogI16(std::make_shared<std::vector<int16_t>>(2,(int16)0)->data(), 1, true);
		task.WaitUntilDone(500);
		task.Clear();
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}


}
