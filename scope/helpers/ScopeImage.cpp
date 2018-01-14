#include "stdafx.h"
#include "ScopeImage.h"

namespace scope {

bool InsertPixels(ScopeImageU16Ptr const _img, std::vector<uint16_t>::iterator& _where, const DaqChunk<uint16_t>::dataiterator& _from, const DaqChunk<uint16_t>::dataiterator& _to) {
	bool retval = false;
	// Important: this locks the ScopeImage mutex for write access to the data !!
	ScopeImageAccessU16 imagedata(*_img);

	assert(_from <= _to);												// both conditions should be taken care of by the pixelmapper
	assert(_where + std::distance(_from, _to) <= imagedata.GetData()->end() );			// do not insert over data end, otherwise exception by std::copy ?!

	// insert the pixels into ScopeImage and advance inserter
	if ( _from != _to )														// Check here, otherwise copy throws exception on empty range
		_where = std::copy( _from, _to, _where);

	if ( _where == imagedata.GetData()->end() )												// image completely filled
		retval = true;

	return retval;
}

bool InsertAndAveragePixels(ScopeImageU16Ptr const _img, std::vector<uint16_t>::iterator& _where, const DaqChunk<uint16_t>::dataiterator _from, const DaqChunk<uint16_t>::dataiterator _to, const uint16_t& _currentavgcount) {
	assert(_currentavgcount < UINT16_MAX>>1);			// since worst case daqch=65535, i=65535, _multiplier=65535 (=> 65535/2)

	bool retval = false;
	// Important: this locks the ScopeImage mutex for write access to the data !!
	ScopeImageAccessU16 imagedata(*_img);
	uint32_t divisor = 1;
	uint32_t halfdivisor = 1;
	uint32_t multiplier = 0;
	if ( _currentavgcount > 0 ) {					// currenavgcount = 0: first frame, multiply by 0 -> overwrite last image pixel (for running update of old pixels), see PipelineController
		multiplier = _currentavgcount;				// currenavgcount = 1: second frame, multiply by 1, divide by 2
		divisor = _currentavgcount + 1;				// etc etc
		halfdivisor = divisor >> 2;
	}

	uint32_t n = 0;
	// insert the pixels into ScopeImage and advance inserter
	_where = std::transform(_from, _to, _where, _where, [&](const uint16_t& daqch, const uint16_t& i) {
		n = (static_cast<uint32_t>(i) * multiplier) + static_cast<uint32_t>(daqch);
		return static_cast<uint16_t>(  n / divisor + ((n%divisor)>halfdivisor?1u:0u) ); } );					// check residual to fix integer division rounding error

	if ( _where == imagedata.GetData()->end() ) {				// image completely filled
		retval = true;
		_where = imagedata.GetData()->begin();					
	}

	return retval;
}



}