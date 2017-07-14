#pragma once

#include "ScopeDefines.h"

// Forward declarations
namespace scope {
	class DaqChunk;
}

namespace scope {

/** Wraps hardware connection for signal input from PMTs. */
class Inputs {

protected:
	/** the area */
	const uint32_t area;

	/** the total number of samples that should be read (in case of nframes mode) otherwise the buffer size (usually one frame) */
	uint32_t requested_samples;

public:
	/** get the area and set up everything */
	Inputs(const uint32_t& _area);
	
	virtual ~Inputs(void);

	/** Start task. */
	virtual void Start() = 0;

	/** Stops task. */
	virtual void Stop() = 0;

	/** @return the total number of samples that should be read for that area. */
	virtual uint32_t RequestedSamples() const;

	/** @return the standard size of a read chunk (per channel) */
	virtual uint32_t StandardChunkSize() const { return 128u*128u; }

	/** Reads one chunk of samples for one area.
	* @param[in,out] _chunk chunk to fill with the read data
	* @param[out] _timedout true if read timed out
	* @param[in] _timeout timeout in seconds after which to return even if not enough samples were read
	* @return the number of samples per channel actually read */
	virtual int32_t Read(DaqChunk& _chunk, bool& _timedout, const double& _timeout) = 0;
};

}
