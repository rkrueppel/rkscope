#include "StdAfx.h"
#include "DaqChunk.h"
#include "helpers/helpers.h"

namespace scope {

DaqChunk::DaqChunk(const uint32_t& _perchannel, const uint32_t& _nchannels, const uint32_t& _area)
	: area(_area)
	, nchannels(_nchannels)
	, perchannel(_perchannel)
	, data(_perchannel*_nchannels, 0)
	, lastmapped(_nchannels, std::begin(data)) {
	assert(area < SCOPE_NAREAS);
	size_t c = 0;
	for (auto& l : lastmapped )
		l = std::begin(data) + c++*perchannel;
}

void DaqChunk::Downsample(const uint32_t& _factor) {
	assert( (perchannel % _factor) == 0 );		// make sure we have _factor*perchannel=pixelsperchannel
	
	if ( _factor == 1 )
		return;
	uint32_t acc = 0;								// accumulate as 32bit to avoid overflow
	uint32_t halffactor = _factor>>2;
	auto pixelend = std::begin(data);
	// We do not have to care about the channel borders here.
	// Outer loop increases iterator for downsampled pixels (overwrites old pixels)
	for ( auto it = std::begin(data), downit = std::begin(data); it != std::end(data) ; ++downit ) {
		acc = 0;
		pixelend = it + _factor;
		// inner loop increases iterator for pixels and accumulates
		for ( ; it != pixelend ; ++it)
			acc = acc + static_cast<uint32_t>(*it);
		*downit = static_cast<uint16_t>(acc / _factor) + ((acc%_factor)>halffactor?1u:0u);		// quick rounding trick for integer multiplication
	}
	perchannel /= _factor;
	// since we have overwritten part of the vector with the downsampled pixels, we resize it now
	data.resize(perchannel*nchannels);
	
	size_t c = 0;
	for (auto& l : lastmapped )
		l = std::begin(data) + c++*perchannel;
}

void DaqChunk::Scale(const double& _factor) {
	assert(_factor > 0 );
	std::transform(std::begin(data), std::end(data), std::begin(data), [&_factor](uint16_t& elem) {
		return round2ui16(_factor * static_cast<double>(elem)); } );
}

}