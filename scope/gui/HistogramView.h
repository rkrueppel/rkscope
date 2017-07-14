#pragma once

#include "direct2d/D2HistogramRender.h"
#include "controllers/ScopeController.h"
#include "controls/ScopeButtonCtrl.h"
#include "ScopeDatatypes.h"
#include "resource.h"

// Forward declarations
namespace scope {
class SCOPE_MULTIIMAGE_T;
typedef std::shared_ptr<const SCOPE_MULTIIMAGE_T> SCOPE_MULTIIMAGECPTR_T;
class ScopeMultiHistogram;
typedef std::shared_ptr<ScopeMultiHistogram> ScopeMultiHistogramPtr;
}

namespace scope {
	namespace gui {

/** CHistogramView is a holder for the renderer, calculates histograms, and takes care of window resizes and mouse/limit bar movements */
class CHistogramView
	: public CWindowImpl<CHistogramView> {

protected:
	/** static constant width of the histogram renderer */
	static const uint32_t width = 512;

	/** for which area */
	const uint32_t area;

	/** how many channels */
	const uint32_t channels;

	/** what uin16_t histogram range is used */
	uint16_t range;

	/** the ScopeController kept handy here */
	ScopeController scope_controller;

	/** image corresponding to the currently displayed histogram */
	SCOPE_MULTIIMAGECPTR_T current_frame;

	/** currently displayed histogram */
	ScopeMultiHistogramPtr current_histogram;

	/** lower limit values of all channels/the value that corresponds to histogram array index (because of range) */
	std::vector<uint16_t> lower_limits;

	/** upper limit values of all channels/the value that corresponds to histogram array index (because of range) */
	std::vector<uint16_t> upper_limits;

	/** this one handles all of the Views rendering */
	d2d::D2HistogramRender renderer;

	/** Keeps track of mouse dragging which lower limit */
	std::vector<bool> dragginglower;

	/** Keeps track of mouse dragging which upper limit */
	std::vector<bool> draggingupper;

	/** Keeps track of the lower limit positions in screen coordinates/histogram array index */
	std::vector<FLOAT> llpos;

	/** Keeps track of the upper limit positions in screen coordinates/histogram array index */
	std::vector<FLOAT> ulpos;

protected:
	/** Set limits in renderer and scope_controller */
	void SetLimits();

public:
	/** Initialize everything */
	CHistogramView(const uint32_t& _area, const uint32_t& _channels, uint16_t _range);

	/** Set background brush to -1, avoids erasure of background (similar effect as OnEraseBkgnd below...) */
	DECLARE_WND_CLASS_EX(nullptr, CS_HREDRAW | CS_VREDRAW, -1);

	BEGIN_MSG_MAP(CChannelView)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_DISPLAYCHANGE(OnDisplayChange)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL PreTranslateMessage(MSG* pMsg);

	/** Do not do 'delete this;' because view is a member of frame and gets destroyed when frame is destroyed */
	virtual void OnFinalMessage(HWND /*hWnd*/);

	/** Create the renderer only now, because now the window is shown */
	void OnShowWindow(BOOL bShow, UINT nStatus);

	/** Ask renderer for hit test and set click state */
	void OnLButtonDown(UINT nFlags, CPoint point);

	/** Releases click state */
	void OnLButtonUp(UINT nFlags, CPoint point);

	/** If moving with left mouse button down, give position to Renderer and render limit bars */
	void OnMouseMove(UINT nFlags, CPoint point);

	/** avoids flickering during resize */
	LRESULT OnEraseBkgnd(HDC wParam) { return true; }

	/** Render the histogram and the limit bars (both via Renderer) */
	void OnPaint(CDCHandle /*dc*/);

	/** Resize the Renderer accordingly, be careful not to resize to 0 */
	void OnSize(UINT /*type*/, CSize _size);

	void OnDisplayChange(UINT /*bpp*/, CSize /*resolution*/);
	/** @}

	/** @name Called by CHistogramFrame
	* @{ */
	/** Set lower limit to the first value with count >0 and the upper limit to
	* the last value with count >0. Optimizes dynamic range for display. */
	void Optimize();

	/** Set lower and upper limits to full range */
	void FullRange();
	
	/** Sets the current frame and calculates its histogram (either with regular count or with logarithmic counts) */
	void SetCurrentFrame(SCOPE_MULTIIMAGECPTR_T const _multi, const bool& _loghisto);

	/** Render histogram and limit bars */
	uint16_t LowerLimit(const uint32_t& _ch) const { return lower_limits[_ch]; }
	uint16_t UpperLimit(const uint32_t& _ch) const { return upper_limits[_ch]; }
	/** @} */

};

}}

