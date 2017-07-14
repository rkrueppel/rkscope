#include "StdAfx.h"
#include "ScanSettingsSheet.h"
#include "NoScanBasePage.h"
#include "FrameScanSawPage.h"
#include "FrameScanBidiPage.h"
#include "FrameScanHopperPage.h"
#include "FrameScanResonancePage.h"
#include "FrameScanResonanceSlavePage.h"
#include "ScopeDatatypes.h"

namespace scope {
	namespace gui {

CScanSettingsSheet::CScanSettingsSheet()
	: inputsinfospage(dynamic_cast<parameters::SCOPE_INPUTS_PARAMETERS_T*>(scope_controller.GuiParameters.areas[0]->daq.inputs.get())){

	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		if ( scope_controller.GuiParameters.areas[a]->isslave() )
			scanpages[a] = std::unique_ptr<CNoScanBasePage>(new CNoScanBasePage(a, scope_controller.GuiParameters.areas[a]->Currentframe()));
			//	scanpages[a] = std::unique_ptr<CNoScanBasePage>(new CNoScanBasePage(a, scope_controller.GuiParameters.areas[a]->frameresonance));
			//else
		else
			scanpages[a] = std::unique_ptr<CFrameScanSawPage>(new CFrameScanSawPage(a));
	}
}

HWND CScanSettingsSheet::Create(const HWND hWndParent, const int nStartPage, const CRect & rc) {
	ATLASSERT(m_hWnd == NULL);

	for ( auto& fsp : scanpages )
		AddPage(*fsp);
	AddPage(storagesettingspage);
	AddPage(stimulationsettingspage);
	AddPage(movementpage);
	AddPage(inputsinfospage);

	m_psh.dwFlags	    = PSH_NOAPPLYNOW | PSH_MODELESS | PSH_USECALLBACK;
	m_psh.hwndParent	= hWndParent;
	m_psh.phpage		= (HPROPSHEETPAGE*)m_arrPages.GetData();
	m_psh.nPages		= m_arrPages.GetSize();
	m_psh.pfnCallback	= CScanSettingsSheet::PropSheetCallback;
		
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

	// Register the callback that is called by ScopeController upon scan mode changes
	std::function<void(const uint32_t&, const ScannerVectorType&)> cb(std::bind(&CScanSettingsSheet::ChangeScanmode, this, std::placeholders::_1, std::placeholders::_2));
	scope_controller.RegisterScanmodeCallback(cb);

	return hWnd;
}

int CALLBACK CScanSettingsSheet::PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam) {
	int nRetVal = 0;

	if (uMsg == PSCB_INITIALIZED) {		
		ATLASSERT(hWnd != NULL);
		CScanSettingsSheet* pT = (CScanSettingsSheet*)_Module.ExtractCreateWndData();		
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

LRESULT CScanSettingsSheet::OnSelChange(WPARAM wParam, LPNMHDR pnmHdr, BOOL & bHandled) {
	return  DefWindowProc(WM_NOTIFY, wParam, (LPARAM)pnmHdr);
}

void CScanSettingsSheet::ChangeScanmode(const uint32_t& _area, const ScannerVectorType& _type) {

	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		RemovePage(a);
		// Do NOT use different dialog sheet for Resonance Scanner slave area! (Master/Slave is handled  inside CFrameScanResonancePage)
		// If slave area, use different dialog sheet for Resonance scanner
		/*if(scope_controller.GuiParameters.areas[a]->isslave()) {
			if((_type.t == ScannerVectorTypeHelper::ResonanceBiDi) || (_type.t == ScannerVectorTypeHelper::ResonanceHopper))
				scanpages[a].reset(new CFrameScanResonanceSlavePage(a));
			else {
				//if ( SCOPE_USE_RESONANCESCANNER )
				//	scanpages[a].reset(new CNoScanBasePage(a, scope_controller.GuiParameters.areas[a]->frameresonance));
				//else
					scanpages[a].reset(new CNoScanBasePage(a, scope_controller.GuiParameters.areas[a]->FrameSaw()));
			}
		}*/
		//else { // for Master area
			switch( _type.t ) {
				case ScannerVectorTypeHelper::Sawtooth:
					scanpages[a].reset(new CFrameScanSawPage(a));
					break;
				case ScannerVectorTypeHelper::Bidirectional:
					scanpages[a].reset(new CFrameScanBidiPage(a));
					break;
				case ScannerVectorTypeHelper::Planehopper:
					scanpages[a].reset(new CFrameScanHopperPage(a));
					break;
				case ScannerVectorTypeHelper::ResonanceBiDi:
					scanpages[a].reset(new CFrameScanResonancePage(a));
					break;
				case ScannerVectorTypeHelper::ResonanceHopper:
					scanpages[a].reset(new CFrameScanResonancePage(a));
					break;
				}
		//}
		InsertPage(a, *scanpages[a]);
	}
	SetActivePage(_area);
}

}}