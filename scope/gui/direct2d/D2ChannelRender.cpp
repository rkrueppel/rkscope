#include "StdAfx.h"
#include "D2ChannelRender.h"
#include "helpers/hresult_exception.h"

namespace d2d {

D2ChannelRender::D2ChannelRender()
	: render_target(nullptr)
	, bitmap(nullptr)
	, text_brush(nullptr)
	, text_format(nullptr)
	, hwnd(NULL)
	, scaletext(L"__ µm x __ µm") {
}

D2ChannelRender::~D2ChannelRender() {
	DiscardDeviceResources();
}

void D2ChannelRender::DiscardDeviceResources() {
	SafeRelease(&bitmap);
	SafeRelease(&text_brush);
	SafeRelease(&text_format);
}

void D2ChannelRender::Create(const HWND& _hwnd, const uint32_t& _xres, const uint32_t& _yres) {
	std::lock_guard<std::mutex> lock(mutex);
	hwnd = _hwnd;
	render_target.reset(new RenderTarget(hwnd));
	HResult hr;
	hr(__FUNCTION__) = render_target->CreateBitmap(&bitmap, _yres, _xres);
	hr(__FUNCTION__) = render_target->CreateSolidColorBrush(&text_brush);
	hr(__FUNCTION__) = render_target->CreateTextFormat(&text_format);
}

bool D2ChannelRender::Resize(const uint32_t& _xres, const uint32_t& _yres) {
	std::lock_guard<std::mutex> lock(mutex);
	HResult hr(render_target->Resize(D2D1::SizeU(_xres, _yres)), __FUNCTION__);
	return SUCCEEDED(hr);
}

bool D2ChannelRender::ResizeBitmap(const uint32_t& _xres, const uint32_t& _yres) {
	std::lock_guard<std::mutex> lock(mutex);
	SafeRelease(&bitmap);
	HResult hr(render_target->CreateBitmap(&bitmap, _yres, _xres), __FUNCTION__);
	return SUCCEEDED(hr);
}

void D2ChannelRender::Render() {
	HResult hr;
	std::lock_guard<std::mutex> lock(mutex);
	// Do not draw if window not visible
	if ( 0 == (D2D1_WINDOW_STATE_OCCLUDED & render_target->CheckWindowState()) ) {
		render_target->BeginDraw();
		render_target->SetTransform(D2D1::Matrix3x2F::Identity());
		render_target->Clear(D2D1::ColorF(D2D1::ColorF::Red));

		render_target->DrawBitmap(bitmap);
		render_target->DrawText(scaletext.c_str()
			, &D2D1::RectF(0, 0, render_target->GetSize().width, render_target->GetSize().height)
			, text_format
			, text_brush
			);
		hr(__FUNCTION__) = render_target->Flush();
		hr(__FUNCTION__) = render_target->EndDraw();
		if ((HRESULT)hr == D2DERR_RECREATE_TARGET) {
            hr(__FUNCTION__) = S_OK;
            DiscardDeviceResources();
        }
	}
}

void D2ChannelRender::UpdateScaleText(const std::wstring& _text) {
	std::lock_guard<std::mutex> lock(mutex);
	scaletext = _text;
}


}