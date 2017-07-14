#include "stdafx.h"
#include "PixelmapperFrameResonanceHW.h"
#include "parameters\Framescan.h"
#include "helpers\DaqChunk.h"
#include "helpers\ScopeImage.h"
#include "helpers\ScopeMultiImage.h"

namespace scope {

PixelmapperFrameResonanceHW::PixelmapperFrameResonanceHW()
	: PixelmapperBasic(ScannerTypeHelper::Resonance, ScannerVectorTypeHelper::ResonanceBiDi) {
}

PixelmapperResult PixelmapperFrameResonanceHW::LookupChunk(DaqChunkPtr const _chunk, const uint16_t& _currentavgcount) {
	PixelmapperResult result(Nothing);

	return result;
}


}
