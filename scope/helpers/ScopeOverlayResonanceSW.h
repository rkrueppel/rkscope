#pragma once
#include "ScopeOverlay.h"

// Forward declarations
namespace scope {
	class ScopeMultiImageResonanceSW;
	typedef std::shared_ptr<const ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWCPtr;
}

namespace scope {

/** Overlay of several gray-scale/uint16_t channels into one BGRA8 image. Includes additional code to show only forward lines of a software mapped
* ScopeMultiImageResonanceSW. Thread-safe. */
class ScopeOverlayResonanceSW :
	public ScopeOverlay {

public:
	/** overlay will be initialized with 0s
	* @param[in] _lines initial y resolution
	* @param[in] _linewidth initial x resolution */
	ScopeOverlayResonanceSW(const uint32_t& _lines = 0, const uint32_t& _linewidth = 0);

	/** Creates an overlay from a multi image with the specified color properties per channel
	* @param[in] _multi the resonance sw mapped multi image to create overlay from
	* @param[in] _color_props the vector with the ColorProps for each channel */
	virtual void Create(ScopeMultiImageResonanceSWCPtr const _multi, const std::vector<ColorProps>& _color_props);
};

}

