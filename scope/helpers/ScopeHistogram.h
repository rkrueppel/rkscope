#pragma once

#include "ScopeImage.h"
#include "helpers/ScopeDatatypes.h"

namespace scope {

/** A histogram for a uint16_t image with uint32_t counts */
class ScopeHistogram {

protected:
	/** for data protection */
	mutable std::mutex mutex;
	
	/** range of uint16 to do histogram of */
	uint16_t range;

	/** size of each bin */
	double binsize;

	/** data vector */
	std::vector<uint32_t> hist;

public:
	/** Initialize to binsize 1 (histogram size is thus UINT16_MAX+1) and zero counts */
	ScopeHistogram(const uint32_t& _no_of_bins = 512, const uint16_t& _range = UINT16_MAX);

	/** safe copy constructor */
	ScopeHistogram(const ScopeHistogram& _h);

	/** @return current bin size */
	double Binsize() const;

	/** Calculate the histogram
	* @param[in] _img the uint16 ScopeImage to calculate from
	* @param[in] _loghistogram if true the histogram contains the logarithms of the counts, if false it contains the counts */
	void Calculate(ScopeImageU16CPtr const _img, const bool& _loghistogram = false);

	/** Resize the histogram to a new number of bins
	* @post size of data vector is _no_of_bins */
	void Resize(const uint32_t& _no_of_bins);

	/** Update the range */
	void UpdateRange(uint16_t&  _range);

	/** @return the maximum count in the histogram */
	uint32_t MaxCount() const;

	/** @return the first position that has a count > 0 */
	uint16_t FirstCountPosition() const;

	/** @return the last position that has a count > 0 */
	uint16_t LastCountPosition() const;

	/** @return a constant pointer to the histogram data
	* @post mutex is locked shared */
	const std::vector<uint32_t>* GetHistConst() const;

	/** @post mutex is unlocked */
	void ReleaseHistConst() const;
};

/** Shared pointer to a histogram */
typedef std::shared_ptr<ScopeHistogram> ScopeHistogramPtr;

}