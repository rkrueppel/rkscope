#pragma once

#include "ScanModesSettingsPage.h"
#include "StackSettingsPage.h"
#include "TimeSeriesSettingsPage.h"
#include "BehaviorSettingsPage.h"

namespace scope {
	namespace gui {

/** Settings sheet for the various property sheets dealing with different experiment types */
class CExperimentSettingsSheet
	: public CPropertySheetImpl<CExperimentSettingsSheet> {

protected:
	/** Page for selecting different scan modes */
	CScanModesSettingsPage pageScanModesSettings;

	/** Page for stack stuff */
	CStackSettingsPage pageStackSettings;

	/** Page for timeseries stuff */
	CTimeSeriesSettingsPage pageTimeSeriesSettings;

	/** Page for behavior triggered acquisition stuff */
	CBehaviorSettingsPage pageBehaviorSettings;

public:
	BEGIN_MSG_MAP(CUserSettingsSheet)	  
		NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnSelChange)
		CHAIN_MSG_MAP(CPropertySheetImpl<CExperimentSettingsSheet>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages.
	* Override Create and use Callback to switch to child window style during creation
	* since Property sheets are popups as standard (Windows not WTL I think) 
	* @{ */
	/** Adds the pages and do some magic */
	HWND Create(const HWND hWndParent, const int nStartPage, const CRect & rc);

	static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);

	LRESULT OnSelChange(WPARAM wParam, LPNMHDR pnmHdr, BOOL & bHandled);
	/** @} */
};

}}
