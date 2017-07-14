#include "StdAfx.h"
#include "D2HistogramRender.h"
#include "helpers/hresult_exception.h"
#include "helpers/ScopeMultiHistogram.h"

namespace d2d {

D2HistogramRender::D2HistogramRender(const uint32_t& _width, const uint32_t& _channels, scope::ScopeMultiHistogramPtr _hist)
	: width(_width)
	, channels(_channels)
	, render_target(nullptr)
	, brushwhite(nullptr)
	, brushred(nullptr)
	, brushyellow(nullptr)
	, hwnd(NULL)
	, llpos(channels, 0.0)
	, ulpos(channels, 512.0)
	, hist(_hist) {
}

D2HistogramRender::~D2HistogramRender(){
	DiscardDeviceResources();
}

void D2HistogramRender::DiscardDeviceResources() {
	SafeRelease(&brushwhite);
	SafeRelease(&brushred);
	SafeRelease(&brushyellow);
}

void D2HistogramRender::Create(const HWND& _hwnd) {
	std::lock_guard<std::mutex> lock(mutex);
	hwnd = _hwnd;
	
	// Ensure the client area/HWND has the width we need for the histogram (because RenderTarget uses GetClientRect)
	RECT rect;
	GetClientRect(hwnd, &rect);
	assert(( rect.right-rect.left) == width );
	
	render_target.reset(new RenderTarget(hwnd));
	render_target->CreateSolidColorBrush(&brushwhite, D2D1::ColorF(D2D1::ColorF::White));
	render_target->CreateSolidColorBrush(&brushred, D2D1::ColorF(D2D1::ColorF::Red));
	render_target->CreateSolidColorBrush(&brushyellow, D2D1::ColorF(D2D1::ColorF::Yellow));
	llpos.assign(channels, 0);
	// make sure we don't get in trouble when window has size 0
	ulpos.assign(channels, (render_target->GetSize().width==0)?1.0f:render_target->GetSize().width-1.0f);
}

void D2HistogramRender::SetHistogram(scope::ScopeMultiHistogramPtr _hist) {
	std::lock_guard<std::mutex> lock(mutex);
	hist = _hist;
}

void D2HistogramRender::Render() {
	std::lock_guard<std::mutex> lock(mutex);
	HResult hr;
	if ( 0 == (D2D1_WINDOW_STATE_OCCLUDED & render_target->CheckWindowState()) ) {
		render_target->BeginDraw();
		render_target->SetTransform(D2D1::Matrix3x2F::Identity());
		render_target->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		
		D2D1_SIZE_F size = render_target->GetSize();
		std::vector<uint32_t> maxvals(hist->MaxCounts());
		float channelheight = size.height/channels;
		float multiplier = 1;
		for ( uint32_t c = 0 ; c < channels ; c++ ) {
			// scale factor from counts to screen pixels, +1 for maxvals in case of 0
			multiplier = channelheight / (maxvals[c]+1.0f);
			// Get a pointer to that channels histogram, call lock that histogram
			const std::vector<uint32_t>* h = hist->GetHistConst(c);
			assert(h->size() >= size.width);
			// Draw histogram out of single lines
			for ( float pos = 0.0f ; pos < size.width ; pos += 1.0f ) {
				render_target->DrawLine( D2D1::Point2F(pos, channelheight*(c+1.0f))
					, D2D1::Point2F(pos, channelheight*(c+1.0f) - static_cast<float>(h->at(static_cast<size_t>(pos))) * multiplier)
					, brushwhite, 2 );
			}
			// Release the histogram lock of that channel
			hist->ReleaseHistConst(c);
			// Draw the limit lines, lower in red, upper in yellow
			render_target->DrawLine( D2D1::Point2F(ulpos[c], channelheight*c)
				, D2D1::Point2F(ulpos[c], channelheight*(c+1.0f))
				, brushyellow, 3);
			render_target->DrawLine( D2D1::Point2F(llpos[c], channelheight*c)
				, D2D1::Point2F(llpos[c], channelheight*(c+1.0f))
				, brushred, 3);
		}
		// Errors from non-hresult-returning function end up here
		hr(__FUNCTION__) = render_target->Flush();
		hr(__FUNCTION__) = render_target->EndDraw();
		// "A presentation error has occurred that may be recoverable. The caller needs to re-create the render target then attempt to render the frame again."
		if ((HRESULT)hr == D2DERR_RECREATE_TARGET) {
            hr(__FUNCTION__) = S_OK;
            DiscardDeviceResources();
        }
	}
}

bool D2HistogramRender::ClickedAt(const CPoint _clickpoint, uint32_t& _channel, bool& _uplo) {
	std::lock_guard<std::mutex> lock(mutex);		// be safe here
	ID2D1RectangleGeometry* hitTestlower = nullptr;
	ID2D1RectangleGeometry* hitTestupper = nullptr;
	FLOAT channelheight = render_target->GetSize().height/channels;
	BOOL hit = false;
	D2D1_POINT_2F point = D2D1::Point2F(static_cast<FLOAT>(_clickpoint.x), static_cast<FLOAT>(_clickpoint.y));
	for ( uint32_t c = 0 ; c < channels ; c++ ) {
		// Create rectangle around limit line (widht +/- 5 pixels)
		render_target->CreateRectangleGeometry(D2D1::RectF(llpos[c]-5.0f, channelheight*c
			, llpos[c]+5.0f, channelheight*(c+1.0f)), &hitTestlower);
		render_target->CreateRectangleGeometry(D2D1::RectF(ulpos[c]-5.0f, channelheight*c
			, ulpos[c]+5.0f, channelheight*(c+1.0f)), &hitTestupper);
		// Test hit on lower limit
		hitTestlower->FillContainsPoint(point, NULL, &hit);
		if ( hit ) {
			_channel = c;
			_uplo = false;
			break;
		}
		else {		// Avoid dragging both when (nearly) overlying
			// Test hit on upper limit
			hitTestupper->FillContainsPoint(point, NULL, &hit);
			if ( hit ) {
				_channel = c;
				_uplo = true;
				break;
			}
		}
	}
	SafeRelease(&hitTestlower);
	SafeRelease(&hitTestupper);
	return (hit!=0);
}

void D2HistogramRender::SetLowerLimitPosition(const uint32_t& _channel, const FLOAT& _pos) {
	std::lock_guard<std::mutex> lock(mutex);
	llpos[_channel] = _pos;
}

void D2HistogramRender::SetUpperLimitPosition(const uint32_t& _channel, const FLOAT& _pos) {
	std::lock_guard<std::mutex> lock(mutex);
	ulpos[_channel] = _pos;
}

void D2HistogramRender::Size(const CSize& size) {
	std::lock_guard<std::mutex> lock(mutex);
	if ( render_target != nullptr ) {
		DBOUT(L"New size x: " << size.cx << L" old size x: " << render_target->GetSize().width);
		// Fix size for calculations if it is zero (windows start with size zero!)
		FLOAT s = ((size.cx==0)?1.0f:static_cast<FLOAT>(size.cx));
		FLOAT r = ((render_target->GetSize().width==0)?1.0f:render_target->GetSize().width);
		// Recalculate the limit positions
		for ( FLOAT& p : llpos )
			p *= s / r;
		for ( FLOAT& p : ulpos )
			p *= s / r;
		render_target->Resize(D2D1::SizeU(size.cx, size.cy));
	}
}

}