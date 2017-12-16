#pragma once

#include "Inputs.h"
#include "config\config_choices.h"
#include "devices/daqmx/DAQmxTask.h"
#include "helpers/ScopeDatatypes.h"

// Forward declarations
namespace scope {
	namespace parameters {
		class InputsDAQmx;
		class Scope;
	}
}

namespace scope {

	/** Wraps hardware connection for signal input from PMTs with NI-DAQmx. */
	class InputsDAQmx
		: public Inputs {

	protected:
		/** The DAQmx task */
		DAQmx::CDAQmxAnalogInTask task;
	
		/** the standard chunk size per channel for Read */
		uint32_t standardchunksize;

	public:
		/** Creates a task for PMT signal acquisition with NI DAQmx.\n
		* Configure sample timings for output tasks.
		* With DaqTiming ReferenceClock both tasks use the onboard oscillator (empty timing source string) which is
		* synchronized to the PXI 10MHz backplane clock used as reference clock.
		* Read http://www.ni.com/white-paper/3615/en, "M Series Synchronization with LabVIEW and NI-DAQmx" at NI Developer Zone.\n
		* Configures the triggering for all input tasks. Since all output&input tasks have to start synchronously, the start trigger for all
		* is the /ao/StartTrigger signal of the first area's analog output task */
		InputsDAQmx(const uint32_t& _area, const parameters::InputsDAQmx* const _inputparams, const parameters::Scope& _params);

		/** Stop and clear input tasks. */
		~InputsDAQmx();

		void Start() override;

		void Stop() override;

		uint32_t StandardChunkSize() const override;

		int32_t Read(const uint32_t& _area, config::DaqMultiChunkType& _chunk, bool& _timedout, const double& _timeout) override;
	};

}

