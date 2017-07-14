#include "stdafx.h"
#include "ScopeMultiImageResonanceSW.h"
#include "ScopeImage.h"

namespace scope {

ScopeMultiImageResonanceSW::ScopeMultiImageResonanceSW(const uint32_t& _area, const size_t& _nochannels, const uint32_t& _lines, const uint32_t& _linewidth)
	: ScopeMultiImage(_area, _nochannels, _lines, _linewidth)
	, lastimagepos(_nochannels, 0)
	, lastforthline(_nochannels, true)
	, lastl(_nochannels, 0)
	, lastx(_nochannels, 0) {
	// Generate the (blank) images for each channel
}


}