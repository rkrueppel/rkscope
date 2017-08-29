#pragma once

#include "controllers/ScopeControllerButtons.h"
#include "controllers/ScopeControllerCounter.h"
#include "parameters/Runstates.h"
#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeProgressCtrl.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Settings page for stack scan stuff */
class CStackSettingsPage :
	public CPropertyPageImpl<CStackSettingsPage> {

protected:
	/** true if window is initialized */
	bool initialized;
	
	parameters::Stack& stackparams;

	/** to start stack scan */
	CScopeButtonCtrl start_stack_button;

	/** stack start zcontrol position */
	CScopeEditCtrl<double> startat_edit;

	/** stack end zcontrol position */
	CScopeEditCtrl<double> stopat_edit;

	/** spacing (in um) between slices */
	CScopeEditCtrl<double> spacing_edit;

	/** to set startat to current position */
	CScopeButtonCtrl starthere_button;

	/** to set stopat to current position */
	CScopeButtonCtrl stophere_button;

	/** for stack progress */
	CScopeProgressCtrl stack_progress;

	/** trigger recalculation of the slices list */
	CButton recalc_button;

	/** use zstage radio button */
	CButton zstage_button;

	/** use fastz radio button */
	CButton fastz_button;

	/** for displaying space constant of exponential pockels interpolation (see parameters::stack) */
	CStatic lambda_static;

	/** keeps track locally which zdevice type to use for stack */
	ZDevice zdevicetype;

	/** to edit properties of each plane */
	CListViewCtrl planes_list;

public:
	enum { IDD = IDD_STACK_PROPPAGE };

	CStackSettingsPage(parameters::Stack& _stackparams, RunButtons& _runbuttons, StackButtons& _stackbuttons, ScopeControllerCounters& _counters);

	BEGIN_MSG_MAP_EX(CStackSettingsPage)	
		MSG_WM_INITDIALOG(OnInitDialog);
		COMMAND_HANDLER(IDC_RECALC_BUTTON, BN_CLICKED, OnRecalcButton)
		COMMAND_HANDLER(IDC_ZSTAGE_RADIO, BN_CLICKED, OnZControlRadio)
		COMMAND_HANDLER(IDC_ETL_RADIO, BN_CLICKED, OnZControlRadio)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */

	/** initializes the dialog */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

	/** handles clicks on the z control buttons */
	LRESULT OnZControlRadio(WORD, WORD, HWND, BOOL&);

	/** handles clicks on the recalculate button */
	LRESULT OnRecalcButton(WORD, WORD, HWND, BOOL&);
	/** @} */

	/** changes button states etc when acquisition is running. Is connected to ScopeController::DisableOnRun */
	void OnChangedRunState();

	/** updates the list view ctrl on start/stop/spacing changes */
	void UpdatePlanesList();
};

}}

