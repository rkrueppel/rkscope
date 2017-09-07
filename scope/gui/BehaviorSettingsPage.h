#pragma once

#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeProgressCtrl.h"
#include "TheScopeButtons.h"
#include "controllers/ScopeController.h"
#include "ThirdParty/ToolTipDialog.h"
#include "resource.h"

namespace scope {
	struct ScopeControllerCounters;
}

namespace scope {
	namespace gui {

/** Property page for settings of behavior triggered scanning */
class CBehaviorSettingsPage
	: public CPropertyPageImpl<CBehaviorSettingsPage>
	, public CToolTipDialog<CBehaviorSettingsPage> {

protected:
	/** start behavior triggered scanning */
	CScopeButtonCtrl start_behavior_button;

	parameters::Behavior& behaviorparameters;

	/** list box for all planes */
	CListViewCtrl planes_list;

	/** Edit control (disabled) that displays current framecount of curren trial */
	CScopeEditCtrl<double> framecount_edit;

	/** Edit control (disabled) that displays elapsed total time since start of first trial */
	CScopeEditCtrl<double> totaltime_edit;

	/** Edit control (disabled) that displays current trial number */
	CScopeEditCtrl<double> trialcount_edit;

public:
	enum { IDD = IDD_BEHAVIOR_PROPPAGE };

	CBehaviorSettingsPage(RunButtons& _runbuttons, ScopeControllerCounters& _scopecontrollercounters, parameters::Behavior& _behaviorparameters);

	BEGIN_MSG_MAP_EX(CBehaviorSettingsPage)	
		MSG_WM_INITDIALOG(OnInitDialog);
		COMMAND_HANDLER(IDC_ADDPLANE_BUTTON, BN_CLICKED, OnAddPlane)
		COMMAND_HANDLER(IDC_DELETEPLANE_BUTTON, BN_CLICKED, OnDeletePlane)
		CHAIN_MSG_MAP(CToolTipDialog<CBehaviorSettingsPage>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** Handles dialog initialization */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	LRESULT OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** @} */

	/** updates the list view ctrl on add/delete plane */
	void UpdatePlanesList();
};

}}