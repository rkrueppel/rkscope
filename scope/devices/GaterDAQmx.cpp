#include "stdafx.h"
#include "GaterDAQmx.h"
#include "helpers/helpers.h"
#include "daqmx/DAQmxTask.h"

namespace scope {

GaterDAQmx::GaterDAQmx(const std::wstring& _gateline, const StopCondition* const _sc)
	: gateline(_gateline)
	, sc(_sc) {
}

bool GaterDAQmx::WaitFor(const bool& _waitforhigh) {
	if ( sc->IsSet() )
		return false;

	DAQmx::CDAQmxAnalogInTask task;
	task.CreateTask();
	task.CreateAIVoltageChannel(L"PXI-6229_0/ai0", L"Gater");
	//task.ConfigureDigStartTrigger(gateline, _waitforhigh?DAQmx_Val_Rising:DAQmx_Val_Falling);
	task.ConfigureDigStartTrigger(L"PFI0", _waitforhigh?DAQmx_Val_Rising:DAQmx_Val_Falling);
	task.ConfigureSampleTiming(L"OnboardClock", 10000, 500, DAQmx_Val_ContSamps);
	
	bool timeout = true;
	std::vector<uint16_t> readvec(1,0);

	// Try repeatedly to read for 200ms, as long as the read from the task times out or until the Stop condition is set
	while ( timeout ) {
		auto b = std::begin(readvec);
		auto e = std::end(readvec);
		task.ReadU16(b, e, 1, 1, timeout, 120);
		if ( sc->IsSet() ) {
			DBOUT(L"GaterDAQmx::WaitFor StopCondition set");
			return false;
		}
	}
	DBOUT(L"GaterDAQmx::WaitFor waiting succeeded");
	return true;
}


}