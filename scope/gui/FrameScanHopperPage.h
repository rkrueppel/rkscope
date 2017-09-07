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
class CFrameScanHopperPage
	: public CFrameScanBasePage {

protected:
	parameters::ScannerVectorFramePlaneHopper& hoppervecparams;

	/** list box for all planes */
	CListViewCtrl planes_list;

public:
	enum { IDD = IDD_FRAMESCAN_HOPPER_PROPPAGE };

	/** create the edit objects (e.g. ScopeEditImpl) and tie them to the global variables (ScopeValues)
	* set connect back to true -> on change of ScopeValue (e.g. by ScopeController) the dialog control will
	* be updated accordingly */
	CFrameScanHopperPage(const uint32_t& _area
	, const bool& _isslave
	, ScopeNumber<double>& _pockels
	, ScopeNumber<double>& _fastz
	, ScopeNumber<double>& _pixeltime
	, const double& _minpixeltime
	, ScopeNumber<double>& _fpux
	, ScopeNumber<double>& _fpuy
	, FPUButtons& _fpubuttons
	, parameters::ScannerVectorFramePlaneHopper& _scanvecframehopperparams
	, ScopeNumber<uint32_t>& _averages
	, ScopeNumber<double>& _scannerdelay
	, ScopeNumber<double>& _framerate
	, ScopeNumber<double>& _frametime
	, ScopeNumber<double>& _linerate
	);

	BEGIN_MSG_MAP(CFrameScanHopperPage)
		COMMAND_HANDLER(IDC_ADDPLANE_BUTTON, BN_CLICKED, OnAddPlane)
		COMMAND_HANDLER(IDC_DELETEPLANE_BUTTON, BN_CLICKED, OnDeletePlane)
		CHAIN_MSG_MAP(CFrameScanBasePage)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* Note: On WM_INITDIALOG message is in message map of base class, only override handling member function here
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	LRESULT OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** @} */

	/** updates the list view ctrl on add/delete plane */
	void UpdatePlanesList();
};

}}