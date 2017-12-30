#pragma once

#include "PixelmapperBasic.h"

// Forward declarations
namespace scope {
	class DaqChunk;
	typedef std::shared_ptr<DaqChunk> DaqChunkPtr;
}

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
		PixelmapperResult LookupChunk(DaqChunkPtr const _chunk, const uint16_t& _currentavgcount);
	};

}
