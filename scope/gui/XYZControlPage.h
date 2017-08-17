#pragma once

#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controllers/ScopeController.h"
#include "controls\ScopeColorComboCtrl.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Property page for XYZ stage settings */
class CXYZControlPage :
	public CPropertyPageImpl<CXYZControlPage> {

protected:
	/** edit for the stage's x position */
	CScopeEditCtrl<double> xpos_edit;

	/** edit for the stage's y position */
	CScopeEditCtrl<double> ypos_edit;

	/** edit for the stage's z position */
	CScopeEditCtrl<double> zpos_edit;

	/** button to set current position as zero */
	CScopeButtonCtrl setzero_button;

public:
	enum { IDD = IDD_XYZCONTROL_PROPPAGE };

	/** create the edit objects (e.g. ScopeEditImpl) and tie them to the global variables (ScopeValues)
	* set connect back to true -> on change of ScopeValue (e.g. by scope_controller) the dialog control will
	* be updated accordingly */
	CXYZControlPage(void);

	BEGIN_MSG_MAP_EX(CXYZControlPage)	
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** attach the dialog controls to the edit objects */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}
}
