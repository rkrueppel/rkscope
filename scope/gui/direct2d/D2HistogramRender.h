#pragma once

#include "d2wrap.h"

// Forward declarations
namespace scope {
class ScopeMultiHistogram;
typedef std::shared_ptr<ScopeMultiHistogram> ScopeMultiHistogramPtr;
}

namespace d2d {

/** Handles all Direct2D rendering in a CHistogramView.
* Renders a histogram made up of single lines. Its size is fixed to 'width'. Also renders and repositions limit lines.
Also can test for click hits on limit lines. All mutex protected. */
class D2HistogramRender {

protected:
	/** mutex for protection */
	mutable std::mutex mutex;

	/** constant width of the renderer */
	const uint32_t width;

	/** number of channels */
	const uint32_t channels;

	/** Direct2D render target */
	std::unique_ptr<RenderTarget> render_target;

	/** Direct2D white brush */
	ID2D1SolidColorBrush* brushwhite;

	/** Direct2D red brush */
	ID2D1SolidColorBrush* brushred;

	/** Direct2D yellow brush */
	ID2D1SolidColorBrush* brushyellow;

	/** windows handle */
	HWND hwnd;

	/** Keeps track of the lower limit positions in screen coordinates */
	std::vector<FLOAT> llpos;

	/** Keeps track of the upper limit positions in screen coordinates */
	std::vector<FLOAT> ulpos;

	/** Pointer to the currently rendered multi histogram */
	scope::ScopeMultiHistogramPtr hist;

protected:
	/** Safely release all Direct2D resources */
	void DiscardDeviceResources();

	/** Disable copy */
	D2HistogramRender(const D2HistogramRender&);

	/** Disable assignment */
	D2HistogramRender operator=(const D2HistogramRender&);

public:

public:
	/** Initialize all to nullptr */
	D2HistogramRender(const uint32_t& _width, const uint32_t& _channels, scope::ScopeMultiHistogramPtr _hist);

	/** Discard device resources */
	~D2HistogramRender();

	/** Creates the render target for a hwnd, the brushes for painting, and sets the limit position to the window edges */
	void Create(const HWND& _hwnd);

	/** Sets the pointer to the current multi histogram */
	void SetHistogram(scope::ScopeMultiHistogramPtr _hist);

	/** Draws the current multi histogram with the limit lines */
	void Render();

	/** Resizes the renderer and the limit positions */
	void Size(const CSize& size);

	/** Checks if click hit a limit (+-5 pixel) and if yes, on which channel and upper or lower limit line
	* @param[in] _clickpoint the point (in window coordinates) to test
	* @param[out] _channel histogram of which channel was hit
	* @param[out] _uplo if lower limit was hit false, if upper limit line was hit true
	* @return is the _clickpoint on (+-5 pixel) a limitline */
	bool ClickedAt(const CPoint _clickpoint, uint32_t& _channel, bool& _uplo);

	/** Set position of a lower limit line */
	void SetLowerLimitPosition(const uint32_t& _channel, const FLOAT& _pos);

	/** Set position of an upper limit line */
	void SetUpperLimitPosition(const uint32_t& _channel, const FLOAT& _pos);
};

/** Shared pointer to a D2HistogramRenderPtr */
typedef std::shared_ptr<D2HistogramRender> D2HistogramRenderPtr;

}