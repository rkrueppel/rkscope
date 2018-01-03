#include "stdafx.h"
#include "PixelmapperFrameResonanceHW.h"
#include "parameters\Framescan.h"
#include "helpers\DaqChunks.h"
#include "helpers\ScopeImage.h"
#include "helpers\ScopeMultiImage.h"

namespace scope {

PixelmapperFrameResonanceHW::PixelmapperFrameResonanceHW()
	: PixelmapperBasic(ScannerTypeHelper::Resonance, ScannerVectorTypeHelper::ResonanceBiDi) {
}

PixelmapperResult PixelmapperFrameResonanceHW::LookupChunk(DaqMultiChunk<2, 2, uint16_t>& _chunk, const uint16_t& _currentavgcount) {
	PixelmapperResult result(Nothing);

	return result;
}


}
