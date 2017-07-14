#include "StdAfx.h"
#include "FrameScanBidiPage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanBidiPage::CFrameScanBidiPage(const uint32_t& _area)
	: CFrameScanBasePage(_area, scope_controller.GuiParameters.areas[_area]->FrameBiDi())
	, xturn_edit(&scope_controller.GuiParameters.areas[_area]->FrameBiDi().xturnfraction, true, true)
	, ycutoff_edit(&scope_controller.GuiParameters.areas[_area]->FrameBiDi().ycutoff, true, true)
	, yretrace_edit(&scope_controller.GuiParameters.areas[_area]->FrameBiDi().yretrace, true, true) {

	// Overwrite base class enum { IDD = ... }
	// Use different dialog resources depending if this area is a slave area (only Pockels and ETL sliders) or a master area (full control complement)
	if ( scope_controller.GuiParameters.areas[area]->isslave() )
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SLAVE_PROPPAGE);
	else
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_BIDI_PROPPAGE);
}

BOOL CFrameScanBidiPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CFrameScanBasePage::OnInitDialog(wndFocus, lInitParam);
	
	// slave areas do not have these controls
	if ( !scope_controller.GuiParameters.areas[area]->isslave() ) {
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
