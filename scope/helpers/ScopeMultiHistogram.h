#pragma once

#include "helpers/ScopeDatatypes.h"

// Forward declarations
namespace scope {
class ScopeMultiImage;
typedef std::shared_ptr<const ScopeMultiImage> ScopeMultiImageCPtr;
class ScopeHistogram;
typedef std::shared_ptr<ScopeHistogram> ScopeHistogramPtr;
}

namespace scope {

/** A multi channel histogram.
* Mutex-protected if you use GetHist and ReleaseHist correctly */
class ScopeMultiHistogram {

protected:
	/** area of the histogram */
	const uint32_t area;

	/** number of channels in the area/histogram */
	const uint32_t channels;

	/** vector with histograms for each channel */
	std::vector<ScopeHistogramPtr> hists;

public:
	/** Initialize all channels */
	ScopeMultiHistogram(const uint32_t& _area = 0, const uint32_t& _channels = 1, const uint32_t& _no_of_bins = 512, uint16_t  _range = UINT16_MAX);

	/** @param[in] _multi the multi image to calculate the multi histogram for
	* @param[in] _loghistogram if true the histogram contains the logarithms of the counts, if false it contains the counts */
	void Calculate(ScopeMultiImageCPtr const _multi, const bool& _loghistogram = false);

	/** Resize the multi histogram to a new number of bins */
	void Resize(const uint32_t& _no_of_bins);

	/** Update the histogram range to a new value */
	void UpdateRange(uint16_t&  _range);

	/** @return vector with maximum counts in every channel */
	std::vector<uint32_t> MaxCounts() const ;

	/** @return vector with positions of first counts >0 */
	std::vector<uint16_t> FirstCountPositions() const;

	/** @return vector with positions of last counts >0 */
	std::vector<uint16_t> LastCountPositions() const;

	/** @return constant pointer to the histogram for one channel
	* @post mutex for that channel is locked shared */
	const std::vector<uint32_t>* GetHistConst(const uint32_t& _c) const;

	/** @post mutex for that channel is unlocked */
	void ReleaseHistConst(const uint32_t& _c) const;
};

/** a shared pointer to a ScopeMultiHistogram */
typedef std::shared_ptr<ScopeMultiHistogram> ScopeMultiHistogramPtr;

}

