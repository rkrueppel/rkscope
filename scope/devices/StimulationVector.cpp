#include "StdAfx.h"
#include "StimulationVector.h"

namespace scope {

StimulationVector::StimulationVector()
	: vecptr(std::make_shared<std::vector<uint8_t>>(round2ui32(parameters.duration() * 10000), 0U)) {
}

void StimulationVector::UpdateVector() {
	uint32_t onsamples = round2ui32(parameters.ontime() * 10000);
	uint32_t offsamples = round2ui32(parameters.offtime() * 10000);
	uint32_t onsetsamples = round2ui32(parameters.onset() * 10000);
	uint32_t durationsamples = round2ui32(parameters.duration() * 10000);

	vecptr->resize(durationsamples);
	std::fill_n(vecptr->begin(), onsetsamples, 0U);						// Just to be safe
	std::vector<uint8_t>::iterator it(vecptr->begin() + onsetsamples);
	do {
		if ( (it + onsamples) > vecptr->end() )
			break;
		std::fill_n(it, onsamples, 2^8);
		it += onsamples;
		if ( (it + offsamples) > vecptr->end() )
			break;
		it += offsamples;
	} while (1);
}

}