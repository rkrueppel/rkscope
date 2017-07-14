#pragma once

#include "ScannerVectorFrameBasic.h"

namespace scope {

class ScannerVectorFrameResonanceHopper
	: public ScannerVectorFrameBasic {

protected:
	/** the actual scanner vector (if fullframevector) with x, y, fast z, Pockels either interleaved (for fullframevector)
	* or first x,p interleaved and then y,z interleaved (for not fullframevector). */
	std::shared_ptr<std::vector<int16_t>> vecresonanceptr;

	/** Calculate the scanner vector based on the current parameters */
	void UpdateVector() override;

	/** Fill the samples for the y scanner axis */
	void FillY();

	/** Fill the samples for the fast z  */
	void FillZ();

	/** Fill the samples for the Pockels cell (cutoff&retrace blanking for x and y) */
	void FillP();

public:
	/** * @param[in] _filltype type of vector fill, see GetInterleavedVector for details */
	ScannerVectorFrameResonanceHopper(const ScannerVectorFillType& _filltype);

	/** Dummy override, no scannerdelay for resonance scanners.
	* @param[in] _scannerdelaysamples the scannerdelay in samples */
	void SetScannderdelay(const uint32_t& _scannerdelaysamples) override { };
};

/** A shared pointer to a ScannerVectorFrameResonanceHopper */
typedef std::shared_ptr<ScannerVectorFrameResonanceHopper> ScannerVectorFrameResonanceHopperPtr;


}

