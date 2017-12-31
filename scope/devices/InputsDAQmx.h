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
		* is the /ao/StartTrigger signal of the first area's analog output task
		* @param[in] _area has to be 0 since only one area supported right now through daqmx */
		InputsDAQmx(const uint32_t& _area, const parameters::InputsDAQmx* const _inputparams, const parameters::Scope& _params);

		/** Stop and clear input tasks. */
		~InputsDAQmx();

		void Start() override;

		void Stop() override;

		uint32_t StandardChunkSize() const override;

		template<uint32_t NCHANNELS>
		int32_t Read(DaqMultiChunk<NCHANNELS, 1, uint16_t>& _chunk, bool& _timedout, const double& _timeout) override {
			int32_t read = 0;
			bool timedout = false;
			try {
				auto start = _chunk.GetDataStart(0);
				read = task.ReadU16(start, start + NCHANNELS * _chunk.PerChannel(), _chunk.PerChannel(), NCHANNELS, timedout, _timeout);
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__); }
			return read;
		}
	};

}

