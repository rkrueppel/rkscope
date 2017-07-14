#pragma once

#include "Outputs.h"
#include "devices/daqmx/DAQmxTask.h"
#include "controllers/ScopeController.h"

// Forward declarations
namespace scope {
	namespace parameters {
		class OutputsDAQmxResonanceSlave;
		class Scope;
	}
}

namespace scope {

/** Wraps hardware connection for signal output to fast z control and pockels cell (for a slave area to a resonance master without its own scanner) with NI-DAQmx. */
class OutputsDAQmxResonanceSlave
	: public Outputs {

protected:
	/** Task for fast z/Pockels clocked by a pixel clock */
	DAQmx::CDAQmxAnalogOutTask zpout_task;

	/** a ScopeController instance */
	scope::ScopeController scope_controller;

public:
	/** Create the task for pockels and fast z */
	OutputsDAQmxResonanceSlave(const uint32_t& _area, const parameters::OutputsDAQmxResonanceSlave& _outputparams, const parameters::Scope& _params);

	~OutputsDAQmxResonanceSlave(void);

	void Start() override;

	void Stop() override;

	/** we also take a std::vector<int16_t> but for the slave area it has only samples for z&p ! */
	int32_t Write(const std::vector<int16_t>& _xyzp, const uint32_t& _blocks) override;
};

/** Wraps hardware connection for zeroing signal output to fast z control and pockels cell with NI-DAQmx for a slave to a resonance master. */
class ZeroOutputsDAQmxResonanceSlave {
public:
	/** Create a simple tasks for writing a scalar zero to all channels. Use values from the standard parameters for 
	* devices, channels, etc. Make sure all tasks are cleared before calling this! */
	ZeroOutputsDAQmxResonanceSlave(const parameters::OutputsDAQmxResonanceSlave& _params);
};

}