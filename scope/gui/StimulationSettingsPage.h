#pragma once

#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controllers/ScopeController.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Settings page for stimulation stuff */
class CStimulationSettingsPage :
	public CPropertyPageImpl<CStimulationSettingsPage> {

protected:
	/** stimulation onset (in sec) */
	CScopeEditCtrl<double> onset;

	/** total duration of stimulation, is filled with as many stimulation pulses as possible according to on&off time (in sec) */
	CScopeEditCtrl<double> duration;

	/** on time of one stimulation pulse (in sec) */
	CScopeEditCtrl<double> ontime;

	/** off time of one stimulation pulse (in sec) */
	CScopeEditCtrl<double> offtime;

	/** to enable the whole stimulation page */
	CScopeCheckBoxCtrl enabled;

public:

	enum { IDD = IDD_STIMULATION_PROPPAGE };

	/** create the edit objects (e.g. ScopeEditImpl) and tie them to the global variables (ScopeValues)
	* set connect back to true -> on change of ScopeValue (e.g. by ScopeController) the dialog control will
	* be updated accordingly */
	CStimulationSettingsPage(void);

	BEGIN_MSG_MAP_EX(CStimulationSettingsPage)	
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */

	/** Enable/Disable controls, depending on scope_controller.GuiParameters.stimulation.enable() */
	void EnableDisableControls();
};

}}

