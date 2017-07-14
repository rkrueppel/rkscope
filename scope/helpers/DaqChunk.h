#pragma once

#include "../ScopeDefines.h"

namespace scope {

/** A DaqChunk contains data from all channels sequentially.
* First perchannel samples of channel 0, then perchannel samples of channel 1 etc etc... */
class DaqChunk {

protected:
	/** Area of the DaqChunk */
	const uint32_t area;

	/** Number of channels in that area/DaqChunk */
	const uint32_t nchannels;

	/** Number of samples per channel */
	uint32_t perchannel;

public:
	/** Iterator over the data vector */
	typedef std::vector<uint16_t>::iterator iterator;

	/** The data vector */
	std::vector<uint16_t> data;

	/** Iterators to positions that was last mapped. One for every channel. */
	std::vector<iterator> lastmapped;

	/** @param[in] _perchannel number of samples the chunk should contain per channel
	* @param[in] _nchannels number of channels in the chunk (total number of samples is thus _perchannel*_nchannels)
	* @param[in] _area sets the area value for the channel */
	DaqChunk(const uint32_t& _perchannel, const uint32_t& _nchannels, const uint32_t& _area);

	/** Virtual destructor here, to be prepared for derived classes */
	virtual ~DaqChunk() { };

	/** Downsamples by averaging _factor samples. Replaces every _factor samples by their average.
	* The chunk does shrink that way and perchannel is adjusted.
	* @param[in] _factor downsampling factor */
	virtual void Downsample(const uint32_t& _factor);

	/** Multiplies every sample by _factor */
	virtual void Scale(const double& _factor);

	/** @name Several accessor methods
	* @{ */
	uint32_t Area() const { return area; }
	uint32_t NChannels() const { return nchannels; }
	uint32_t PerChannel() const { return perchannel; }
	iterator GetLastMapped(const uint32_t& _channel) const { return lastmapped[_channel]; }
	/** @} */

	/** @name Mutator methods
	* @{ */
	void SetLastMapped(const uint32_t& _channel, const iterator& _last) { lastmapped[_channel] = _last; }
	/** @} */
};

/** A shared pointer to a DaqChunk */
typedef std::shared_ptr<DaqChunk> DaqChunkPtr;
/** A vector of shared pointers to DaqChunks (one pointer for each area) */
typedef std::vector<DaqChunkPtr> DaqAreaChunks;
/** A shared pointer to this vector */
typedef std::shared_ptr<DaqAreaChunks> DaqAreaChunksPtr;

}