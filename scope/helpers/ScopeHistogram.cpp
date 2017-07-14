#include "StdAfx.h"
#include "ScopeHistogram.h"
#include "Pixel.h"
#include "ScopeImage.h"
#include "helpers.h"

namespace scope {

ScopeHistogram::ScopeHistogram(const uint32_t& _no_of_bins, const uint16_t& _range)
	: range(_range) 
	, binsize(static_cast<double>(range-0+1)/_no_of_bins)
	, hist(_no_of_bins, 0){
}

ScopeHistogram::ScopeHistogram(const ScopeHistogram& _h)
	: range(_h.range)
	, binsize(_h.Binsize())
	, hist(*_h.GetHistConst()) {
	_h.ReleaseHistConst();
}

double ScopeHistogram::Binsize() const {
	std::lock_guard<std::mutex> lock(mutex);
	return binsize;
}

void ScopeHistogram::Calculate(ScopeImageU16CPtr const _img, const bool& _loghistogram) {
	std::lock_guard<std::mutex> lock(mutex);

	// Initialize histogram with 0s
	std::fill(std::begin(hist), std::end(hist), 0); 

	const uint16_t low(0);
	const uint16_t high(range);

	// Do the histogram
	// With const we have shared access to the image data, good for parallelism...
	{
	ScopeImageConstAccessU16 imagedata(*_img);
	std::for_each(std::begin(*imagedata.GetConstData()), std::end(*imagedata.GetConstData()), [&](const uint16_t& val) {
		// The static_cast<size_t> does floor double -> fill in the lowest bin
		if ( (val >= low) && (val <= high) )
			++( hist[ static_cast<size_t>( static_cast<double>(val-low)/binsize ) ] );
	} );
	}

	// Max count is 2^32 (~4E9), log of this is ~9.6. To put the (double) log values back into the uint32_t histogram
	// we can safely multiply them with 100000 (we could go up to ~4E8)
	if ( _loghistogram )
		std::transform(std::begin(hist), std::end(hist), std::begin(hist), [](uint32_t h) {
			return static_cast<uint32_t>(100000 * std::log10(static_cast<double>(h)));
		} );
}

void ScopeHistogram::Resize(const uint32_t& _no_of_bins) {
	std::lock_guard<std::mutex> lock(mutex);
	assert(_no_of_bins <= static_cast<uint32_t>(range-0));
	// There are +1 number of possible values (including 0)
	binsize = static_cast<double>(range-0+1)/_no_of_bins;
	hist.resize(_no_of_bins, 0);			
}

void ScopeHistogram::UpdateRange(uint16_t&  _range) {
	std::lock_guard<std::mutex> lock(mutex);
	range = _range;
}

uint32_t ScopeHistogram::MaxCount() const {
	std::lock_guard<std::mutex> lock(mutex);
	return *std::max_element(std::begin(hist), std::end(hist));
}

uint16_t ScopeHistogram::FirstCountPosition() const {
	std::lock_guard<std::mutex> lock(mutex);
	auto first = std::find_if(std::begin(hist), std::end(hist), [&](const uint32_t& c) { return c > 0; } );
	if ( first != hist.end() ) {
		// get position from iterator, multiply to get corresponding value
		return static_cast<uint16_t>(binsize * (first - std::begin(hist)));
	}
	return 0;
}

uint16_t ScopeHistogram::LastCountPosition() const {
	std::lock_guard<std::mutex> lock(mutex);
	auto last = std::find_if(hist.rbegin(), hist.rend(), [&](const uint32_t& c) { return c > 0; } );
	if ( last != hist.rend() ) {
		// get position from iterator, multiply to get corresponding value. Note: -1 because uint16_t starts at zero...
		return static_cast<uint16_t>(binsize * static_cast<double>(hist.rend() - last) - 1 );
	}
	return 0;
}

const std::vector<uint32_t>* ScopeHistogram::GetHistConst() const {
	mutex.lock();
	return &hist;
}

void ScopeHistogram::ReleaseHistConst() const {
	mutex.unlock();
}


}
