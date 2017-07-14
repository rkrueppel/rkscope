#include "StdAfx.h"
#include "ScopeOverlay.h"
#include "ScopeImage.h"
#include "ScopeMultiImage.h"
#include "helpers.h"
#include "pixel.h"
#include "lut.h"

namespace scope {

ScopeOverlay::ScopeOverlay(const uint32_t& _lines, const uint32_t& _linewidth)
	: lines(_lines)
	, linewidth(_linewidth)
	, overlay(_lines*_linewidth, 0) {
	assert( _lines!=0 && _linewidth!=0 );
}

void ScopeOverlay::Create(ScopeMultiImageCPtr const _multi, const std::vector<ColorProps>& _color_props) {
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

			// ~  185 ms for 1024x1024 (~ 180 without histogram!)
			std::transform(std::begin(overlay), std::end(overlay), it, std::begin(overlay), [&](BGRA8Pixel& pix, const uint16_t& gray) {
				return pix + U16ToBGRA8Histo(gray, col, ll, ul);
			} );

			/** Alternative incarnations I had tried out...
			// ~ 230 ms for 1024x1024
			uint32_t size = overlay.size();
			for ( uint32_t i = 0 ; i < size ; i++ )
				overlay[i] += U16ToBGRA8Histo(pixel->at(i), col, ll, ul);

			// ~1000ms for 1024x1024 This is 4x slower than the simple for loop (why?)
			auto itch = chimage->GetDataBeginConst();
			std::for_each(std::begin(overlay), std::end(overlay), [&](BGRA8Pixel& pix) {
				pix += U16ToBGRA8Histo(*itch++, col, ll, ul); } );
			// several seconds (why??? to much overhead???)
			concurrency::parallel_transform(std::begin(overlay), std::end(overlay), it, std::begin(overlay), [&](BGRA8Pixel& pix, const uint16_t& gray) {
				return pix + U16ToBGRA8Histo(gray, col, ll, ul);
			} );
			*/
		}
	}
}

void ScopeOverlay::ToD2Bitmap(ID2D1Bitmap* const _d2bitmap) const {
	std::lock_guard<std::mutex> lock(mutex);
	FLOAT h = _d2bitmap->GetSize().height;
	FLOAT w = _d2bitmap->GetSize().width;
	assert( (_d2bitmap->GetSize().height == lines) && (_d2bitmap->GetSize().width == linewidth) );
	_d2bitmap->CopyFromMemory(&D2D1::RectU(0,0,linewidth,lines), overlay.data(), linewidth*4);			// stride is *4 because 4 bytes per pixel (BGRA)
}

void ScopeOverlay::Resize(const uint32_t& _lines, const uint32_t& _linewidth) {
	assert( _lines!=0 && _linewidth!=0 );
	std::lock_guard<std::mutex> lock(mutex);
	lines = _lines;
	linewidth = _linewidth;
	overlay.resize(lines*linewidth, BGRA8BLACK);
}

uint32_t ScopeOverlay::Lines() const {
	std::lock_guard<std::mutex> lock(mutex);
	return lines;
}

uint32_t ScopeOverlay::Linewidth() const {
	std::lock_guard<std::mutex> lock(mutex);
	return linewidth;
}

}