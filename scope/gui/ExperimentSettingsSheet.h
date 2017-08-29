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
	CScanModesSettingsPage scanmode;

	/** Page for stack stuff */
	CStackSettingsPage stack;

	/** Page for timeseries stuff */
	CTimeSeriesSettingsPage timeseries;

	/** Page for behavior triggered acquisition stuff */
	CBehaviorSettingsPage behavior;

public:
	CExperimentSettingsSheet(std::vector<ScanModeButtons>& _scanmodebuttonsvec
	, parameters::Stack& _stackparams
	, RunButtons& _runbuttons
	, StackButtons& _stackbuttons
	, ScopeControllerCounters& _counters
	, parameters::Timeseries& _timeseriesparams
	, parameters::Behavior& _behaviorparams);

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
