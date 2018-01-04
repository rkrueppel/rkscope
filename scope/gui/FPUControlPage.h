#pragma once

#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeFOVDiagram.h"
#include "TheScopeButtons.h"
#include "parameters\Area.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Property page for FPU stage settings */
class CFPUControlPage :
	public CPropertyPageImpl<CFPUControlPage> {

protected:
	/** Reference to TheScope's area parameters for the area this page serves */
	parameters::BaseArea& areaparams;

	/** the page title */
	std::wstring strtitle;

	/** edit for the FPU stage's x position */
	CScopeEditCtrl<double> xpos_edit;

	/** edit for the FPU stage's y position */
	CScopeEditCtrl<double> ypos_edit;

	/** edit for the FPU fast Z calibration file name */
	CScopeEditCtrl<std::wstring> etlcalibrationfile_edit;

	/** button to set current xy position as zero */
	CScopeButtonCtrl setxyzero_button;

	/** edit control for displaying FPU stage information */
	CScopeEditCtrl<std::wstring> fpustageinfos_edit;

	/** diagram displaying FOV positions */
	CScopeFOVDiagram diagram;

public:
	enum { IDD = IDD_FPUCONTROL_PROPPAGE };

	/** create the edit objects (e.g. ScopeEditImpl) and tie them to the global variables (ScopeValues)
	* set connect back to true -> on change of ScopeValue (e.g. by scope_controller) the dialog control will
	* be updated accordingly. The FOVDiagram shows all areas and thus needs information about all areas. */
	CFPUControlPage(const uint32_t& _area, const AreaTypeHelper::Mode& _areatype, std::vector<parameters::MasterArea>& _masterareas, std::vector<parameters::SlaveArea>& _slaveareas, FPUButtons& _fpubuttons, const double& _masterfovsizex, const double& _masterfovsizey);

	BEGIN_MSG_MAP_EX(CFPUControlPage)	
		MSG_WM_INITDIALOG(OnInitDialog);
		COMMAND_HANDLER(IDC_CALIBRATIONFILE_BUTTON, BN_CLICKED, OnCalibrationFileButton)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** attach the dialog controls to the edit objects */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** Loads fast z calibration file */
	LRESULT OnCalibrationFileButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** @} */
};

}
}
