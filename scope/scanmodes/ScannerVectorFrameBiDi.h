#pragma once

#include "ScannerVectorFrameBasic.h"

namespace scope {

/** A scanner vector for bidirectional frame scanning */
class ScannerVectorFrameBiDi :
	public ScannerVectorFrameBasic {

protected:

protected:
	/** Calculate the scanner vector based on the current parameters */
	void UpdateVector() override;

	/** Fill the samples for the x scanner axis */
	void FillX();

	/** Fill the samples for the y scanner axis */
	void FillY();

	/** Fill the samples for the fast z axis (stays constant here) */
	void FillZ();

	/** Fill the samples for the Pockels cell (cutoff&retrace blanking for x and y) */
	void FillP();

	/** Fill in the lookup vector */
	void FillLookup();

public:
	/** * @param[in] _filltype type of vector fill, see GetInterleavedVector for details */
	ScannerVectorFrameBiDi(const ScannerVectorFillType& _filltype);

	~ScannerVectorFrameBiDi();
};

/** A shared pointer to a ScannerVectorFrameBiDi */
typedef std::shared_ptr<ScannerVectorFrameBiDi> ScannerVectorFrameBiDiPtr;


}