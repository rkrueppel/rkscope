#pragma once

#include "direct2d/D2ChannelRender.h"

namespace scope {
	namespace gui {

/** CChannelView is just a dumb holder for the renderer and takes care of window resizes etc */
class CChannelView 
	: public CWindowImpl<CChannelView> {

protected:
	/** the Direct2D renderer class */
	d2d::D2ChannelRender renderer;

public:
	BEGIN_MSG_MAP(CChannelView)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_PAINT(OnPaint)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MSG_WM_DISPLAYCHANGE(OnDisplayChange)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()

	/** Set background brush to -1, avoids erasure of background (similar effect as OnEraseBkgnd below...) */
	DECLARE_WND_CLASS_EX(nullptr, CS_HREDRAW | CS_VREDRAW, -1);

	/** @name Called via Win32 messages
	* @{ */
	/** Does nothing (-> disable default handling) */
	BOOL PreTranslateMessage(MSG* pMsg);

	/** Creates the renderer */
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	/** avoids flickering during resize */
	LRESULT OnEraseBkgnd(HDC wParam) { return true; }

	/** Calls renderer and validates rect */
	void OnPaint(CDCHandle /*dc*/);

	/** Calls renderer's resize */
	void OnSize(UINT /*type*/, CSize size);

	/** Handles WM_MOUSEMOVE messages, just passes them on to ChannelFrame (the parent)
	* via user-defined message WM_UPDATEMOUSEPIXEL */
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/** Calls renderer */
	void OnDisplayChange(UINT /*bpp*/, CSize /*resolution*/);
	/** @} */

	/** @name Called from CChannelFrame
	* @{ */

	/** @return returns the current size of the renderer */
	D2D1_SIZE_F GetRendererSize() const;

	/** Calls renderer.Render */
	void Render();

	/** Resizes the bitmap of the renderer. Calls renderer.ResizeBitmap
	* @param[in] _xres,_yres new size for the bitmap */
	void ResizeContent(const uint32_t& _xres, const uint32_t& _yres);

	/** @return returns a pointer to the bitmap inside the renderer */
	ID2D1Bitmap* GetBitmap();

	/** Update the scale text. Calls d2d::D2ChannelRender::UpdateScaleText */
	virtual void UpdateScaleText(const std::wstring& _text);
	/** @} */
};

}}
