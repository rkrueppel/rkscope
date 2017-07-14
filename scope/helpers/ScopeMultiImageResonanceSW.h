#pragma once

#include "ScopeMultiImage.h"

// Forward declarations
namespace scope {
template<class T>class ScopeImage;
typedef std::shared_ptr<ScopeImage<uint16_t>> ScopeImageU16Ptr;
}

namespace scope {

/** A multichannel image */
class ScopeMultiImageResonanceSW
	: public ScopeMultiImage {

protected:
	/** position in the image that was filled with a value last time (for software mapping resonance scanner mode) */
	std::vector<uint32_t> lastimagepos;

	/** direction of the line that was filled last time (for software mapping resonance scanner mode) */
	std::vector<bool> lastforthline;

	/** line of the image that was being processed last time (for software mapping resonance scanner mode) */
	std::vector<uint32_t> lastl;

	/** x position in the image that was being processed last time (for software mapping resonance scanner mode) */
	std::vector<uint32_t> lastx;

	/** intermediate vector for storing the backward line (for software mapping resonance scanner mode) */
	std::vector<std::vector<uint16_t>> currentlinedata;

public:
	/** Initializes and generate blank images for each channel */
	ScopeMultiImageResonanceSW(const uint32_t& _area = 0, const size_t& _nochannels = 1, const uint32_t& _lines = 256, const uint32_t& _linewidth = 256);

	/** @name Several accessor methods */
	/** @{ */
	bool LastForthline(const size_t& _chan) const { return lastforthline[_chan]; }

	uint32_t LastImagePos(const size_t& _chan) const { return lastimagepos[_chan]; }

	uint32_t LastL(const size_t& _chan) const { return lastl[_chan]; }

	uint32_t LastX(const size_t& _chan) const { return lastx[_chan]; }

	std::vector<std::vector<uint16_t>>* CurrentLineData() const { return (std::vector<std::vector<uint16_t>>*)&currentlinedata; }
	/** @} */

	/** @name Several mutator methods */
	/** @{ */
	/** Sets last the position in the current frame that was last mapped (for software mapping resonance scanner mode) */
	void SetLastImagepos(const size_t& _chan, const uint32_t& _value) { lastimagepos[_chan] = _value; }

	/** Sets boolean indicating the direction of the last line (for software mapping resonance scanner mode) */
	void SetLastForthline(const size_t& _chan, const bool& _value) { lastforthline[_chan] = _value; }

	/** Sets last line (for software mapping resonance scanner mode) */
	void SetLastL(const size_t& _chan, const uint32_t& _value) { lastl[_chan] = _value; }

	/** Sets last x (for software mapping resonance scanner mode) */
	void SetLastX(const size_t& _chan, const uint32_t& _value) { lastx[_chan] = _value; }
	/** @} */

	/** Initializes currentlinedata (for software mapping resonance scanner mode) */
	void InitializeCurrentLineData(const uint32_t& _size) { currentlinedata = std::vector<std::vector<uint16_t>>(nochannels, std::vector<uint16_t>(_size, 0)); }
};

/** Shared pointer to a ScopeMultiImageResonanceSW */
typedef std::shared_ptr<ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWPtr;
/** Shared pointer to a const ScopeMultiImageResonanceSW */
typedef std::shared_ptr<const ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWCPtr;
}