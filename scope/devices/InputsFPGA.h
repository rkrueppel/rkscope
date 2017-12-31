#pragma once

#include "Inputs.h"
#include "config\config_choices.h"
#include "helpers\DaqChunks.h"

// Forward declarations
namespace scope {
	namespace parameters {
		class InputsFPGA;
		class Scope;
	}
}

namespace scope {

	/** the FPGA wrapper class, used as local static variable, there can be only one FPGA!
	* Like this the FPGA bitfile is only loaded and initialized once, and not reset together with InputsFPGA in DaqControllerImpl.
	* The _parameters pointer will be cast to the derived parameters class, to fit to the FPGA class for which Scope was compiled. */
	config::InputFPGAType& theFPGA();

	/** Wraps hardware connection for signal input from PMTs using an NI FPGA.\n
	* Keep in mind to start the FPGA last, since it provides the sample clock for all other tasks. */
	class InputsFPGA
		: public Inputs {

	protected:
		/** how many laser pulses arrive per pixel, photon counts are normalized to the number of laser pulses */
		double laserpulsesperpixel;

	private:
		/** disable copy */
		InputsFPGA(const InputsFPGA& i);

		/** disable assignment */
		InputsFPGA operator=(const InputsFPGA& i) = delete;

	public:
		/** @post _params.areas[a].daq.pixeltime is set to the pixeltime coerced by the FPGA. Since the FPGA provides the sample
		* clock for the DAQmx output via PXI_Trig1, this is merely cosmetic. */
		InputsFPGA(const uint32_t& _area, parameters::InputsFPGA* _inputparams, parameters::Scope& _params);

		~InputsFPGA();

		void Start() override;

		void Stop() override;

		template<uint32_t NCHANNELS>
		int32_t Read(DaqMultiChunk<NCHANNELS, 1, uint16_t>& _chunk, bool& _timedout, const double& _timeout) override {
			int32_t read = 0;
			try {
				read = theFPGA().ReadPixels(area, _chunk, _timeout, _timedout);
				if (_timedout) {
					DBOUT(L"InputsFPGA::Read area " << area << L" timed out");
				}
				DBOUT(L"InputsFPGA::Read area " << area << L" read " << read);
				theFPGA().CheckFPGADiagnosis();
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__); }
			return read;
		}

		template<uint32_t NCHANNELS>
		int32_t Read(DaqMultiChunk<NCHANNELS, 2, uint16_t>& _chunk, bool& _timedout, const double& _timeout) override {
			int32_t read = 0;
			try {
				read = theFPGA().ReadPixels(area, _chunk, _timeout, _timedout);
				if (_timedout) {
					DBOUT(L"InputsFPGA::Read area " << area << L" timed out");
				}
				DBOUT(L"InputsFPGA::Read area " << area << L" read " << read);
				theFPGA().CheckFPGADiagnosis();
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__); }
			return read;
		}
	};

}
