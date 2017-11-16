#pragma once

#include "DaqMultiChunk.h"

namespace scope {

	/** A DaqChunk contains data from all areas and channels sequentially and additionally a bool vector for the resonance scanner sync signal.
	* First area 0 with first perchannel samples of channel 0, then perchannel samples of channel 1 etc
	* Then area 1 with etc.
	* NOT thread-safe!
	* @tparam NAREAS is the number of areas
	* @tparam DATA_T is the datatype of the chunk data.*/
	template<uint32_t NAREAS = 1, class DATA_T = uint16_t>
	class DaqMultiChunkResonance
		: public DaqMultiChunk<NAREAS, DATA_T> {

	public:
		/** Iterator over the sync vector */
		typedef std::vector<bool>::iterator iteratorSync;

		/** The vector with booleans for the synchronization signal for the resonance scanner */
		std::vector<bool> resSync;

		/** Iterators to positions of the last used sync signal point. One for every channel. */
		std::vector<iteratorSync> lastsyncsig;

		/** @param[in] _perchannel number of samples the chunk should contain per channel
		* @param[in] _nchannels number of channels in the chunk (total number of samples is thus NAREAS*_perchannel*_nchannels) */
		DaqChunkResonance(const uint32_t& _perchannel, const uint32_t& _nchannels)
			: DaqMultiChunk(_perchannel, _nchannels)
			, resSync(_perchannel, 0)
			, lastsyncsig(_nchannels, std::begin(resSync))
		{
			for (auto& s : lastsyncsig )
				s = std::begin(resSync);
		}
		
		/** @name Mutator/accessor for the resonance sync stuff
		* @{ */
		iteratorSync GetLastSyncSig(const uint32_t& _channel) const { return lastsyncsig[_channel]; }
		void SetLastSyncSig(const uint32_t& _channel, const iteratorSync& _last) { lastsyncsig[_channel] = _last; }
		/** @} */
	};

	/** A shared pointer to a DaqMultiChunkResonance */
	template<uint32_t NAREAS = 1, class DATA_T = uint16_t>
	using DaqMultiChunResonancekPtr = std::shared_ptr<DaqMultiChunkResonance<NAREAS, DATA_T>;
}