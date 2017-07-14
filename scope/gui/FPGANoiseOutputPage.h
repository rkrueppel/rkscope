#pragma once

#include "ScopeDefines.h"
#include "controls/ScopeLEDCtrl.h"
#include "controllers/ScopeController.h"
#include "devices/fpga/FPGANoiseOutput.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Class for the noise output FPGA infos property page */
class CFPGANoiseOutputPage :
	public CPropertyPageImpl<CFPGANoiseOutputPage> {

protected:

public:
	enum { IDD = IDD_FPGANOISEOUTPUT_PROPPAGE };

	/** @name LEDs for FPGA status
	* @{ */
	CScopeLEDCtrl tohostoverflowA1Ch1_led;
	CScopeLEDCtrl tohostoverflowA1Ch2_led;
	CScopeLEDCtrl tohostoverflowA2Ch1_led;
	CScopeLEDCtrl tohostoverflowA2Ch2_led;
	CScopeLEDCtrl acquiring_led;
	/** @} */

	/** Connect to the FIFOStatus ScopeNumbers */
	CFPGANoiseOutputPage(parameters::InputsFPGANoiseOutput* const _parameters);

	BEGIN_MSG_MAP(CFPGANoiseOutputPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}
