#pragma once

#include "FPGAInterface.h"
#include "FPGAIO5771.h"
#include "NiFpga_AnalogDemultiplexer_NI5771_Resonance.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGAAnalogDemultiplexer;
	}
}

namespace scope {

	/** Wraps an FPGA that does two area demultiplexing and analog integration with the NI 5771 FlexRio adapter module on the NI 7962R FPGA
	* @ingroup ScopeComponentsHardware */
	class FPGAAnalogDemultiplexerResonance :
		public FPGAInterface, 
		public FPGAIO5771 {

		protected:
			/** the parameter set */
			parameters::InputsFPGAAnalogDemultiplexer* parameters;

			/** all FIFOs for both areas */
			std::array<NiFpga_AnalogDemultiplexer_NI5771_Resonance_TargetToHostFifoU64, 2> fifos;

			/** requested pixels for both areas */
			std::array<NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU32, 2> reqpixels;

			/** samples per pixel for both areas */
			NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU16 smplsperpixel;

		public:
			/** Load the FPGA bitfile, set the IO module's onboard clock and initialize the acquisition */
			FPGAAnalogDemultiplexerResonance();

			/** Close FPGA session */
			~FPGAAnalogDemultiplexerResonance();

			void Initialize(parameters::InputsFPGA* _parameters) override;
			double SetPixeltime(const uint32_t& _area, const double& _pixeltime) override;
			double SetLinetime(const uint32_t& _area, const double& _linetime) override;
			void SetTriggering(const bool& _waitfortrigger) override;
			void SetContinuousAcquisition(const bool& _cont) override;
			void SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) override;
			void StartAcquisition() override;
			void StopAcquisition()  override;

			int32_t ReadPixels(const uint32_t& _area, DaqMultiChunk<SCOPE_NBEAM_AREAS, uint16_t>& _chunk, const double& _timeout, bool& _timedout) override;

			/** Checks the status of the FIFOs on the FPGA */
			void CheckFPGADiagnosis();

			/** Set channel properties as baseline cutoff and bitshift */
			void SetChannelProps();

			/** Clears the interloop and ToHost FIFOs */
			void ClearFIFOs();
	};

}