#include "StdAfx.h"
#include "ExperimentSettingsSheet.h"

namespace scope {
	namespace gui {
		
CExperimentSettingsSheet::CExperimentSettingsSheet(RunButtons& _runbuttons, ScopeControllerCounters& _scopecontrollercounters, parameters::Behavior& _behaviorparameters)
	: behaviorpage(_runbuttons, _scopecontrollercounters, _behaviorparameters)
{
	
}

HWND CExperimentSettingsSheet::Create(const HWND hWndParent, const int nStartPage, const CRect & rc) {
	ATLASSERT(m_hWnd == NULL);

	// Add pages to the property sheet 
	AddPage(pageScanModesSettings);
	AddPage(pageStackSettings);
	AddPage(pageTimeSeriesSettings);
	AddPage(pageBehaviorSettings);

	// and do some magic to make it work
	m_psh.dwFlags	    = PSH_NOAPPLYNOW | PSH_MODELESS | PSH_USECALLBACK;
	m_psh.hwndParent	= hWndParent;
	m_psh.phpage		= (HPROPSHEETPAGE*)m_arrPages.GetData();
	m_psh.nPages		= m_arrPages.GetSize();
	m_psh.pfnCallback	= CExperimentSettingsSheet::PropSheetCallback;
		
	_Module.AddCreateWndData(&m_thunk.cd, this);

	HWND hWnd = (HWND)::PropertySheet(&m_psh);
	_CleanUpPages();	// ensure clean-up, required if call failed
	
	ATLASSERT(m_hWnd == hWnd);

	if ( hWnd ) {
		int nAdjX = GetSystemMetrics(SM_CXDLGFRAME) * 2;
		int nAdjY = GetSystemMetrics(SM_CYDLGFRAME) * 2;
		SetWindowPos(NULL, rc.left - nAdjX, rc.top - nAdjY, rc.Width(), rc.Height(), 
			SWP_NOZORDER|SWP_NOACTIVATE);	
	}

	SetActivePage(nStartPage);
	return hWnd;
}

int CALLBACK CExperimentSettingsSheet::PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam) {	
	int nRetVal = 0;

	if (uMsg == PSCB_INITIALIZED) {		
		ATLASSERT(hWnd != NULL);
		CExperimentSettingsSheet* pT = (CExperimentSettingsSheet*)_Module.ExtractCreateWndData();		
		pT->SubclassWindow(hWnd);	// subclass the sheet window		
		pT->_CleanUpPages();		// remove page handles array
	}
	else if (uMsg == PSCB_PRECREATE) {
		LPDLGTEMPLATE pTemplate = (LPDLGTEMPLATE)lParam;
		ATLASSERT(pTemplate);
		
		DWORD dwRemove	= WS_POPUP|WS_SYSMENU|WS_CAPTION|DS_MODALFRAME;
		DWORD dwAdd		= WS_CHILD|WS_VISIBLE|WS_TABSTOP|DS_CONTEXTHELP|DS_3DLOOK|DS_CONTROL;

		pTemplate->style			 = (pTemplate->style & ~dwRemove) | dwAdd;
		pTemplate->dwExtendedStyle	|= WS_EX_CONTROLPARENT;		
		nRetVal = 1;
	}
		
	return nRetVal;
}	

LRESULT CExperimentSettingsSheet::OnSelChange(WPARAM wParam, LPNMHDR pnmHdr, BOOL & bHandled) {
	return DefWindowProc(WM_NOTIFY, wParam, (LPARAM)pnmHdr);
}

}}