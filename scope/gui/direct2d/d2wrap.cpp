#include "stdafx.h"
#include "d2wrap.h"
#include "helpers/hresult_exception.h"

namespace d2d {

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;

RenderTarget::RenderTarget(HWND _hwnd)
	: target(nullptr)
	, dwrite_factory(nullptr)
	, pixelformat(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)) {
	HResult hr;

	// Get the size of the HWND client rect and crate render target to fill it
	RECT rc;
	GetClientRect(_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU( rc.right - rc.left, rc.bottom - rc.top );

	// Create a Direct2D render target.
	const D2D1_RENDER_TARGET_PROPERTIES targetProperties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelformat);
	try {
		hr(__FUNCTION__) = factory.pd2d_factory->CreateHwndRenderTarget( targetProperties
			, D2D1::HwndRenderTargetProperties(_hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY)
			, &target);

		assert(target != nullptr);

		hr(__FUNCTION__) = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED
			, __uuidof(IDWriteFactory)
			, reinterpret_cast<IUnknown**>(&dwrite_factory));

		target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
	catch (const hresult_exception& e) {
		e;				// avoid warning C4101 in release build
		DBOUT(L"Exception in d2d::render_target::render_target, hresult" << e.hr);
	}
}

RenderTarget::~RenderTarget() {
	SafeRelease(&target);
	SafeRelease(&dwrite_factory);
}

HRESULT RenderTarget::CreateSolidColorBrush(ID2D1SolidColorBrush** _brush, const D2D1_COLOR_F& _color) {
	SafeRelease(_brush);
	return HResult(target->CreateSolidColorBrush(_color, _brush), __FUNCTION__);
}

HRESULT RenderTarget::CreateBitmap(ID2D1Bitmap** _bitmap, const uint32_t& _yres, const uint32_t& _xres) {
	HResult hr;
	// Release the old bitmap
	SafeRelease(_bitmap);

	// ... and create the new one
	try {
		FLOAT systemdpi_x = 96;
		FLOAT systemdpi_y = 96;
		factory.pd2d_factory->GetDesktopDpi(&systemdpi_x, &systemdpi_y);

		hr(__FUNCTION__) = target->CreateBitmap(
			D2D1::SizeU(_xres, _yres),
			NULL,
			_xres*4,							// 4 bytes per pixel (RGBA)
			// the following provokes an error for testing purposes
			//D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_UINT), systemdpi.x, systemdpi.y),
			D2D1::BitmapProperties(pixelformat, systemdpi_x, systemdpi_y),
			_bitmap );
	}
	catch (const hresult_exception& e) {
		e;				// avoid warning C4101 in release build
		DBOUT(L"Exception in d2d::render_target.CreateBitmap, hresult=" << e.hr);
	}
	return hr;
}

HRESULT RenderTarget::CreateTextFormat(IDWriteTextFormat** _text_format) {
	SafeRelease(_text_format);
	HResult hr(dwrite_factory->CreateTextFormat(
		L"Consolas",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		14.0f,
		L"en-us",
		_text_format ), __FUNCTION__);

	hr(__FUNCTION__) = (*_text_format)->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	return hr;
}

HRESULT RenderTarget::CreateRectangleGeometry(const D2D1_RECT_F& _rectangle, ID2D1RectangleGeometry **_rectangleGeometry) {
	SafeRelease(_rectangleGeometry);
	return HResult(factory.pd2d_factory->CreateRectangleGeometry(_rectangle, _rectangleGeometry), __FUNCTION__);
}

void RenderTarget::BeginDraw() {
	target->BeginDraw();
}

D2D1_WINDOW_STATE RenderTarget::CheckWindowState() {
	return target->CheckWindowState();
}

void RenderTarget::SetTransform(const D2D1_MATRIX_3X2_F& _transform) {
	target->SetTransform(_transform);
}

void RenderTarget::ClearWindow() {
	target->SetTransform(D2D1::Matrix3x2F::Identity());
	target->Clear(D2D1::ColorF(255.0f, 255.0f, 255.0f,0.f));
}

void RenderTarget::Clear(const D2D1_COLOR_F& _clearColor) {
	target->SetTransform(D2D1::Matrix3x2F::Identity());
	target->Clear(_clearColor);
}

void RenderTarget::DrawBitmap(ID2D1Bitmap* _bitmap, D2D1_BITMAP_INTERPOLATION_MODE _mode) {
	ATLASSERT(_bitmap != nullptr);
	auto size = target->GetSize();
	auto sizebitmap = _bitmap->GetSize();
	auto p = _bitmap->GetPixelSize();
	target->DrawBitmap(_bitmap, D2D1::RectF(0, 0, size.width, size.height), 1, _mode);
}

void RenderTarget::DrawLine(const D2D1_POINT_2F& _point0, const D2D1_POINT_2F& _point1, ID2D1Brush* _brush, const FLOAT& _strokeWidth) {
	target->DrawLine(_point0, _point1, _brush, _strokeWidth);
}

void RenderTarget::DrawText(const CString& _text, const D2D1_RECT_F* _layoutRect, IDWriteTextFormat* _text_format, ID2D1Brush* _brush) {
	target->DrawText(_text.GetString()
		, _text.GetLength()
		, _text_format
		, _layoutRect
		, _brush );
}

void RenderTarget::FillRectangle(const D2D1_RECT_F* _rect, ID2D1Brush* _brush) {
	target->FillRectangle(_rect, _brush);
}

D2D1_SIZE_F RenderTarget::GetSize() const {
	return target->GetSize();
}

HRESULT RenderTarget::EndDraw() {
	return HResult(target->EndDraw(),__FUNCTION__);
}

HRESULT RenderTarget::Resize(const D2D1_SIZE_U& _pixelSize) {
	return HResult(target->Resize(_pixelSize), __FUNCTION__);
}

HRESULT RenderTarget::Flush() {
	return HResult(target->Flush(), __FUNCTION__);
}

}