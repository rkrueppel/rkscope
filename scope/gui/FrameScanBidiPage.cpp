#include "StdAfx.h"
#include "FrameScanBidiPage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanBidiPage::CFrameScanBidiPage(const uint32_t& _area
	, const bool& _isslave
	, ScopeNumber<double>& _pockels
	, ScopeNumber<double>& _fastz
	, ScopeNumber<double>& _pixeltime
	, const double& _minpixeltime
	, ScopeNumber<double>& _fpux
	, ScopeNumber<double>& _fpuy
	, FPUButtons& _fpubuttons
	, ScopeNumber<bool>& _readonlywhilescanning
	, ScopeNumber<uint32_t>& _averages
	, ScopeNumber<double>& _scannerdelay
	, ScopeNumber<double>& _framerate
	, ScopeNumber<double>& _frametime
	, ScopeNumber<double>& _linerate
	, parameters::ScannerVectorFrameBiDi& _scanvecparams)
	//scope_controller.GuiParameters.areas[area]->isslave()
	// scope_controller.GuiParameters.areas[_area]->FrameBiDi()
	: CFrameScanBasePage(_area, _isslave, _pockels, _fastz, _pixeltime, _minpixeltime, _fpux, _fpuy, _fpubuttons, _readonlywhilescanning
		, _scanvecparams, _averages, _scannerdelay, _framerate, _frametime, _linerate)
	, isslave(_isslave)
	, xturn_edit(_scanvecparams.xturnfraction, true, true)
	, ycutoff_edit(_scanvecparams.ycutoff, true, true)
	, yretrace_edit(_scanvecparams.yretrace, true, true) {

	// Overwrite base class enum { IDD = ... }
	// Use different dialog resources depending if this area is a slave area (only Pockels and ETL sliders) or a master area (full control complement)
	if ( isslave )
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SLAVE_PROPPAGE);
	else
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_BIDI_PROPPAGE);
}

BOOL CFrameScanBidiPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CFrameScanBasePage::OnInitDialog(wndFocus, lInitParam);
	
	// slave areas do not have these controls
	if ( !isslave ) {
		// Add the additional dialog elements
		xturn_edit.AttachToDlgItem(GetDlgItem(IDC_XTURN_EDIT));
		ycutoff_edit.AttachToDlgItem(GetDlgItem(IDC_YCUTOFF_EDIT));
		yretrace_edit.AttachToDlgItem(GetDlgItem(IDC_YRETRACE_EDIT));
	}

	SetMsgHandled(true);
	return 0;
}


}

}
