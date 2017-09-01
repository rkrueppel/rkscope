#include "StdAfx.h"
#include "FrameScanSawPage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanSawPage::CFrameScanSawPage(const uint32_t& _area
	, const bool& _isslave
	, ScopeNumber<double>& _pockels
	, ScopeNumber<double>& _fastz
	, ScopeNumber<double>& _pixeltime
	, const double& _minpixeltime
	, ScopeNumber<double>& _fpux
	, ScopeNumber<double>& _fpuy
	, FPUButtons& _fpubuttons
	, ScopeNumber<bool>& _readonlywhilescanning
	, parameters::ScannerVectorFrameSaw& _scanvecframesawparams
	, ScopeNumber<uint32_t>& _averages
	, ScopeNumber<double>& _scannerdelay
	, ScopeNumber<double>& _framerate
	, ScopeNumber<double>& _frametime
	, ScopeNumber<double>& _linerate
	)
	// scope_controller.GuiParameters.areas[_area]->FrameSaw()
	: CFrameScanBasePage(_area, _isslave, _pockels, _fastz, _pixeltime, _minpixeltime, _fpux, _fpuy, _fpubuttons, _readonlywhilescanning, _scanvecframesawparams, _averages, _scannerdelay, _framerate, _frametime, _linerate)
	, xcutoff_edit(_scanvecframesawparams.xcutoff, true, true)
	, ycutoff_edit(_scanvecframesawparams.ycutoff, true, true)
	, xretrace_edit(_scanvecframesawparams.xretrace, true, true)
	, yretrace_edit(_scanvecframesawparams.yretrace, true, true) {

	// Overwrite base class enum { IDD = ... }
	// Use different dialog resources depending if this area is a slave area (only Pockels and ETL sliders) or a master area (full control complement)
	if ( _isslave )
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SLAVE_PROPPAGE);
	else
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SAW_PROPPAGE);
}

BOOL CFrameScanSawPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CFrameScanBasePage::OnInitDialog(wndFocus, lInitParam);

	// Add the additional dialog elements
	xcutoff_edit.AttachToDlgItem(GetDlgItem(IDC_XCUTOFF_EDIT));
	ycutoff_edit.AttachToDlgItem(GetDlgItem(IDC_YCUTOFF_EDIT));
	xretrace_edit.AttachToDlgItem(GetDlgItem(IDC_XRETRACE_EDIT));
	yretrace_edit.AttachToDlgItem(GetDlgItem(IDC_YRETRACE_EDIT));

	SetMsgHandled(true);
	return 0;
}


}

}
