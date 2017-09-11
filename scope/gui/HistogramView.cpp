#include "StdAfx.h"
#include "HistogramView.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "helpers/ScopeMultiHistogram.h"
#include "controllers/ScopeLogger.h"

namespace scope {
	namespace gui {

CHistogramView::CHistogramView(const uint32_t& _area, const uint32_t& _channels, const uint16_t& _range)
	: area(_area)
	, channels(_channels)
	, range(_range)
	, current_frame(std::make_shared<scope::SCOPE_MULTIIMAGE_T>())
	, current_histogram(std::make_shared<scope::ScopeMultiHistogram>(area, channels, 512, range))
	, lower_limits(channels, 0)
	, upper_limits(channels, range)
	, renderer(width, channels, current_histogram)
	, dragginglower(channels, false)
	, draggingupper(channels, false)
	, llpos(channels, 0.0)
	, ulpos(channels, 511.0) {
}

void CHistogramView::OnFinalMessage(HWND /*hWnd*/) {
}

void CHistogramView::OnLButtonDown(UINT nFlags, CPoint point) {
	uint32_t ch = 0;
	bool uplo = false;
	if ( renderer.ClickedAt(point, ch, uplo) ) {
		if ( uplo ) {
			dragginglower.assign(channels, false);	// to be safe
			draggingupper[ch] = true;
		}
		else {
			draggingupper.assign(channels, false);
			dragginglower[ch] = true;
		}
	}
}

void CHistogramView::OnLButtonUp(UINT nFlags, CPoint point) {
	DBOUT(L"OnLButtonUp");
	draggingupper.assign(channels, false);
	dragginglower.assign(channels, false);
}

void CHistogramView::OnMouseMove(UINT nFlags, CPoint point) {
	if ( nFlags == MK_LBUTTON ) {
		for ( uint32_t c = 0 ; c < channels ; c++ ) {
			if ( dragginglower[c] ) {
				// make sure ul and ll do not swap
				llpos[c] = std::min(ulpos[c], static_cast<FLOAT>(point.x));
				// calculate limit value from window coordinate
				lower_limits[c] = round2ui16(llpos[c] / 512 * (range-0));
				renderer.SetLowerLimitPosition(c, llpos[c]);
			}
			if ( draggingupper[c] ) {
				ulpos[c] = std::max(llpos[c], static_cast<FLOAT>(point.x));
				upper_limits[c] = round2ui16(ulpos[c] / 512 * (range-0));
				renderer.SetUpperLimitPosition(c, ulpos[c]);
			}
			// Notify ScopeController of new limits, to adjust display in CChannelFrame etc.
			scope_controller.SetHistogramLimits(area, c, lower_limits[c], upper_limits[c]);
		}
		Invalidate();
		::SendMessage(GetParent().m_hWnd, WM_UPDATEHISTOLIMITS, NULL, NULL);
	}
}

void CHistogramView::OnShowWindow(BOOL bShow, UINT nStatus) {
	current_histogram->UpdateRange(range);
	renderer.Create(m_hWnd);
}

BOOL CHistogramView::PreTranslateMessage(MSG* pMsg) {
	pMsg;
	return false;
}

void CHistogramView::OnPaint(CDCHandle /*dc*/) {
	PAINTSTRUCT paint;
	ATLASSERT(BeginPaint(&paint));
	renderer.Render();
	EndPaint(&paint);
	ValidateRect(NULL);
}

void CHistogramView::OnSize(UINT /*type*/, CSize _size) {
	assert(_size.cx == 512);			// Histogram expects this!
	// maximum position to draw a line is size-1
	renderer.Size(_size);
}

void CHistogramView::OnDisplayChange(UINT /*bpp*/, CSize /*resolution*/) {
	Invalidate(false);			// Sends WM_PAINT
}

void CHistogramView::Optimize() {
	auto firsts = current_histogram->FirstCountPositions();
	auto lasts = current_histogram->LastCountPositions();
	for ( uint32_t c = 0 ; c < channels ; c++ ) {
		lower_limits[c] = firsts[c];
		upper_limits[c] = lasts[c];
		DBOUT(L"CHistogramView::Optimize Ch" << c << L" lower limit " << lower_limits[c] << L" upper limit " << upper_limits[c]);
	}
	SetLimits();
	Invalidate(false);		// Sends WM_PAINT
}

void CHistogramView::FullRange() {
	range = static_cast<uint16_t>(scope_controller.GuiParameters.areas[area]->histrange);
	for ( uint32_t c = 0 ; c < channels ; c++ ) {
		lower_limits[c] = 0;
		upper_limits[c] = range;
	}
	SetLimits();
	Invalidate(false);		// Sends WM_PAINT
}

void CHistogramView::SetLimits() {
	for ( uint32_t c = 0 ; c < channels ; c++ ) {
		// Calculate window coordinates of limit bars positions (be careful with data types here... do not use uint16_t for intermediate results!)
		llpos[c] = static_cast<float>(lower_limits[c]) / static_cast<FLOAT>((range-0)) * 512.0f;
		ulpos[c] = static_cast<float>(upper_limits[c]) / static_cast<FLOAT>((range-0)) * 512.0f;
		current_histogram->UpdateRange(range);

		// Hand these positions to the renderer
		renderer.SetLowerLimitPosition(c, llpos[c]);
		renderer.SetUpperLimitPosition(c, ulpos[c]);
		// Signals new limits to the ScopeController, to adjust display in CChannelFrame etc.
		scope_controller.SetHistogramLimits(area, c, lower_limits[c], upper_limits[c]);
	}
}

void CHistogramView::SetCurrentFrame(scope::SCOPE_MULTIIMAGECPTR_T const _multi, const bool& _loghist) {
	current_frame = _multi;
	current_histogram->Calculate(current_frame, _loghist);
	renderer.SetHistogram(current_histogram);
}

}}