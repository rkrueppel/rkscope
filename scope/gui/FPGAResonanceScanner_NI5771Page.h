#pragma once

#include "ScopeDefines.h"
#include "controls/ScopeLEDCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controllers/ScopeController.h"
#include "devices/fpga/FPGAResonanceScanner_NI5771.h"
#include "resource.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGAAnalogIntegrator;
	}
}

namespace scope {
	namespace gui {

/** Class for the analog integrator FPGA info property page */
class CFPGAResonanceScannerNI5771Page :
	public CPropertyPageImpl<CFPGAResonanceScannerNI5771Page> {

protected:

public:
	enum { IDD = IDD_FPGAANALOGINTEGRATOR_PROPPAGE };

	/** @name LEDs for FPGA status
	* @{ */
	CScopeLEDCtrl tohostoverflow1_led;
	CScopeLEDCtrl tohostoverflow2_led;
	CScopeLEDCtrl interloopoverflow_led;
	CScopeLEDCtrl interlooptimeout_led;
	CScopeLEDCtrl acquiring_led;
	CScopeLEDCtrl aioverrange_led;
	/** @} */

	/** @name Baseline and bitshift for both channels. Baseline is around 127 and bitshift for conversion from U32 to U16, start at around 3?!
	* @{ */
	CScopeEditCtrl<uint8_t> bitshiftch1_edit;
	CScopeEditCtrl<uint8_t> baselinech1_edit;
	CScopeEditCtrl<uint8_t> bitshiftch2_edit;
	CScopeEditCtrl<uint8_t> baselinech2_edit;
	/** @} */

	/** Connect to the FIFOStatus ScopeNumbers */
	CFPGAResonanceScannerNI5771Page(parameters::InputsFPGAAnalogIntegrator* const _parameters);

	BEGIN_MSG_MAP(CFPGAResonanceScannerNI5771Page)
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}