#pragma once

#include "ScopeDefines.h"
#include "devices/daqmx/DAQmxTask.h"

// Forward declarations
namespace scope {
	namespace parameters {
		class Scope;
	}
}

namespace scope {

/** Wraps hardware connection for stimulation output with NI-DAQmx. */
class StimulationsDAQmx {

protected:
	/** The DAQmx tasks */
	DAQmx::CDAQmxDigitalOutTask task;

public:
	/** Creates the tasks for digital stimulation output with NI-DAQmx.\n
	* Configure timing for the stimulation task. */
	StimulationsDAQmx(const parameters::Scope& _params);

	/** Stop and clear the digital stimulation task. */
	~StimulationsDAQmx();

	/** Start stimulation task, waits on first output task sample clock */
	void Start();

	/** Write the stimulation vector to the digital stimulation task
	* @return the number of samples per channel actually written, -1 in case of error */
	virtual int32_t Write(std::shared_ptr<const std::vector<uint8_t>> const _stimvec);
};

}
