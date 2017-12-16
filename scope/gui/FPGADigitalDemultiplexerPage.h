#pragma once

#include "controls/ScopeLEDCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controllers/ScopeController.h"
#include "devices/fpga/FPGADigitalDemultiplexer.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Class for the digital demultiplexer FPGA infos property page */
class CFPGADigitalDemultiplexerPage :
	public CPropertyPageImpl<CFPGADigitalDemultiplexerPage> {

protected:
	
public:
	enum { IDD = IDD_FPGADIGITALDEMULTIPLEXER_PROPPAGE };

	/** @name LEDs for FPGA status
	* @{ */
	CScopeLEDCtrl tohostoverflowA1Ch1_led;
	CScopeLEDCtrl tohostoverflowA1Ch2_led;
	CScopeLEDCtrl tohostoverflowA2Ch1_led;
	CScopeLEDCtrl tohostoverflowA2Ch2_led;
	CScopeLEDCtrl interloopoverflow_led;
	CScopeLEDCtrl interlooptimeout_led;
	CScopeLEDCtrl acquiring_led;
	/** @} */

	/** Displays measured laser frequency */
	CScopeEditCtrl<double> laserfreq_edit;

	/** Connect to the FIFOStatus ScopeNumbers */
	CFPGADigitalDemultiplexerPage(parameters::InputsFPGADigitalDemultiplexer& _parameters);

	BEGIN_MSG_MAP(CFPGADigitalDemultiplexerPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}
