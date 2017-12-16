#pragma once

#include "controls/ScopeLEDCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controllers/ScopeController.h"
#include "devices/fpga/FPGAResonanceScanner.h"
#include "resource.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGAResonanceScanner;
	}
}

namespace scope {
	namespace gui {

/** Class for the analog integrator FPGA info property page */
class CFPGAResonanceScannerPage :
	public CPropertyPageImpl<CFPGAResonanceScannerPage> {

protected:

public:
	enum { IDD = IDD_FPGARESONANCESCANNER_PROPPAGE };

	/** @name LEDs for FPGA status
	* @{ */
	CScopeLEDCtrl tohostoverflow1_led;
	CScopeLEDCtrl tohostoverflow2_led;
	CScopeLEDCtrl interloopoverflow_led;
	CScopeLEDCtrl interlooptimeout_led;
	CScopeLEDCtrl acquiring_led;
	CScopeLEDCtrl aioverrange_led;
	/** @} */

	/** @name Baseline and bitshift for both channels. Baseline is around 32768 and bitshift for conversion from U32 to U16, start at around 3?!
	* @{ */
	CScopeEditCtrl<uint8_t> bitshiftch1_edit;
	CScopeEditCtrl<uint32_t> baselinech1_edit;
	CScopeEditCtrl<uint8_t> bitshiftch2_edit;
	CScopeEditCtrl<uint32_t> baselinech2_edit;
	/** @} */

	/** Connect to the FIFOStatus ScopeNumbers */
	CFPGAResonanceScannerPage(parameters::InputsFPGAResonanceScanner* const _parameters);

	BEGIN_MSG_MAP(CFPGAResonanceScannerPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}