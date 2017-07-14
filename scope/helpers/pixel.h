#pragma once

/** @file pixel.h The BGRA8Pixel struct and various related helpers for color conversions. */

namespace scope {

/** converts 16 bit value to 8 bit value */
inline uint8_t to8(const uint16_t& shortval) {
	return static_cast<uint8_t>(shortval >> 8);
}

/** 16 bit to 8 bit (with lower and upper bound: s <= l -> 0, s >= u -> 255 */
inline uint8_t to8hist(const uint16_t& s, const uint16_t& l, const uint16_t& u) {
	return ( (s <= l) ? 0 : (s >= u)  ? 255 : to8(s) );
}

/** 16 bit to 8 bit (with lower and upper bound: s <= l -> 0, s >= u -> 255 */
inline uint8_t to8hist_mod(const uint16_t& s, const uint16_t& l, const uint16_t& u) {
	if ( s <= l )
		return 0;
	if ( s >= u )
		return UINT8_MAX;
	uint8_t ranged = (uint8_t)( (s-l) * 255 / (u-l));
	return ranged;
}

/** 16 bit to 8 bit (with lower and upper bound: s <= l -> 0, s >= u -> 255 */
inline uint8_t to8hist_mod2(const uint16_t& s, const uint16_t& l, const uint16_t& u, const uint16_t& scaler) {
	if ( s <= l )
		return 0;
	if ( s >= u )
		return UINT8_MAX;
	uint8_t ranged = (uint8_t)( (s-l) * scaler);
	return ranged;
}

/** Encapsulated a 4-byte pixel in BGRA format for use with Direct2D */
struct BGRA8Pixel {
	/** blue value */
	uint8_t B;

	/** green value */
	uint8_t G;

	/** red value */
	uint8_t R;

	/** alpha value */
	uint8_t A;
	
	/** Initialize to opaque black */
	BGRA8Pixel()
		: B(0)
		, G(0)
		, R(0)
		, A(255)
	{}

	/** Sets all colors to val (=> gray) and alpha to opaque */
	BGRA8Pixel(const uint8_t& val)
		: B(val)
		, G(val)
		, R(val)
		, A(255)
	{}

	/** Sets all colors as desired */
	BGRA8Pixel(const uint8_t& vB, const uint8_t& vG, const uint8_t& vR)
		: B(vB)
		, G(vG)
		, R(vR)
		, A(255)
	{}

	/** Sets all colors to val => gray and alpha to opaque */
	BGRA8Pixel& operator= (const uint8_t& val) {
		B = G = R = val;		
		A = 255;
		return *this;
	}

	/** satured addition (evtl do with MMX / 8 pixel at the same time because 64bit?!) */
	BGRA8Pixel operator+ (const BGRA8Pixel& adder) {
		return BGRA8Pixel((B > (0xFF - adder.B)) ? 0xFF : B + adder.B
			, (G > (0xFF - adder.G)) ? 0xFF : G + adder.G
			, (R > (0xFF - adder.R)) ? 0xFF : R + adder.R);
	}

	/** satured addition (evtl do with MMX / 8 pixel at the same time because 64bit?!) */
	inline BGRA8Pixel& operator+= (const BGRA8Pixel& adder) {
		B = (B > (0xFF - adder.B)) ? 0xFF : B + adder.B;
		G = (G > (0xFF - adder.G)) ? 0xFF : G + adder.G;
		R = (R > (0xFF - adder.R)) ? 0xFF : R + adder.R;
		return *this;
	}

	/** @name Accessor methods to the color components */
	/** @{ */
	inline void to_blue(const uint8_t& val) { B = val; }
	inline void to_green(const uint8_t& val) { G = val; }
	inline void to_red(const uint8_t& val) { R = val; }
	inline void to_gray(const uint8_t& val) { B = G = R = val; }
	/** @} */
};

}
