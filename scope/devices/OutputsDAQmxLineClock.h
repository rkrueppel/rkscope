#pragma once

#include "Outputs.h"
#include "devices/daqmx/DAQmxTask.h"

// Forward declarations
namespace scope {
	namespace parameters {
		class OutputsDAQmxLineClock;
		class Scope;
	}
}

namespace scope {

/** Wraps hardware connection for signal output to scanners, fast z control, and pockels cell with NI-DAQmx. X/Pockels use a pixel
* clock and Y/Z use a different, a line clock. So you need to NI cards to get this working (since one card can only run
* one output task with only one clock. */
class OutputsDAQmxLineClock
	: public Outputs {

protected:
	/** Task for x-scanner/Pockels clocked by the pixel clock from the FPGA */
	DAQmx::CDAQmxAnalogOutTask xpout_task;

	/** Task for y-scanner/fast z clocked by the line clock from the FPGA */
	DAQmx::CDAQmxAnalogOutTask yzout_task;

	/** Length of a line in samples */
	int32_t XTotalPixels;

	/** Length of a column in samples */
	int32_t YTotalLines;

public:
	/** Creates the tasks and clock tasks */
	OutputsDAQmxLineClock(const uint32_t& _area, const parameters::OutputsDAQmxLineClock& _outputparams, const parameters::Scope& _params);

	~OutputsDAQmxLineClock(void);

	void Start() override;

	void Stop() override;

	int32_t Write(const std::vector<int16_t>& _xyzp, const uint32_t& _blocks) override;
};

/** Wraps hardware connection for zeroing signal output to scanners, fast z control, and pockels cell with NI-DAQmx. */
class ZeroOutputsDAQmxLineClock {
public:
	/** Create a simple tasks for writing a scalar zero to all channels. Use values from the standard parameters for 
	* devices, channels, etc. Make sure all tasks are cleared before calling this! */
	ZeroOutputsDAQmxLineClock(const parameters::OutputsDAQmxLineClock& _params);
};

}
