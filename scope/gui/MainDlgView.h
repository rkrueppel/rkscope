#pragma once

#include "controllers/ScopeControllerButtons.h"
#include "ScanSettingsSheet.h"
#include "ExperimentSettingsSheet.h"
#include "controls/ScopeButtonCtrl.h"
#include "resource.h"


namespace scope {
	namespace gui {

/** View class of the main dialog */
class CMainDlgView
	: public CDialogImpl<CMainDlgView> {

protected:
	/** the settings sheet with all the stuff */
	CScanSettingsSheet m_sheetScanSettings;

	/** the settings sheet for the different scan modes */
	CExperimentSettingsSheet m_sheetExperimentSettings;

	/** to start live scanning */
	CScopeButtonCtrl m_ScanLiveButton;

	/** to start single frame scan */
	CScopeButtonCtrl m_ScanSingleButton;

	/** to stop whatever scanning */
	CScopeButtonCtrl m_StopButton;

	/** to quit the application */
	CScopeButtonCtrl m_QuitButton;

public:
	enum { IDD = IDD_MAINDIALOG };

	/** Attach buttons to ScopeController */
	CMainDlgView(scope::RunButtons& _runbuttons);
	~CMainDlgView();
	
	BEGIN_MSG_MAP(CScopeView)
		MSG_WM_INITDIALOG(OnInitDialog)
		REFLECT_NOTIFICATIONS()
//		COMMAND_HANDLER(IDC_SCAN_SINGLE, BN_CLICKED, OnBnClickedScanSingle)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL PreTranslateMessage(MSG* pMsg);

	/** Replace placeholders with property sheets (for scan settings and experiment settings) */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
	
	void ChangeScanMode(const uint32_t& _area, const ScannerVectorType& _mode);
	
	/** Set controls that are not coupled to ScopeNumbers etc. (e.g. presets combo box) to read only while scanning */
	void SetReadOnlyWhileScanning(const bool& _ro);
public:
//	LRESULT OnBnClickedScanSingle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

}}