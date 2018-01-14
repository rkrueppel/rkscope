#pragma once
#include "helpers/helpers.h"
#include "config\config_choices.h"

/** @file
* Parts of the code are modified from Microsoft's Direct2D nbody example: Copyright (c) Microsoft Corporation.  All rights reserved. 
* Parts of the code are modified from Microsoft's "Mouse movement example": http://msdn.microsoft.com/en-us/library/windows/desktop/gg153550(v=vs.85).aspx */

/** Wrappers around the Direct2D interface */
namespace d2d {

/** Helper class to convert from screen mouse coordinates to device independent pixels for Direct2D */
class DPIScale {
	/** x scale factor */
    static float scaleX;

	/** y scale factor */
    static float scaleY;

public:
	/** Initialize with desktop dpis */
    static void Initialize(ID2D1Factory *pFactory) {
		FLOAT X, Y;
		pFactory->GetDesktopDpi(&X,&Y);
		scaleX = X/96.0f;
        scaleY = Y/96.0f;
    }

	/** Convert screen pixels to device independent points */
    template <typename T>
    static D2D1_POINT_2F PixelsToDips(T x, T y) {
        return D2D1::Point2F(static_cast<float>(x) / scaleX, static_cast<float>(y) / scaleY);
    }
};


/** Simple wrapper around a Direct2D factory */
template <D2D1_FACTORY_TYPE policy_class>
class d2dfactory {
public:
	/** Direct2D factory (do not use a unique_ptr for COM interfaces) */
	ID2D1Factory* pd2d_factory;

	/** for error logging */
	HRESULT hr;

	/** Create the factory and initialize the global DPIScale object, if debug build activate Direct2D debug layer. */
	d2dfactory()
		: hr(0) {
		#ifdef _DEBUG
			if ( SCOPE_DIRECT2D_DEBUG ) {
				D2D1_FACTORY_OPTIONS options;
				options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
				D2D1CreateFactory(policy_class, options, &pd2d_factory);
			}
		#endif
		#ifndef _DEBUG
			D2D1CreateFactory(policy_class, &pd2d_factory);
		#endif
		DPIScale::Initialize(pd2d_factory);
	}

	/** Safely release the factory. */
	~d2dfactory() {
		SafeRelease(&pd2d_factory);
	}
};


/** Wrapper around a Direct2D render target and the underlying Direct2D factory and IDWriteFactory */
class RenderTarget {

protected:
	/** Direct2D factory */
	d2dfactory<D2D1_FACTORY_TYPE_MULTI_THREADED> factory;

	/** Direct2D render target (do not use a unique_ptr for COM interfaces) */
	ID2D1HwndRenderTarget* target;

	/** Direct2D write factory for text rendering (do not use a unique_ptr for COM interfaces) */
	IDWriteFactory* dwrite_factory;

	/** current pixel format */
	D2D1_PIXEL_FORMAT pixelformat;

protected:
	/** disable copy */
	RenderTarget(const RenderTarget& );

	/** disable assignment */
	RenderTarget& operator=(const RenderTarget& );

public:
	/** Create render target on window */
	RenderTarget(HWND _hwnd);

	/** Release resources */
	~RenderTarget();
	
	/** @name Methods for creation of resources 
	* @{ */
	/** Create a solid color brush */
	HRESULT CreateSolidColorBrush(ID2D1SolidColorBrush** _brush, const D2D1_COLOR_F& _color = D2D1::ColorF(D2D1::ColorF::White));

	/** Creates a new Direct2D bitmap
	* @param[out] _bitmap handle to the new bitmap
	* @param[in] _yres,_xres resolution of the bitmap */
	HRESULT CreateBitmap(ID2D1Bitmap** _bitmap, const uint32_t& _yres, const uint32_t& _xres);

	/** Create a text format */
	HRESULT CreateTextFormat(IDWriteTextFormat** _text_format);

	/** Create a rectangle geometry */
	HRESULT CreateRectangleGeometry(const D2D1_RECT_F& _rectangle, ID2D1RectangleGeometry** _rectangleGeometry);
	/** @} */

	/** @name Methods for drawing
	* @{ */
	/** Trigger Direct2D begin draw  */
	void BeginDraw();

	/** Set current transform matrix */
	void SetTransform(const D2D1_MATRIX_3X2_F& _transform);

	/** Clear the window with a certain color */
	void Clear(const D2D1_COLOR_F& _clearColor = D2D1::ColorF(D2D1::ColorF::White));

	/** Clear the window with black */
	void ClearWindow();

	/** Draws a Direct2D bitmap
	* @param[in] _bitmap pointer to bitmap
	* @param[in] _mode interpolation mode, possible values
	* - D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
	* - D2D1_BITMAP_INTERPOLATION_MODE_LINEAR */
	void DrawBitmap(ID2D1Bitmap* _bitmap, D2D1_BITMAP_INTERPOLATION_MODE _mode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

	/** Draw a line */
	void DrawLine(const D2D1_POINT_2F& _point0, const D2D1_POINT_2F& _point1, ID2D1Brush* _brush, const FLOAT& _strokeWidth);

	/** Draw text */
	void DrawText(const CString& _text, const D2D1_RECT_F* _layoutRect, IDWriteTextFormat* _text_format, ID2D1Brush* _brush);

	/** Fill a rectangle */
	void FillRectangle(const D2D1_RECT_F* _rect, ID2D1Brush* _brush);

	/** Flush the render target */
	HRESULT Flush();

	/** Trigger Direct2D end draw */
	HRESULT EndDraw();
	/** @} */

	/** Resize render target */
	HRESULT Resize(const D2D1_SIZE_U& _pixelSize);

	/** Check state of window, e.g. if it is occluded */
	D2D1_WINDOW_STATE CheckWindowState();

	/** Get size of render target */
	D2D1_SIZE_F GetSize() const;



};


}