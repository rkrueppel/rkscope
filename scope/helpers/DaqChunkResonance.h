#pragma once

#include "DaqChunk.h"

namespace scope {

/** A DaqChunk contains data from all channels sequentially and additionally a bool vector for the resonance scanner sync signal.
* First perchannel samples of channel 0, then perchannel samples of channel 1 etc etc... 
* NOT thread-safe! */
class DaqChunkResonance
	: public DaqChunk {

public:
	/** Iterator over the sync vector */
	typedef std::vector<bool>::iterator iteratorSync;

	/** The vector with booleans for the synchronization signal for the resonance scanner */
	std::vector<bool> resSync;

	/** Iterators to positions of the last used sync signal point. One for every channel. */
	std::vector<iteratorSync> lastsyncsig;

	/** @param[in] _perchannel number of samples the chunk should contain per channel
	* @param[in] _nchannels number of channels in the chunk (total number of samples is thus _perchannel*_nchannels)
	* @param[in] _area sets the area value for the channel */
	DaqChunkResonance(const uint32_t& _perchannel, const uint32_t& _nchannels, const uint32_t& _area);
	
	/** @name Mutator/accessor for the resonance sync stuff
	* @{ */
	iteratorSync GetLastSyncSig(const uint32_t& _channel) const { return lastsyncsig[_channel]; }
	void SetLastSyncSig(const uint32_t& _channel, const iteratorSync& _last) { lastsyncsig[_channel] = _last; }
	/** @} */
};

/** A shared pointer to a DaqChunkResonance */
typedef std::shared_ptr<DaqChunkResonance> DaqChunkResonancePtr;
/** A vector of shared pointers to DaqChunkResonance (one pointer for each area) */
typedef std::vector<DaqChunkResonancePtr> DaqAreaChunksResonance;
/** A shared pointer to this vector */
typedef std::shared_ptr<DaqAreaChunksResonance> DaqAreaChunksResonancePtr;

}