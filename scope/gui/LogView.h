#pragma once

#include "resource.h"

namespace scope {
	namespace gui {

		/** Traits for this window are standard CControlWinTraits but without WS_HSCROLL => automatic word wrap */
		typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL, 0> CLogViewTraits;

		/** View class for the log. Provides saving of the log via rich edit control EditStreamCallback function. */
		class CLogView
			: public CWindowImpl<CLogView, CRichEditCtrl, CLogViewTraits>
			, public CRichEditCommands<CLogView> {

		public:
			DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

			BEGIN_MSG_MAP(CLogView)
				MSG_WM_CREATE(OnCreate)
				MESSAGE_HANDLER(WM_CHAR, OnChar)
				CHAIN_MSG_MAP_ALT(CRichEditCommands<CLogView>, 1)
			END_MSG_MAP()

			/** @name Called via Win32 messages
			* @{ */
			BOOL PreTranslateMessage(MSG* pMsg);
			int OnCreate(LPCREATESTRUCT lpCreateStruct);
			LRESULT OnChar(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled);
			/** @} */

			/** Saves the logwindow text into a file */
			BOOL SaveFile(LPTSTR lpstrFilePath);

			/** Callback for actual writing to disk. This is not yet implemented. Writing is currently done in ScopeLoggerImpl */
			static DWORD CALLBACK StreamWriteCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb);
		};

	}
}