#pragma once

#include "FrameScanBasePage.h"
#include "controls/ScopeSliderCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controllers/ScopeController.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Property page for bidirectional frame scan */
class CFrameScanBidiPage
	: public CFrameScanBasePage {

protected:
	/** turn fraction for x scanner */
	CScopeEditCtrl<double> xturn_edit;

	/** cutoff fraction for y scanner */
	CScopeEditCtrl<double> ycutoff_edit;		

	/** retrace fraction for y scanner */
	CScopeEditCtrl<double> yretrace_edit;

public:
	/** create the edit objects (e.g. ScopeEditImpl) and tie them to the global variables (ScopeValues)
	* set connect back to true -> on change of ScopeValue (e.g. by ScopeController) the dialog control will
	* be updated accordingly */
	CFrameScanBidiPage(const uint32_t& _area, parameters::BaseArea& _areaparams, FPUButtons& _fpubuttons);

	// Only needed if we add handlers here (see e.g. CFrameScanHopperPage)
	BEGIN_MSG_MAP(CFrameScanBidiPage)
		CHAIN_MSG_MAP(CFrameScanBasePage)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}