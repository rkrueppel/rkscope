#pragma once

#include "Outputs.h"
#include "devices/daqmx/DAQmxTask.h"
#include "ScopeDatatypes.h"
#include "controllers/ScopeController.h"

// Forward declarations
namespace scope {
	namespace parameters {
		class OutputsDAQmxResonance;
		class Scope;
	}
}

namespace scope  {

/** Wraps hardware connection for signal output to scanners, fast z control, and pockels cell with NI-DAQmx. */
class OutputsDAQmxResonance
	: public Outputs {

protected:
	/** The DAQmx task for x/y-scanners/fast z/Pockels clocked by a pixel clock */
	DAQmx::CDAQmxAnalogOutTask task;

	/** The DAQmx task for the resenance scanner zoom factor */
	DAQmx::CDAQmxDigitalOutTask taskResonanceZoom;

	/** a ScopeController instance */
	scope::ScopeController scope_controller;

public:
	/** Creates the task for scanner, pockels, and fast-z output with NI DAQmx.\n
	* Configure sample timings for output tasks.
	* With DaqTiming ReferenceClock both tasks use the onboard oscillator (empty timing source string) which is
	*  synchronized to the PXI 10MHz backplane clock used as reference clock.
	* Read http://www.ni.com/white-paper/3615/en, "M Series Synchronization with LabVIEW and NI-DAQmx" at NI Developer Zone.\n
	* Configures the triggering for the output tasks. Since all output&input tasks have to start synchronously, the start trigger for all
	* is the /ao/StartTrigger signal of the first area's analog output task */
	OutputsDAQmxResonance(const uint32_t& _area, const parameters::OutputsDAQmxResonance& _outputparams, const parameters::Scope& _params);

	/** Stop and clear output task. */
	~OutputsDAQmxResonance();

	/** Start task, last the first output task (all other are waiting for it as their start trigger). */
	void Start() override;

	/** Stop task */
	void Stop() override;

	/** Writes values for one frame of X-Y-fastZ and pockels data to the device buffer.
	* Use several WriteAnalogI16 commands with small parts (blocks) of the whole ScannerVectorFrame to
	* speed up the update (do not have to wait until the complete buffer is empty/ready for writing into).
	* @param[in] _xyzp the scannervector with xyzp data to write to device
	* @param[in] _reszoomfactor the factor for zooming in resonance scanner mode
	* @param[in] _blocks in how many blocks should the scannervector be written (more blocks faster update, since smaller blocksize -> buffer for this is free earlier), see "DAQmx quick buffer update.vi" */
	int32_t Write(const std::vector<int16_t> &_xyzp, const uint32_t& _blocks = 1) override;

};

/** Wraps hardware connection for zeroing signal output to scanners, fast z control, and pockels cell with NI-DAQmx. */
class ZeroOutputsDAQmxResonance {
public:
	/** Create a simple tasks for writing a scalar zero to all channels. Use values from the standard parameters for 
	* devices, channels, etc. Make sure all tasks are cleared before calling this! */
	ZeroOutputsDAQmxResonance(const parameters::OutputsDAQmxResonance& _params);
};

}

