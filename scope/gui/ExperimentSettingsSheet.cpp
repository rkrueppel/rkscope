#include "StdAfx.h"
#include "ExperimentSettingsSheet.h"

namespace scope {
	namespace gui {
		
		CExperimentSettingsSheet::CExperimentSettingsSheet(std::array<ScanModeButtons, config::nmasters>& _scanmodebuttonsvec
			, parameters::Stack& _stackparams
			, RunButtons& _runbuttons
			, StackButtons& _stackbuttons
			, ScopeCounters<config::nmasters>& _counters
			, parameters::Timeseries& _timeseriesparams
			, parameters::Behavior& _behaviorparams
			, config::XYZStageParametersType& _stageparams
			, std::vector<parameters::MasterArea>& _masterareas
			, std::vector<parameters::SlaveArea>& _slaveareas
		)
			: scanmode(_scanmodebuttonsvec)
			, stack(_stackparams, _runbuttons, _stackbuttons, _counters, _stageparams, _masterareas, _slaveareas)
			, timeseries(_timeseriesparams, _runbuttons, _counters, _masterareas, _slaveareas)
			, behavior(_runbuttons, _counters, _behaviorparams, _masterareas, _slaveareas)
		{
			
		}

		HWND CExperimentSettingsSheet::Create(const HWND hWndParent, const int nStartPage, const CRect & rc) {
			ATLASSERT(m_hWnd == NULL);

			// Add pages to the property sheet 
			AddPage(scanmode);
			AddPage(stack);
			AddPage(timeseries);
			AddPage(behavior);

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

		void CExperimentSettingsSheet::ChangeScanMode(const uint32_t& _area, const ScannerVectorType& _mode) {

		}

		void CExperimentSettingsSheet::SetReadOnlyWhileScanning(const bool& _ro) {
				stack.SetReadOnlyWhileScanning(_ro);
				scanmode.SetReadOnlyWhileScanning(_ro);
				timeseries.SetReadOnlyWhileScanning(_ro);
				behavior.SetReadOnlyWhileScanning(_ro);
		}

	}
}