#pragma once

#include "LogView.h"

namespace scope {
	namespace gui {

/** Frame window class for the log */
class CLogFrame
	: public CFrameWindowImpl<CLogFrame> {

protected:
	/** the view class inside the frame */
	CLogView view;

	/** are we attached to the DisplayController? */
	bool attached;

	/** font for use inside the CLogView */
	CFont font;

public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_LOGBOOK)

	~CLogFrame();

	BEGIN_MSG_MAP(CLogFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		CHAIN_MSG_MAP(CFrameWindowImpl<CLogFrame>)
		CHAIN_CLIENT_COMMANDS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	virtual void OnFinalMessage(HWND /*hWnd*/);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	BOOL PreTranslateMessage(MSG* pMsg);
	/** @} */

	/** @name Called from ScopeLogger
	* @{ */
	void AppendLogText(const std::wstring& _text);
	void ReplaceLogText(const std::wstring& _text);
	std::wstring GetLogText();
	/** @} */
};

}}

