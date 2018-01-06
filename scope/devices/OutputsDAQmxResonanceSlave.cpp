#include "stdafx.h"
#include "OutputsDAQmxResonanceSlave.h"
#include "parameters/Outputs.h"
#include "parameters/Scope.h"

namespace scope {

OutputsDAQmxResonanceSlave::OutputsDAQmxResonanceSlave(const uint32_t& _area, const parameters::OutputsDAQmxResonanceSlave& _outputparams, const parameters::Scope& _params)
	: Outputs(_area) {

	if ( _area == 0 )
		throw ScopeException("The first area must not be a slave area! Reconfigure!!!!");

	int32_t samplingtype = (_params.requested_mode()==DaqModeHelper::continuous)?DAQmx_Val_ContSamps:DAQmx_Val_FiniteSamps;
	//size_t num_planes = (_params.areas[area]->FrameResonance().planes.size())?_params.areas[area]->FrameResonance().planes.size():1;
	parameters::ScannerVectorFrameResonance* svecres = nullptr;
	_params.allareas[area]->GetFrame(svecres);
	size_t num_planes = (svecres->planes.size()) ? svecres->planes.size() : 1;

	std::wstring commontrig = _params.commontrigger();

	zpout_task.CreateTask();
	zpout_task.CreateAOVoltageChannel(_outputparams.zpout()
		, L"ZPOut"
		, -_outputparams.range()
		, _outputparams.range());

	// Calculate pixelrate and number of pixels to generate
	assert(_params.allareas[area]->areatype() == AreaTypeHelper::Slave);
	double pixelrate = 1/(dynamic_cast<parameters::SlaveArea*>(_params.allareas[area].get())->masterarea->daq.pixeltime()*1E-6); // Pixelrate/Pixeltime to be the same as Master area
	
	// Slave outputs the whole thing (as opposed to a non resonance slave)
	int32_t pixelsperchan = svecres->TotalPixels();
	
	if ( _params.requested_mode() == DaqModeHelper::nframes )
		pixelsperchan = svecres->TotalPixels() * _params.allareas[area]->daq.requested_frames();

	// Configure timing (if using ReferenceClock timing ClockString gives "")
	zpout_task.ConfigureSampleTiming(DAQmx::ClockString(_outputparams.timing(), _outputparams.externalclocksource())
		, pixelrate
		, pixelsperchan
		, samplingtype);
	if ( DaqTimingHelper::Mode::ReferenceClock ==_outputparams.timing() )
		zpout_task.ConfigureReferenceClock(_outputparams.referenceclocksource(), _outputparams.referenceclockrate());

	zpout_task.ConfigureBuffer(num_planes* svecres->YTotalLines()/2);

	// Regenerate frame samples if we are in nframes mode
	if ( _params.requested_mode() == DaqModeHelper::nframes )
		zpout_task.SetRegeneration(true);

	// All (exept the first aouttask) wait for the first tasks start trigger
	if ( area != 0 ) {
		zpout_task.ConfigureDigStartTrigger(commontrig, DAQmx_Val_Rising);
	}
}

OutputsDAQmxResonanceSlave::~OutputsDAQmxResonanceSlave(void) {
	Stop();
	zpout_task.Clear();
}

void OutputsDAQmxResonanceSlave::Start() {
	zpout_task.Start();
}

void OutputsDAQmxResonanceSlave::Stop() {
	writeabort = true;
	zpout_task.Stop();
}

int32_t OutputsDAQmxResonanceSlave::Write(std::vector<int16_t>& _zp, const uint32_t& _blocks) {
	int32_t written = 0;
	assert(_blocks!=0);
	int32_t sizeperchannel = static_cast<int32_t>(_zp.size() / 2);			// number of samples in each output channel
	int32_t blocksize = sizeperchannel/_blocks;
	int32_t lastblocksize = blocksize + (sizeperchannel - blocksize*_blocks);	// adjust size of last block if sizeperchannel is not a multiple of blocksize
	DBOUT(L"OutputsDAQmxResonanceSlave::Write blocksize " << blocksize << L" last block " << lastblocksize);
	DBOUT(L"OutputsDAQmxResonanceSlave::Write blocks " << _blocks << L" sizeperchannel " << sizeperchannel << L" totblocksize " << (_blocks-1)*blocksize+lastblocksize);

	try {
		for ( uint32_t b = 0 ; b < _blocks ; b++ ) {
			if ( writeabort == true ) {
				DBOUT(L"OutputsDAQmxResonanceSlave::Write writeabort");
				break;
			}
			DBOUT(L"OutputsDAQmxResonanceSlave::Write block " << b);
			zpout_task.SetWriteOffset(b*blocksize);						// set write offset in the device buffer
			written += zpout_task.WriteAnalogI16((_zp.data()+2*b*blocksize)	// set offset in the data vector
				, ((b==_blocks-1)?lastblocksize:blocksize), false, 2, DAQmx_Val_GroupByScanNumber);								// data layout is by sample
		}
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	writeabort = false;

	return written;
}

ZeroOutputsDAQmxResonanceSlave::ZeroOutputsDAQmxResonanceSlave(const parameters::OutputsDAQmxResonanceSlave& _params) {
	DAQmx::CDAQmxAnalogOutTask task;
	try {
		task.CreateTask();
		task.CreateAOVoltageChannel(_params.zpout()
			, L"XYZPOut"
			, -_params.range()
			, _params.range());
		task.WriteAnalogI16(std::make_shared<std::vector<int16>>(2,(int16)0)->data(), 1, true);
		task.WaitUntilDone(500);
		task.Clear();
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}


}
