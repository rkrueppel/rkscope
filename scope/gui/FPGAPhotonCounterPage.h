#pragma once

#include "ScopeDefines.h"
#include "controls/ScopeLEDCtrl.h"
#include "controllers/ScopeController.h"
#include "devices/fpga/FPGAPhotonCounter.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Class for the photon counter FPGA infos property page */
class CFPGAPhotonCounterPage :
	public CPropertyPageImpl<CFPGAPhotonCounterPage> {

protected:

public:
	enum { IDD = IDD_FPGAPHOTONCOUNTER_PROPPAGE };

	/** @name LEDs for FPGA status
	* @{ */
	CScopeLEDCtrl tohostoverflow1_led;
	CScopeLEDCtrl tohostoverflow2_led;
	CScopeLEDCtrl interloopoverflow_led;
	CScopeLEDCtrl interlooptimeout_led;
	CScopeLEDCtrl acquiring_led;
	/** @} */

	/** pointer to the static FPGA object */
	FPGAPhotonCounter* const myFPGA;

	/** @name Buttons for the two counting modes
	* @{ */
	CButton counthigh_button;
	CButton countpulses_button;
	/** @} */

	/** Connect to the FIFOStatus ScopeNumbers */
	CFPGAPhotonCounterPage(parameters::InputsFPGAPhotonCounter* const _parameters);

	BEGIN_MSG_MAP(CFPGAPhotonCounterPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		COMMAND_HANDLER(IDC_COUNTHIGH_RADIO, BN_CLICKED, OnCountModeChange);
		COMMAND_HANDLER(IDC_COUNTPULSES_RADIO, BN_CLICKED, OnCountModeChange);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** Handles count mode changes */
	LRESULT OnCountModeChange(WORD, WORD, HWND, BOOL&);
	/** @} */
};

}}
