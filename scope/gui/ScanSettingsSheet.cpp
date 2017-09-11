#include "StdAfx.h"
#include "ScanSettingsSheet.h"
#include "NoScanBasePage.h"
#include "FrameScanSawPage.h"
#include "FrameScanBidiPage.h"
#include "FrameScanHopperPage.h"
#include "FrameScanResonancePage.h"
#include "FrameScanResonanceSlavePage.h"
#include "helpers/ScopeDatatypes.h"

namespace scope {
	namespace gui {

CScanSettingsSheet::CScanSettingsSheet(
	const uint32_t& _nareas
	, std::vector<parameters::Area>& _areaparamsvec
	, std::vector<FPUButtons>& _fpubuttonsvec
	, const double& _masterfovsizex
	, const double& _masterfovsizey
	, parameters::Storage& _storageparams
	, parameters::Stimulation& _stimulationparams
	, parameters::Stage& _stageparams
	, ZeroButtons& _zerobuttons
	, ScopeNumber<bool>& _readonlywhilescanning
)
	: nareas(_nareas)
	, areaparamsvec(_areaparamsvec)
	, fpubuttonsvec(_fpubuttonsvec)
	, readonlywhilescanning(_readonlywhilescanning)
	, storagesettingspage(_storageparams)
	, stimulationsettingspage(_stimulationparams)
	, movementpage(_areaparamsvec, _fpubuttonsvec, _masterfovsizex, _masterfovsizey, _stageparams, _zerobuttons)
	, inputsinfospage(dynamic_cast<parameters::SCOPE_INPUTS_PARAMETERS_T*>(_areaparamsvec[0].daq.inputs.get()))
{
	int32_t a = -1;
	std::generate_n(std::back_inserter(scanpages), nareas, [&a, &areaparamsvec]() {
		a++;
		if areaparamsvec[a].isslave()
			return std::make_unique<CNoScanBasePage>(a, areaparamsvec[a] , fpubuttonsvec[a]);
		else
			return std::make_unique<CFrameScanSawPage>(a, areaparamsvec[a] , fpubuttonsvec[a]);
	});
	
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

	for ( uint32_t a = 0 ; a < scanpages.size() ; a++ ) {
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
					scanpages[a] = std::make_unique<CFrameScanSawPage>(a, areaparamsvec[a] , fpubuttonsvec[a]);
					break;
				case ScannerVectorTypeHelper::Bidirectional:
					scanpages[a] = std::make_unique<CFrameScanBidiPage>(a, areaparamsvec[a] , fpubuttonsvec[a]);
					break;
				case ScannerVectorTypeHelper::Planehopper:
					scanpages[a] = std::make_unique<CFrameScanHopperPage>(a, areaparamsvec[a] , fpubuttonsvec[a]);
					break;
				case ScannerVectorTypeHelper::ResonanceBiDi:
					scanpages[a] = std::make_unique<CFrameScanResonancePage>(a, areaparamsvec[a] , fpubuttonsvec[a]);
					break;
				case ScannerVectorTypeHelper::ResonanceHopper:
					scanpages[a] = std::make_unique<CFrameScanResonancePage>(a, areaparamsvec[a] , fpubuttonsvec[a]);
					break;
				}
		//}
		InsertPage(a, *scanpages[a]);
	}
	SetActivePage(_area);
}

void CScanSettingsSheet::SetReadOnlyWhileScanning(const bool& _ro) {
	for(auto sp : scanpages)
		sp->SetReadOnlyWhileScanning(_ro);
}

}}