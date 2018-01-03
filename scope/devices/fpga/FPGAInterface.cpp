#include "stdafx.h"
#include "FPGAInterface.h"
#include "parameters/Inputs.h"

namespace scope {

	FPGAInterface::FPGAInterface()
		: status(NiFpga_Status_Success)
		, session(0)
		, initialized(false) {
	}

	FPGAInterface::~FPGAInterface() {
	}

	int32_t FPGAInterface::ReadPixels(const uint32_t& _area, DaqChunk<uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		return -1;
	}

	int32_t FPGAInterface::ReadPixels(const uint32_t& _area, DaqMultiChunk<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		return -1;
	}

	int32_t FPGAInterface::ReadPixels(const uint32_t& _area, DaqMultiChunk<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		return -1;
	}

	int32_t FPGAInterface::ReadPixels(const uint32_t& _area, DaqMultiChunkResonance<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		return -1;
	}

	int32_t FPGAInterface::ReadPixels(const uint32_t& _area, DaqMultiChunkResonance<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		return -1;
	}

}