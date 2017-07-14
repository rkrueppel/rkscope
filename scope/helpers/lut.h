#pragma once

#include "pixel.h"

namespace scope {

/** @file lut.h
* following http://www.mochima.com/articles/LUT/lut_h.html */

/** a static lookup table with rainbow colors */
extern const std::array<BGRA8Pixel,256> RainbowTable;

/** class for generating a lookup table by providing a function to the constructor */
template<int32_t LBound = 0, int32_t UBound = 255, class TResult = uint8_t, class TArg = uint16_t>
class LUT_function {

protected:
	/** the lookup table */
	std::array<TResult, UBound - LBound + 1> lut_array;

	/** iterator to beginning of lookup table */
	typename std::array<TResult, UBound - LBound + 1>::iterator lut;

public:
	/** Fills lookup table with function f
	* @param[in] f function that takes a double returns looked-up value (TResult, usually uint8_t)
	* @param[in] coeff multiplies taken double */
	explicit LUT_function ( std::function<TResult(double)> f, const double& coeff = 1 ) {
		lut = lut_array.begin() + LBound;
		for ( uint32_t i = LBound ; i <= UBound ; ++i )
			*(lut+i) = f(i*coeff);
	}

	/** @return looked-up value for i */
	const TResult& operator() (TArg i) const {
		return *(lut+i);
	}
};

/** Enum for some Lookup tables */
enum ColorEnum { None, Gray, Red, Green, Blue, Yellow, Rainbow, Limits };

/** Array of CString with color names */
extern const std::array<CString, 8> gColorStrings;

/** Encapsulated lower and upper limit for range adjustment and color for displaying */
class ColorProps {

protected:
	/** for protection */
	mutable std::mutex mutex;

	/** the color map type */
	ColorEnum col;

	/** lower limit to display */
	uint16_t ll;

	/** upper limit to display */
	uint16_t ul;

protected:

public:
	/** NI-DAQ devices have range of +-x volts digitized to int16, we read this as uint16, thus only values from 0-32767 are used
	*	=> maximum pixel value is 65534 / 2 = 32767 */
	ColorProps(const ColorEnum& _col = None, const uint16_t& _ll = 0, const uint16_t& _ul = UINT16_MAX >> 1)
		: col(_col)
		, ll(_ll)
		, ul(_ul) {
	}

	/** Safe copy constructor */
	ColorProps(const ColorProps& cp) {
		col = cp.col;
		ll = cp.ll;
		ul = cp.ul;
	}

	/** Safe assignment */
	ColorProps& operator=(const ColorProps& cp) {
		col = cp.Color();
		ll = cp.LowerLimit();
		ul = cp.UpperLimit();
		return *this;
	}

	/** @name Several accessor methods
	* @{ */
	uint16_t Range() const { std::lock_guard<std::mutex> lock(mutex); return ul-ll; }
	ColorEnum Color() const { std::lock_guard<std::mutex> lock(mutex); return col; }
	uint16_t LowerLimit() const { std::lock_guard<std::mutex> lock(mutex); return ll; }
	uint16_t UpperLimit() const { std::lock_guard<std::mutex> lock(mutex); return ul; }
	/** @} */

	/** @name Several mutator methods
	* @{ */
	void SetColor(const ColorEnum& _col) { std::lock_guard<std::mutex> lock(mutex); col = _col; }
	void SetLowerLimit(const uint16_t& _ll) { std::lock_guard<std::mutex> lock(mutex); ll = _ll; }
	void SetUpperLimit(const uint16_t& _ul) { std::lock_guard<std::mutex> lock(mutex); ul = _ul; }
	/** @} */

	/** Converts class to enum (for the color type) */
	operator ColorEnum() const { std::lock_guard<std::mutex> lock(mutex); return Color(); }
};

/** An opaque black */
#define BGRA8BLACK BGRA8Pixel(0) 

/** An opaque white */
#define BGRA8WHITE BGRA8Pixel(255) 

/** Maps a U16 pixel to a BGRA8 pixel by using a color mapping */
inline BGRA8Pixel U16ToBGRA8(const uint16_t& gray, const ColorEnum& pixelcolor) {
	switch (pixelcolor) {
		case Gray:
			return BGRA8Pixel(to8(gray));
		case Blue:
			return BGRA8Pixel(to8(gray), 0U, 0U);
		case Green:
			return BGRA8Pixel(0U, to8(gray), 0U);
		case Red:
			return BGRA8Pixel(0U, 0U, to8(gray));
		case Yellow:
			return BGRA8Pixel(0U, to8(gray), to8(gray));
		case None:
			return BGRA8Pixel(0U);
		case Rainbow:
			return RainbowTable[to8(gray)];
		case Limits:
			return ( (to8(gray) == 0)
				? BGRA8Pixel(255,0,0) 
				: (to8(gray) == 255)
					? BGRA8Pixel(0,0,255) 
					: BGRA8Pixel(to8(gray))
				);
		default:
			return BGRA8Pixel(0U);
	}
}

/** Maps a U16 pixel to a BGRA8 pixel by using a color mapping and upper and lower bounds */
inline BGRA8Pixel U16ToBGRA8Histo(const uint16_t& gray, const ColorEnum& pixelcolor, const uint16_t& l, const uint16_t& u) {
	const uint8_t tmp8 = to8hist_mod(gray, l, u);
	switch (pixelcolor) {
		case Gray:
			return BGRA8Pixel(tmp8);
		case Blue:
			return BGRA8Pixel(tmp8, 0U, 0U);
		case Green:
			return BGRA8Pixel(0U, tmp8, 0U);
		case Red:
			return BGRA8Pixel(0U, 0U, tmp8);
		case Yellow:
			return BGRA8Pixel(0U, tmp8, tmp8);
		case None:
			return BGRA8Pixel(0U);
		case Rainbow:
			return RainbowTable[tmp8];
		case Limits:
			return ( (tmp8 == 0)
				? BGRA8Pixel(255,0,0) 
				: (tmp8 == 255)
					? BGRA8Pixel(0,0,255) 
					: BGRA8Pixel(tmp8)
			);
		default:
			return BGRA8Pixel(0U);
	}
}

/** Maps a U16 pixel to a BGRA8 pixel by using a color mapping and upper and lower bounds */
inline BGRA8Pixel U16ToBGRA8Histo2(const uint16_t& gray, const ColorEnum& pixelcolor, const uint16_t& l, const uint16_t& u, const uint16_t& scaler) {
	const uint8_t tmp8 = to8hist_mod2(gray, l, u, scaler);
	switch (pixelcolor) {
		case Gray:
			return BGRA8Pixel(tmp8);
		case Blue:
			return BGRA8Pixel(tmp8, 0U, 0U);
		case Green:
			return BGRA8Pixel(0U, tmp8, 0U);
		case Red:
			return BGRA8Pixel(0U, 0U, tmp8);
		case Yellow:
			return BGRA8Pixel(0U, tmp8, tmp8);
		case None:
			return BGRA8Pixel(0U);
		case Rainbow:
			return RainbowTable[tmp8];
		case Limits:
			return ( (tmp8 == 0)
				? BGRA8Pixel(255,0,0) 
				: (tmp8 == 255)
					? BGRA8Pixel(0,0,255) 
					: BGRA8Pixel(tmp8)
			);
		default:
			return BGRA8Pixel(0U);
	}
}

}