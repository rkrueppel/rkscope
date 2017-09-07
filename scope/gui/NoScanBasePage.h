#pragma once

#include "ThirdParty/ToolTipDialog.h"
#include "controls/ScopeSliderCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controls/ScopeUpDownCtrl.h"
#include "controls/ScopeScrollbarCtrl.h"
#include "TheScopeButtons.h"
#include "scanmodes/ScannerVectorFrameBasic.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Base class for all scanning and non-scanning scan property pages, i.e. for frame-scanning pages and for non-scanning slave area pages.
* Uses CToolTipDialog for displaying help tooltips. Add strings with the same ID as a control for help to that control. */
class CNoScanBasePage
	: public CPropertyPageImpl<CNoScanBasePage>
	, public CToolTipDialog<CNoScanBasePage> {

protected:
	/** true after dialog is initialized */
	bool initialized;

	/** for which area are the image settings */
	const uint32_t area;

	const bool isslave;

	/** string for settings page title */
	std::wstring strtitle;

	/** pockels cell scrollbar control */
	CScopeScrollbarCtrl pockels_scroll;

	/** pockels cell edit control */
	CScopeEditCtrl<double> pockels_edit;

	/** Fast Z scrollbar control */
	CScopeScrollbarCtrl fastz_scroll;

	/** Fast Z edit control */
	CScopeEditCtrl<double> fastz_edit;

	/** pixel dwell time scrollbar control */
	CScopeScrollbarCtrl pixeltime_scroll;

	/** pixel dwell time edit control */
	CScopeEditCtrl<double> pixeltime_edit;

	/** FPU x position edit control */
	CScopeEditCtrl<double> fpux_edit;

	/** FPU y position edit control */
	CScopeEditCtrl<double> fpuy_edit;

	/** nudge FPU left button control */
	CScopeButtonCtrl fpuleft_button;

	/** nudge FPU rightbutton control */
	CScopeButtonCtrl fpuright_button;

	/** nudge FPU up button control */
	CScopeButtonCtrl fpuup_button;

	/** nudge FPU down button control */
	CScopeButtonCtrl fpudown_button;

public:
	/** Dialog resource ID, do not forget to overwrite in derived class constructor! */
	enum { IDD = IDD_NOSCAN_PROPPAGE };

	/** We need a pointer to a basic ScannerVector (could be e.g. Saw really) but thus we can connect to (inherited) members */
	CNoScanBasePage(const uint32_t& _area
		, const bool& _isslave
		, ScopeNumber<double>& _pockels
		, ScopeNumber<double>& _fastz
		, ScopeNumber<double>& _pixeltime
		, const double& _minpixeltime
		, ScopeNumber<double>& _fpux
		, ScopeNumber<double>& _fpuy
		, FPUButtons& _fpubuttons
	);

	BEGIN_MSG_MAP_EX(CNoScanBasePage)	
		MSG_WM_INITDIALOG(OnInitDialog);
		CHAIN_MSG_MAP(CToolTipDialog<CNoScanBasePage>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */

	/** @name Called from ScopeController */
	/** Connected to ScopeController::ReadOnlyWhileScanning */
	virtual void SetReadOnlyWhileScanning(const bool& _ro) {}
	/** @} */
};

}}