#pragma once

#include "d2wrap.h"

namespace d2d {

/** Handles all Direct2D rendering in a CChannelView.
* Renders and resizes a bitmap. All mutex protected. */
class D2ChannelRender {

protected:
	/** mutex for protection */
	mutable std::mutex mutex;
	
	/** Direct2D render target */
	std::unique_ptr<RenderTarget> render_target;
	
	/** Direct2D bitmap (do not use a unique_ptr for COM interfaces) */
	ID2D1Bitmap* bitmap;
	
	/** Direct2D brush for text (do not use a unique_ptr for COM interfaces) */
	ID2D1SolidColorBrush* text_brush;
	
	/** Direct2D text format (do not use a unique_ptr for COM interfaces) */
	IDWriteTextFormat* text_format;
	
	/** Window handle */
	HWND hwnd;

	/** String with the scale information, ala L"9.5 µm" */
	std::wstring scaletext;

protected:
	/** Safely release all Direct2D resources */
	void DiscardDeviceResources();

public:
	D2ChannelRender();

	/** Discard resources and delete the render_target */
	~D2ChannelRender();

	/** Creates the render target for a hwnd, a bitmap with the desired resolution, a brushes for painting
	* , and a text format for µm text */
	virtual void Create(const HWND& _hwnd, const uint32_t& _xres, const uint32_t& _yres);

	/** Renders the bitmap and the scale text */
	virtual void Render();

	/** @return pointer to current Direct2D bitmap */
	ID2D1Bitmap* Bitmap() const { return bitmap; }

	/** Resizes the bitmap
	* @param[in] _xres,_yres new size
	* @return true if successful */
	bool ResizeBitmap(const uint32_t& _xres, const uint32_t& _yres);

	/** Draws current bitmap in render target */
	void DrawBitmap() { render_target->DrawBitmap(bitmap); }

	/** Resizes the render target
	* @param[in] _xres,_yres new size
	* @return true if successful */
	virtual bool Resize(const uint32_t& _xres, const uint32_t& _yres);

	/** @return current size of render target */
	D2D1_SIZE_F GetSize(void) const { return render_target->GetSize(); }

	/** Update the scale text */
	virtual void UpdateScaleText(const std::wstring& _text);
};

/** Shared pointer to a D2ChannelRender */
typedef std::shared_ptr<D2ChannelRender> D2ChannelRenderPtr;

}