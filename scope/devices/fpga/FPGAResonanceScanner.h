#pragma once

#include "FPGAInterface.h"
#include "FPGAIO5751.h"
#include "NiFpga_ResonanceScanner.h"

// Forward declaration
namespace scope {
	class DaqChunk;
	namespace parameters {
		class InputsFPGAResonanceScanner;
	}
}

namespace scope {

/** Wraps an FPGA that does analog pixel integration (with the NI 5751 FlexRIO adapter module http://sine.ni.com/nips/cds/view/p/lang/en/nid/209099)
* @ingroup ScopeComponentsHardware */
class FPGAResonanceScanner :
	public FPGAInterface,
	public FPGAIO5751 {

protected:
	/** the parameter set */
	parameters::InputsFPGAResonanceScanner* parameters;

	/** both fifos for both channels */
	std::array<NiFpga_ResonanceScanner_TargetToHostFifoU32, 2> fifos;

public:
	/** Load the FPGA bitfile, set the IO module's onboard clock and initialize the acquisition */
	FPGAResonanceScanner();

	/** Close FPGA session */
	~FPGAResonanceScanner();

	void Initialize(parameters::InputsFPGA* _parameters) override;
	double SetPixeltime(const uint32_t& _area, const double& _pixeltime) override;
	double SetLinetime(const uint32_t& _area, const double& _linetime) override;
	void SetTriggering(const bool& _waitfortrigger) override;
	void SetContinuousAcquisition(const bool& _cont) override;
	void SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) override;
	void StartAcquisition() override;
	void StopAcquisition()  override;
	void SetScannerdelay(const uint32_t& _scannerdelay) override;

	int32_t ReadPixels(DaqChunk& _chunk, const double& _timeout, bool& _timedout) override;

	/** Set channel properties as baseline and bitshift */
	void SetChannelProps();

	/** Checks the status of the FIFOs on the FPGA */
	void CheckFPGADiagnosis();

	/** Clears the interloop and ToHost FIFOs */
	void ClearFIFOs();
};

}