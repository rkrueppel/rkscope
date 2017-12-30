#pragma once

#include "FPGAInterface.h"
#include "FPGAIO6587.h"
#include "NiFpga_PhotonCounterV2.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGAPhotonCounter;
	}
}

namespace scope {

	/** Wraps an FPGA that does photon counting (for Version 2 VI)
	* @ingroup ScopeComponentsHardware */
	class FPGAPhotonCounter :
		public FPGAInterface,
		public FPGAIO6587,
		public SupportedAreas<1, 1> {

	protected:
		/** the parameter set */
		parameters::InputsFPGAPhotonCounter* parameters;

		/** both fifos for both channels */
		std::array<NiFpga_PhotonCounterV2_TargetToHostFifoU16, 2> fifos;

		/** programmed sampling rate (usually 1-1.4GHz), this is double the IO modules clock rate */
		double samplingrate;
	
	public:
		/** Load the FPGA bitfile, reset, set the IO module's onboard clock, initialize the acquisition */
		FPGAPhotonCounter();

		/** Close FPGA session */
		~FPGAPhotonCounter();

		void Initialize(parameters::InputsFPGA* _parameters) override;
		double SetPixeltime(const uint32_t& _area, const double& _pixeltime) override;
		double SetLinetime(const uint32_t& _area, const double& _linetime) override;
		void SetTriggering(const bool& _waitfortrigger) override;
		void SetContinuousAcquisition(const bool& _cont) override;
		void SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) override;
		void StartAcquisition() override;
		void StopAcquisition() override;
		int32_t ReadPixels(DaqMultiChunk<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout) override;

		/** Checks the status of the FIFOs on the FPGA */
		void CheckFPGADiagnosis();

		/** Clears the interloop and ToHost FIFOs */
		void ClearFIFOs();

		/** Set the counting mode on the FPGA. Count high samples (true) or count pulses/flanks (false). */
		void SetCountMode(const bool& _mode);
	};

}