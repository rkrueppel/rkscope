#include "stdafx.h"
#include "FPGAInterface.h"
#include "parameters/Inputs.h"
#include "helpers/DaqChunk.h"

namespace scope {

FPGAInterface::FPGAInterface()
	: status(NiFpga_Status_Success)
	, session(0)
	, initialized(false) {
}

FPGAInterface::~FPGAInterface() {
	
}

}