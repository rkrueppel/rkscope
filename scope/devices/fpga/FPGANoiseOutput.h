#pragma once

#include "FPGAInterface.h"
#include "NiFpga_NoiseOutput_PXIe7962R.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGANoiseOutput;
	}
}

namespace scope {

/** FPGA class for vi that generates random pixel data
* @ingroup ScopeComponentsHardware */
class FPGANoiseOutput
	: public FPGAInterface {

protected:
	/** the parameter set */
	parameters::InputsFPGANoiseOutput* parameters;

	/** both fifos for both channels and areas*/
	std::array<NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16, 4> fifos;

	/** requested pixels for both areas */
	std::array<NiFpga_NoiseOutput_PXIe7962R_ControlU32, 2> reqpixels;

	/** samples per pixel for both areas */
	std::array<NiFpga_NoiseOutput_PXIe7962R_ControlU32, 2> smplsperpixel;

	/** samples per line for both areas */
	std::array<NiFpga_NoiseOutput_PXIe7962R_ControlU32, 2> smplsperline;
	
public:
	/** Load the FPGA bitfile, reset, set the IO module's onboard clock and initialize the acquisition */
	FPGANoiseOutput();

	/** Close FPGA session */
	~FPGANoiseOutput();

	void Initialize(parameters::InputsFPGA* _parameters) override;
	double SetPixeltime(const uint32_t& _area, const double& _pixeltime) override;
	double SetLinetime(const uint32_t& _area, const double& _linetime) override;
	void SetTriggering(const bool& _waitfortrigger) override;
	void SetContinuousAcquisition(const bool& _cont) override;
	void SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) override;
	void StartAcquisition() override;
	void StopAcquisition() override;
	int32_t ReadPixels(const uint32_t& _area, DaqMultiChunk<SCOPE_NBEAM_AREAS, uint16_t>& _chunk, const double& _timeout, bool& _timedout) override;

	/** Checks the status of the FIFOs on the FPGA */
	void CheckFPGADiagnosis();

	/** Clears the interloop and ToHost FIFOs */
	void ClearFIFOs();
};

}