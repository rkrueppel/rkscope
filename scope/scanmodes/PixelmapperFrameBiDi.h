#pragma once
#include "PixelmapperBasic.h"

namespace scope {

/** Maps acquired pixels into an image, takes care of return fractions
* I: real image, forth scan, J: real image, back scan, R: return
* Chunk has all samples C&I&R, but we only want to use I samples (and the J samples in reverse order as they come)
*
* RRRRRRRRR
* RIIIIIIIR
* RJJJJJJJR
* RIIIIIIIR
* RJJJJJJJR 
* */
class PixelmapperFrameBiDi
 : public PixelmapperBasic {

public:
	PixelmapperFrameBiDi();

	/** Maps a chunk via the lookup vector */
	PixelmapperResult LookupChunk(DaqChunkPtr const _chunk, const uint16_t& _currentavgcount) override;		
};

}