#pragma once

#include "ScannerVectorFrameBasic.h"

namespace scope {

/** Calculates scanner, fast z, and pockels control voltages for a saw tooth frame scan */
class ScannerVectorFrameSaw
 : public ScannerVectorFrameBasic {

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

	/** Fill the samples for x and Pockels cell for one line. For filltype ScannerVectorFillTypeHelper::LineXPColumnYZ. */
	void FillXP();

	/** Fill the samples for y and fast z for one column (one sample per line). For filltype ScannerVectorFillTypeHelper::LineXPColumnYZ.  */
	void FillYZ();

	/** Fill the samples for fast z and Pockels for one line, used for slave areas. For filltype ScannerVectorFillTypeHelper::LineZP. */
	void FillZP();

	/** Fill in the lookup vector */
	void FillLookup();

	/** Rotate the Pockels samples in the scanner vector
	* @param[in] _pstart the first Pockels sample. If e.g. the vector is filled XYZPXYZPXYZP... this should be 3.
	* @param[in] _interleavedfactor the number of interleaved signals. For XYZP this is 4, for ZP 2 etc.
	* @param[in] _rotateby the number of samples to rotate by. */
	void RotateP(const size_t& _pstart, const size_t& _interleavedfactor, const int32_t& _rotateby);

public:
	/** * @param[in] _filltype type of vector fill, see GetInterleavedVector for details */
	ScannerVectorFrameSaw(const ScannerVectorFillType& _filltype);

	~ScannerVectorFrameSaw();
};

/** A shared pointer to a ScannerVectorFrameSaw */
typedef std::shared_ptr<ScannerVectorFrameSaw> ScannerVectorFrameSawPtr;

}