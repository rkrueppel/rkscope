#include "stdafx.h"
#include "MainDlgView.h"

namespace scope {
	namespace gui {

		CMainDlgView::CMainDlgView(RunButtons& _runbuttons
				, std::vector<FPUButtons>& _fpubuttons
				, parameters::Scope& _guiparameters
				, std::vector<ScanModeButtons>& _scanmodebuttons
				, StackButtons& _stackbuttons
				, ZeroButtons& _zerobuttons
				, ScopeCounters& _counters
		)
			: m_sheetScanSettings(_guiparameters.allareas, _fpubuttons, _guiparameters.masterfovsizex(), _guiparameters.masterfovsizey(), _guiparameters.storage
				, _guiparameters.stimulation, _guiparameters.stage, _zerobuttons)
			, m_sheetExperimentSettings(_scanmodebuttons, _guiparameters.stack, _runbuttons, _stackbuttons, _counters, _guiparameters.timeseries, _guiparameters.behavior, _guiparameters.stage, _guiparameters.allareas)
			, m_ScanSingleButton(_runbuttons.startsingle)
			, m_ScanLiveButton(_runbuttons.startlive)
			, m_StopButton(_runbuttons.stop)
			, m_QuitButton(_runbuttons.quit)
		{
			
		}

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

		void CMainDlgView::SetReadOnlyWhileScanning(const bool& _ro) {
			m_sheetExperimentSettings.SetReadOnlyWhileScanning(_ro);
			m_sheetScanSettings.SetReadOnlyWhileScanning(_ro);
		}

	}
}