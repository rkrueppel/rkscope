#include "stdafx.h"
#include "ScopeOverlayResonanceSW.h"
#include "ScopeImage.h"
#include "ScopeMultiImageResonanceSW.h"
#include "helpers.h"
#include "pixel.h"
#include "lut.h"

namespace scope {

ScopeOverlayResonanceSW::ScopeOverlayResonanceSW(const uint32_t& _lines, const uint32_t& _linewidth)
	: ScopeOverlay(_lines, _linewidth) {
}

void ScopeOverlayResonanceSW::Create(ScopeMultiImageResonanceSWCPtr const _multi, const std::vector<ColorProps>& _color_props) {
	std::lock_guard<std::mutex> lock(mutex);
	assert( (_color_props.size() == _multi->Channels()) && (lines==_multi->Lines()) && (linewidth==_multi->Linewidth()) );

	std::fill(std::begin(overlay), std::end(overlay), BGRA8BLACK);					// Clear the overlay

	for ( size_t ch = 0 ; ch < _multi->Channels() ; ch++ ) {
		ScopeImageU16CPtr chimage = _multi->GetChannel(ch);
		if ( _color_props.at(ch).Color() != None ) {						// save some time...
			ColorEnum col = _color_props.at(ch).Color();
			uint16_t ll = _color_props.at(ch).LowerLimit();
			uint16_t ul = _color_props.at(ch).UpperLimit();

			ScopeImageConstAccessU16 imagedata(*chimage);
			auto it = imagedata.GetConstData()->begin();

			// if in resonance scanner mode, only the forward lines are shown on the screen

			// I do not see that the reason for the l+=2 is here, RKr 7/1/15
			for ( uint32_t l = 0; l < lines; l+= 2 ) {
				it = imagedata.GetConstData()->begin() + l*linewidth;
				std::transform(std::begin(overlay)+l*linewidth, std::begin(overlay)+(l+1)*linewidth, it, std::begin(overlay)+l*linewidth, [&](BGRA8Pixel& pix, const uint16_t& gray) {
					return pix + U16ToBGRA8Histo(gray, col, ll, ul);
				} );
				it = imagedata.GetConstData()->begin() + l*linewidth;
				std::transform(std::begin(overlay)+(l+1)*linewidth, std::begin(overlay)+(l+2)*linewidth, it, std::begin(overlay)+(l+1)*linewidth, [&](BGRA8Pixel& pix, const uint16_t& gray) {
					return pix + U16ToBGRA8Histo(gray, col, ll, ul);
				} );
			}
		}
	}
}

}


