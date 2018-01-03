#include "stdafx.h"
#include "OutputsDAQmxLineClock.h"
#include "parameters/Outputs.h"
#include "parameters/Scope.h"
#include "helpers/ScopeException.h"

namespace scope {

OutputsDAQmxLineClock::OutputsDAQmxLineClock(const uint32_t& _area, const parameters::OutputsDAQmxLineClock& _outputparams, const parameters::Scope& _params)
	: Outputs(_area)
	, XTotalPixels(_params.masterareas[area].Currentframe().XTotalPixels())
	, YTotalLines(_params.masterareas[area].Currentframe().YTotalLines()) {

	std::wstring commontrig = _params.commontrigger();

	xpout_task.CreateTask();
	xpout_task.CreateAOVoltageChannel(_outputparams.xpout(), L"XPOutput", -_outputparams.range(), _outputparams.range());

	double pixelrate = 1/(_params.masterareas[area].daq.pixeltime()*1E-6);
	std::wstring xpclockstring = DAQmx::ClockString(_outputparams.pixel_timing(), _outputparams.pixel_externalclocksource());

	xpout_task.ConfigureSampleTiming(xpclockstring, pixelrate, XTotalPixels, DAQmx_Val_ContSamps, DAQmx_Val_Rising);

	if ( DaqTimingHelper::Mode::ReferenceClock ==_outputparams.pixel_timing() )
		xpout_task.ConfigureReferenceClock(_outputparams.pixel_referenceclocksource(), _outputparams.pixel_referenceclockrate());

	xpout_task.ConfigureBuffer(XTotalPixels);

	// Force to regenerate samples if we are in nframes mode
	if ( _params.requested_mode() == DaqModeHelper::nframes )
		xpout_task.SetRegeneration(true);

	// all tasks (except first area's xpout_task) wait for that first xpout_task's start trigger
	if ( area != 0 ) {
		xpout_task.ConfigureDigStartTrigger(commontrig, DAQmx_Val_Rising);
	}
	// If T-Series is triggered, first xpout_task task waits for external trigger (and all other tasks waiting for that first xpout_task)
	else if ( _params.timeseries.triggered() )
		xpout_task.ConfigureDigStartTrigger(_params.timeseries.triggerchannel(), DAQmx_Val_Rising);

	yzout_task.CreateTask();
	yzout_task.CreateAOVoltageChannel(_outputparams.yzout(), L"YZPOutput", -_outputparams.range(), _outputparams.range());

	std::wstring yzclockstring = DAQmx::ClockString(_outputparams.line_timing(), _outputparams.line_externalclocksource());

	yzout_task.ConfigureSampleTiming(yzclockstring, _params.masterareas[area].linerate(), YTotalLines, DAQmx_Val_ContSamps, DAQmx_Val_Rising);

	if ( DaqTimingHelper::Mode::ReferenceClock ==_outputparams.line_timing() )
		yzout_task.ConfigureReferenceClock(_outputparams.line_referenceclocksource(), _outputparams.line_referenceclockrate());

	yzout_task.ConfigureBuffer(YTotalLines);

	// The onboard buffer of e.g. the NI-6259 is 4096 samples, with ~512 samples in a column it would take several frames for updated values to be actually generated
	// To force updating the onboard buffer we set this data transfer condition (actually why THIS works and not DAQmx_Val_OnBrdMemNotFull I do not fully get...)
	yzout_task.SetDataTransferCondition(_outputparams.yzout(), DAQmx_Val_OnBrdMemEmpty);

	// Force to regenerate samples if we are in nframes mode
	if ( _params.requested_mode() == DaqModeHelper::nframes )
		yzout_task.SetRegeneration(true);

	// all tasks (except first area's xpout_task) wait for that first xpout_task's start trigger
	yzout_task.ConfigureDigStartTrigger(commontrig, DAQmx_Val_Rising);
}

OutputsDAQmxLineClock::~OutputsDAQmxLineClock(void) {
	Stop();
	xpout_task.Clear();
	yzout_task.Clear();
}

void OutputsDAQmxLineClock::Start(void) {
	// Start yzout first, since it waits on xpout's ao/StartTrigger as master trigger
	yzout_task.Start();
	xpout_task.Start();
}

void OutputsDAQmxLineClock::Stop(void) {
	xpout_task.Stop();
	yzout_task.Stop();
}

int32_t OutputsDAQmxLineClock::Write(std::vector<int16_t>& _xyzp, const uint32_t& _blocks) {
	int32_t written = 0;

	int32_t yblocksize = YTotalLines/_blocks;
	int32_t lastyblocksize = yblocksize + (YTotalLines - yblocksize*_blocks);	// adjust size of last block if size per channel is not a multiple of blocksize
	
	try {
		if ( writeabort == true ) {
			DBOUT(L"OutputsDAQmxLineClock::Write writeabort");
			writeabort = false;
			return 0;
		}
		// First write xp as one block, this is very fast since xp (one line) output is also very fast
		written += xpout_task.WriteAnalogI16(_xyzp.data(), XTotalPixels, false, 2, DAQmx_Val_GroupByScanNumber);

		// Then write yz in blocks, since yz is very slow and we want to check in between for a writeabort and do not want to set high timeout times for WriteAnalogI16
		// Since one column is often smaller than the device buffer, use a different transfer condition (see constructor)
		for ( uint32_t b = 0 ; b < _blocks ; b++ ) {
			if ( writeabort == true ) {
				DBOUT(L"OutputsDAQmxLineClock::Write writeabort");
				break;
			}
			yzout_task.SetWriteOffset(b*yblocksize);						// set write offset in the device buffer
			// data layout is by sample, offset for yz data is 2*XTotalPixels
			written += yzout_task.WriteAnalogI16((_xyzp.data() + 2*XTotalPixels + 2*b*yblocksize)	// set offset in the data vector
				, ((b==_blocks-1)?lastyblocksize:yblocksize), false, 2, DAQmx_Val_GroupByScanNumber);								// data layout is by sample
		}
		
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }

	writeabort = false;
	DBOUT(L"OutputsDAQmxLineClock::Write written " << written << L" samples");
	return written;
}

ZeroOutputsDAQmxLineClock::ZeroOutputsDAQmxLineClock(const parameters::OutputsDAQmxLineClock& _params) {
	DAQmx::CDAQmxAnalogOutTask task;
	try {
		task.CreateTask();
		task.CreateAOVoltageChannel(_params.xpout()
			, L"XPOut"
			, -_params.range()
			, _params.range());
		task.WriteAnalogI16(std::make_shared<std::vector<int16>>(2,(int16)0)->data(), 1, true);
		task.WaitUntilDone(200);
		task.Clear();
		task.CreateTask();
		task.CreateAOVoltageChannel(_params.yzout()
			, L"YZOut"
			, -_params.range()
			, _params.range());
		task.WriteAnalogI16(std::make_shared<std::vector<int16>>(2,(int16)0)->data(), 1, true);
		task.WaitUntilDone(200);
		task.Clear();
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}


}
