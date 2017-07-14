#include "stdafx.h"
#include "MainDlgView.h"

namespace scope {
	namespace gui {

CMainDlgView::CMainDlgView()
	: m_ScanSingleButton(&scope_controller.StartSingleButton)
	, m_ScanLiveButton(&scope_controller.StartLiveButton)
	, m_StopButton(&scope_controller.StopButton)
	, m_QuitButton(&scope_controller.QuitButton)
{}

CMainDlgView::~CMainDlgView() {
	DBOUT(L"CMainDlgView::~CMainDlgView()\n");

}

BOOL CMainDlgView::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	// Replace placeholder with scan settings property sheet
	HWND hWndPlaceHolder = GetDlgItem(IDC_SCANSETTINGS_HOLDER);
	::ShowWindow(hWndPlaceHolder, FALSE);	
	CRect rc;	
	::GetWindowRect(hWndPlaceHolder, &rc);		
	ScreenToClient(&rc);
	m_sheetScanSettings.Create(m_hWnd, 0, rc);

	// Replace placeholder with experiment settings property sheet
	hWndPlaceHolder = GetDlgItem(IDC_EXPERIMENTSETTINGS_HOLDER);
	::ShowWindow(hWndPlaceHolder, FALSE);		
	::GetWindowRect(hWndPlaceHolder, &rc);		
	ScreenToClient(&rc);
	m_sheetExperimentSettings.Create(m_hWnd, 0, rc);

	// Attach buttons to controls
	m_ScanLiveButton.AttachToDlgItem(GetDlgItem(ID_SCAN_CONT));
	m_ScanSingleButton.AttachToDlgItem(GetDlgItem(IDC_SCAN_SINGLE));
	m_StopButton.AttachToDlgItem(GetDlgItem(ID_STOP));
	m_QuitButton.AttachToDlgItem(GetDlgItem(IDC_QUIT));

	return 0;
}

BOOL CMainDlgView::PreTranslateMessage(MSG* pMsg) {
	pMsg;
	return FALSE;
}

	}}