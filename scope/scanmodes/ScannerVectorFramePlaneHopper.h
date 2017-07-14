#pragma once

#include "ScannerVectorFrameBasic.h"

namespace scope {

/** Frame scanning with ETL plane hopping */
class ScannerVectorFramePlaneHopper :
	public ScannerVectorFrameBasic {

protected:
	/** Update the vector */
	void UpdateVector();

public:
	/** @param[in] _filltype type of vector fill, see GetInterleavedVector for details */
	ScannerVectorFramePlaneHopper(const ScannerVectorFillType& _filltype);

	~ScannerVectorFramePlaneHopper();
};

}