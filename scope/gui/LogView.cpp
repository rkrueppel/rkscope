#include "StdAfx.h"
#include "LogView.h"

namespace scope {
	namespace gui {

BOOL CLogView::PreTranslateMessage(MSG* pMsg) {
	pMsg;
	return FALSE;
}

int CLogView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFont font;
	CLogFont lf;
	lf.lfWeight = FW_NORMAL;
	lf.SetHeight(11);
	SecureHelper::strcpy_x(lf.lfFaceName, LF_FACESIZE, L"Calibri");
	font.CreateFontIndirect(&lf);
	SetFont(font);
	LimitText(128000);

	return DefWindowProc();			// Complete creation...
}

LRESULT CLogView::OnChar(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled) {
	// ignore without a beep
	switch (wParam) {
	case '\t': // tab
	case '\r': //Carriage return
		scope_logger.GetUserLoggings();
		return 0;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

BOOL CLogView::SaveFile(LPTSTR lpstrFilePath) {
	_ASSERTE(lpstrFilePath != NULL);

	HANDLE hFile = ::CreateFile(lpstrFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	EDITSTREAM es;
	es.dwCookie = (DWORD)hFile;
	es.dwError = 0;
	es.pfnCallback = StreamWriteCallback;
	StreamOut(SF_TEXT, es);

	::CloseHandle(hFile);

	return !(BOOL)es.dwError;
}

DWORD CALLBACK CLogView::StreamWriteCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb) {
	_ASSERTE(dwCookie != 0);
	_ASSERTE(pcb != NULL);

	return !::WriteFile((HANDLE)dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL);
}

}}
