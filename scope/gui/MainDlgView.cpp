#include "stdafx.h"
#include "MainDlgView.h"

namespace scope {
	namespace gui {

CMainDlgView::CMainDlgView(scope::RunButtons& _runbuttons)
	: m_ScanSingleButton(_runbuttons.startsingle)
	, m_ScanLiveButton(_runbuttons.startlive)
	, m_StopButton(_runbuttons.stop)
	, m_QuitButton(_runbuttons.quit)
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

void CMainDlgView::ChangeScanMode(const uint32_t& _area, const ScannerVectorType& _mode) {
	m_sheetExperimentSettings.ChangeScanMode(_area, _mode);
}

	}}