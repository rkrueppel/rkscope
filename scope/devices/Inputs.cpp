#include "stdafx.h"
#include "Inputs.h"
#include "helpers\helpers.h"

namespace scope {

Inputs::Inputs(const uint32_t& _area)
	: area(_area) {
	requested_samples = 10000;
}

Inputs::~Inputs(void) {

}

uint32_t Inputs::RequestedSamples() const {
	return requested_samples;
}

}
