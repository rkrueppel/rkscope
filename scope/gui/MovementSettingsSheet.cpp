#include "StdAfx.h"
#include "MovementSettingsSheet.h"

namespace scope {
	namespace gui {

CMovementSettingsSheet::CMovementSettingsSheet(
	std::vector<parameters::Area>& _areaparamsvec
	, std::vector<FPUButtons>& _fpubuttonsvec
	, const double& _masterfovsizex
	, const double& _masterfovsizey
	, parameters::SCOPE_XYZCONTROL_T& _stageparams
	, ZeroButtons& _zerobuttons
)
	: xyzcontrolpage(_stageparams, _zerobuttons)
{
	for (uint32_t a = 0; a < _areaparamsvec.size() ; a++) 
		fpupages.emplace_back(_areaparamsvec[a++], _fpubuttonsvec, _masterfovsizex, _masterfovsizey);
	//std::generate_n(std::back_inserter(fpupages), _areaparamsvec.size(), [&]() {
	//	return CFPUControlPage(_areaparamsvec[a++], _fpubuttonsvec, _masterfovsizex, _masterfovsizey); } );
}

HWND CMovementSettingsSheet::Create(const HWND hWndParent, const int nStartPage, const CRect & rc) {
	ATLASSERT(m_hWnd == NULL);

	AddPage(xyzcontrolpage);
	for ( auto& fpucp : fpupages )
		AddPage(fpucp);

	m_psh.dwFlags	    = PSH_NOAPPLYNOW | PSH_MODELESS | PSH_USECALLBACK;
	m_psh.hwndParent	= hWndParent;
	m_psh.phpage		= (HPROPSHEETPAGE*)m_arrPages.GetData();
	m_psh.nPages		= m_arrPages.GetSize();
	m_psh.pfnCallback	= CMovementSettingsSheet::PropSheetCallback;
		
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

int CALLBACK CMovementSettingsSheet::PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam) {
	int nRetVal = 0;

	if (uMsg == PSCB_INITIALIZED) {		
		ATLASSERT(hWnd != NULL);
		CMovementSettingsSheet* pT = (CMovementSettingsSheet*)_Module.ExtractCreateWndData();		
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

LRESULT CMovementSettingsSheet::OnSelChange(WPARAM wParam, LPNMHDR pnmHdr, BOOL & bHandled) {
	return  DefWindowProc(WM_NOTIFY, wParam, (LPARAM)pnmHdr);
}

}}