#pragma once

#include "pixel.h"

// Forward declarations
struct ID2D1Bitmap;
namespace scope {
class ColorProps;
class ScopeMultiImage;
typedef std::shared_ptr<const ScopeMultiImage> ScopeMultiImageCPtr;
}

namespace scope {

/** Overlay of several gray-scale/uint16_t channels into one BGRA8 image.
* Thread-safe. */
class ScopeOverlay {

protected:
	/** number of lines, y resolution */
	uint32_t lines;

	/** width of a line, x resolution */
	uint32_t linewidth;

	/** vector with pixeldata */
	std::vector<BGRA8Pixel> overlay;

	/** mutex for protection */
	mutable std::mutex mutex;

public:
	/** overlay will be initialized with 0s
	* @param[in] _lines initial y resolution
	* @param[in] _linewidth initial x resolution */
	ScopeOverlay(const uint32_t& _lines = 0, const uint32_t& _linewidth = 0);

	/** Creates an overlay from a multi image with the specified color properties per channel
	* @param[in] _multi the multi image to create overlay from
	* @param[in] _color_props the vector with the ColorProps for each channel */
	virtual void Create(ScopeMultiImageCPtr const _multi, const std::vector<ColorProps>& _color_props);

	/** @param[in] _d2bitmap pointer to the Direct2D bitmap to fill with the overlay */
	void ToD2Bitmap(ID2D1Bitmap* const _d2bitmap) const;

	/** @param[in] _lines,_linewidth new size */
	void Resize(const uint32_t& _lines, const uint32_t& _linewidth);

	/** @return lines/yres in overlay */
	uint32_t Lines() const;

	/** @return linewidth/xres in overlay */
	uint32_t Linewidth() const;
};

}