#pragma once

#include "FPGAInterface.h"
#include "FPGAIO6587.h"
#include "NiFpga_DigitalDemultiplexerV3.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGADigitalDemultiplexer;
	}
}

namespace scope {

	/** Wraps an FPGA that does two area demultiplexing and photon counting
	* @ingroup ScopeComponentsHardware */
	class FPGADigitalDemultiplexer :
		public FPGAInterface, 
		public FPGAIO6587,
		public SupportedAreas<2, 2> {

		protected:
			/** the parameter set */
			parameters::InputsFPGADigitalDemultiplexer* parameters;

			/** all FIFOs for both areas both channels */
			std::array<NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16, 4> fifos;

			/** Pixel dwell time in microseconds */
			std::array<double, 2> pixeltimes;

			/** requested pixels for both areas */
			std::array<NiFpga_DigitalDemultiplexerV3_ControlU32, 2> reqpixels;

			/** samples per pixel for both areas */
			std::array<NiFpga_DigitalDemultiplexerV3_ControlU16, 2> smplsperpixel;

			/** samples per pixel for both areas, for the acquisition loop*/
			std::array<NiFpga_DigitalDemultiplexerV3_ControlU16, 2> smplsperpixelacq;

			/** programmed sampling rate (usually 1-1.4GHz), this is double the IO modules clock rate */
			double samplingrate;

		public:
			/** Load the FPGA bitfile, set the IO module's onboard clock, initialize the acquisition */
			FPGADigitalDemultiplexer();

			/** Close FPGA session */
			~FPGADigitalDemultiplexer();

			void Initialize(parameters::InputsFPGA* _parameters) override;
			double SetPixeltime(const uint32_t& _area, const double& _pixeltime) override;
			double SetLinetime(const uint32_t& _area, const double& _linetime) override;
			void SetTriggering(const bool& _waitfortrigger) override;
			void SetContinuousAcquisition(const bool& _cont) override;
			void SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) override;
			void StartAcquisition() override;
			void StopAcquisition()  override;

			/** Resets the acquisition */
			void ResetAcquisition();

			int32_t ReadPixels(DaqMultiChunk<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout) override;

			/** Checks the status of the FIFOs on the FPGA */
			void CheckFPGADiagnosis();

			/** Clears the interloop and ToHost FIFOs */
			void ClearFIFOs();

			/** Set the counting mode on the FPGA. Count high samples (true) or count pulses/flanks (false). */
			void SetCountMode(const bool& _mode);
	};

}