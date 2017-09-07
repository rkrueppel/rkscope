#include "stdafx.h"
#include "StimulationsDAQmx.h"
#include "parameters/Scope.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeException.h"

namespace scope {

StimulationsDAQmx::StimulationsDAQmx(const parameters::Scope& _params) {
	if ( _params.stimulation.enable.Value() ) {
		task.CreateTask();
		task.CreateDOChannel(_params.stimulation.channel, L"DigStimOut");
		int32 samplingtype = (_params.requested_mode()==DaqModeHelper::continuous)?DAQmx_Val_ContSamps:DAQmx_Val_FiniteSamps;
		// Calculate pixelrate and number of pixels to generate
		double pixelrate = 1/(_params.areas[ThisAreaOrMasterArea(0)].daq.pixeltime()*1E-6);
		int32_t pixelsperchan = _params.areas[ThisAreaOrMasterArea(0)].Currentframe().TotalPixels();;
		if ( _params.requested_mode() == DaqModeHelper::nframes )
			pixelsperchan = _params.areas[ThisAreaOrMasterArea(0)].Currentframe().TotalPixels() * _params.areas[ThisAreaOrMasterArea(0)].daq.requested_frames() * _params.areas[ThisAreaOrMasterArea(0)].daq.averages();

		// Synchronize stimulation to the first output task
		task.ConfigureSampleTiming(_params.stimulation.timingsource(), pixelrate, pixelsperchan, samplingtype);
	}
}

StimulationsDAQmx::~StimulationsDAQmx() {
	task.Stop();
	task.Clear();
}

void StimulationsDAQmx::Start() {
	task.Start();
}

int32_t StimulationsDAQmx::Write(std::shared_ptr<const std::vector<uint8_t>> const _stimvec) {
	int32_t written = -1;
	try {
		written = task.WriteDigitalU8(_stimvec->data(), static_cast<int32_t>(_stimvec->size()));
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	return written;
}

}