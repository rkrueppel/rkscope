#include "StdAfx.h"
#include "LogFrame.h"

namespace scope {
	namespace gui {

void CLogFrame::OnFinalMessage(HWND /*hWnd*/) {
	delete this;
}

CLogFrame::~CLogFrame() {
	// If we land here through an exception, OnDestroy is eventually not called thus we need to detach here to avoid
	// an invalid pointer in the DisplayController
	if ( attached ) {
		scope_logger.DetachLogFrame();
		attached = false;
	}
}

int CLogFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	m_hWndClient = view.Create(m_hWnd, rcDefault);

	SetWindowText(L"Logbook");

	scope_logger.AttachLogFrame(this);
	attached = true;
	return 1;
}

void CLogFrame::OnDestroy() {
	// We have to detach here, because after OnDestroy the HWND is not valid anymore
	scope_logger.DetachLogFrame();
	attached = false;
}

BOOL CLogFrame::PreTranslateMessage(MSG* pMsg) {
	if(view.PreTranslateMessage(pMsg))
		return TRUE;

	return CFrameWindowImpl<CLogFrame>::PreTranslateMessage(pMsg);
}

void CLogFrame::AppendLogText(const std::wstring& _text) {
	LONG start, end;
	view.GetSel(start, end);
	view.AppendText(_text.c_str());
	view.SetSel(start, end);
}

void CLogFrame::ReplaceLogText(const std::wstring& _text) {
	LONG start, end;
	view.GetSel(start, end);
	view.SetWindowText(_text.c_str());
	// Keeps the caret position (SetWindowText resets it to the beginning)
	view.SetSel(start, end);
	view.Invalidate();
}

std::wstring CLogFrame::GetLogText() {
	std::size_t textlen = view.GetWindowTextLength()+1;			// +1 since terminating \0 is not counted
	std::vector<wchar_t> buffer(textlen, L' ');
	view.GetWindowText(buffer.data(), textlen);					// empty control gives only a \0
	return std::wstring(buffer.begin(), buffer.end()-1);
}

}}