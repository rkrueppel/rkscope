#pragma once

#include "PixelmapperBasic.h"

namespace scope {

	/** Maps acquired pixels into an image analysing the resonance scanner sync signal, takes care of return fractions, forth/back lines etc
	* @tparam NAREAS defines how many areas are mapped in parallel (e.g. multiarea configuration with only one scanner-pair) */
	template<uint32_t NAREAS = 1>
	class PixelmapperFrameResonanceHW
		: public PixelmapperBasic<NAREAS> {

	protected:

	public:
		PixelmapperFrameResonanceHW();

		/** Maps a chunk */
		PixelmapperResult LookupChunk(DaqMultiChunk<2, 2, uint16_t>& _chunk, const uint16_t& _currentavgcount);
	};

}
