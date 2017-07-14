#pragma once
#include "PixelmapperBasic.h"

namespace scope {

/** Maps acquired pixels into an image, takes care of onset cutoff, retrace etc.
* C: Cutoff, I: real image, R: retrace
* Chunk has all samples C&I&R, but we only want to use I samples
* It gets complicated because the end of a chunk can be anywhere
*
* 	CCCCCCCCCCCCC
* 	CCCCCCCCCCCCC
* 	CCIIIIIIIIIRR
* 	CCIIIIIIIIIRR
* 	CCIIIIIIIIIRR
* 	CCIIIIIIIIIRR
* 	CCIIIIIIIIIRR
* 	RRRRRRRRRRRRR
* 	RRRRRRRRRRRRR */
class PixelmapperFrameSaw
 : public PixelmapperBasic {

public:
	PixelmapperFrameSaw();

	/** Maps a chunk via the lookup vector */
	PixelmapperResult LookupChunk(DaqChunkPtr const _chunk, const uint16_t& _currentavgcount) override;		
};

}