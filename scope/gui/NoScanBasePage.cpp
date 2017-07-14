#include "stdafx.h"
#include "NoScanBasePage.h"

namespace scope {
	namespace gui {

CNoScanBasePage::CNoScanBasePage(const uint32_t& _area, parameters::ScannerVectorFrameBasic& _scanvecparams)
	: CToolTipDialog(TTS_NOPREFIX)
	, initialized(false)
	, area(_area)
	, pockels_scroll(&_scanvecparams.pockels, 0.01, 0.1, true, true)
	, pockels_edit (&_scanvecparams.pockels, true, true)
	, fastz_scroll(&_scanvecparams.fastz, 5, 50, true, true)
	, fastz_edit(&_scanvecparams.fastz, true, true)
	, pixeltime_edit(&scope_controller.GuiParameters.areas[area]->daq.pixeltime, true, true)
	, pixeltime_scroll(&scope_controller.GuiParameters.areas[area]->daq.pixeltime, scope_controller.GuiParameters.areas[area]->daq.inputs->MinimumPixeltime(), scope_controller.GuiParameters.areas[area]->daq.inputs->MinimumPixeltime(), true, true)
	, fpux_edit(&scope_controller.GuiParameters.areas[area]->fpuxystage.xpos, true, true)
	, fpuy_edit(&scope_controller.GuiParameters.areas[area]->fpuxystage.ypos, true, true)
	, fpuleft_button(&scope_controller.FPU[area].LeftButton)
	, fpuright_button(&scope_controller.FPU[area].RightButton)
	, fpuup_button(&scope_controller.FPU[area].UpButton)
	, fpudown_button(&scope_controller.FPU[area].DownButton) {

	std::wstringstream stream;
	stream << L"Area " << area+1;
	if ( scope_controller.GuiParameters.areas[area]->isslave() )
		stream << L"S";
	strtitle = stream.str();
	SetTitle(strtitle.c_str());			// Be careful, do not assign a local variable (since SetTitle takes only the pointer, which will become invalid for a local variable)

	// For disabling save preset button and presets combo during scanning
	rwstateconnection = scope_controller.ReadOnlyWhileScanning.ConnectGUI(std::bind(&CNoScanBasePage::SetReadOnlyWhileScanning, this));
}

CNoScanBasePage::~CNoScanBasePage(void) {
	// Disconnect
	rwstateconnection.disconnect();
}

BOOL CNoScanBasePage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	DBOUT(L"CNoScanBasePage::OnInitDialog this " << this);
	// attach the dialog controls to the edit objects
	pixeltime_edit.AttachToDlgItem(GetDlgItem(IDC_PIXELTIME_EDIT));
	pixeltime_scroll.AttachToDlgItem(GetDlgItem(IDC_PIXELTIME_SCROLL));
	pockels_scroll.AttachToDlgItem(GetDlgItem(IDC_POCKELS_SCROLL));
	pockels_edit.AttachToDlgItem(GetDlgItem(IDC_POCKELS_EDIT));
	fastz_scroll.AttachToDlgItem(GetDlgItem(IDC_FASTZ_SCROLL));
	fastz_edit.AttachToDlgItem(GetDlgItem(IDC_FASTZ_EDIT));
	
	// Set the FPU controls and arrow buttons
	fpux_edit.AttachToDlgItem(GetDlgItem(IDC_FPUX_EDIT));
	fpuy_edit.AttachToDlgItem(GetDlgItem(IDC_FPUY_EDIT));
	fpuleft_button.AttachToDlgItem(GetDlgItem(IDC_FPULEFT));
	fpuright_button.AttachToDlgItem(GetDlgItem(IDC_FPURIGHT));
	fpuup_button.AttachToDlgItem(GetDlgItem(IDC_FPUUP));
	fpudown_button.AttachToDlgItem(GetDlgItem(IDC_FPUDOWN));
	CIcon icon;
	icon.LoadIconW(IDI_ARROWLEFT);
	fpuleft_button.SetIcon(	icon.Detach());
	icon.LoadIconW(IDI_ARROWRIGHT);
	fpuright_button.SetIcon(	icon.Detach());
	icon.LoadIconW(IDI_ARROWUP);
	fpuup_button.SetIcon(	icon.Detach());
	icon.LoadIconW(IDI_ARROWDOWN);
	fpudown_button.SetIcon(	icon.Detach());

	// Set the description of the scan type in a bold font
	CStatic heading;
	heading.Attach(GetDlgItem(IDC_STATIC_HEADING));
	HFONT font(AtlCreateBoldFont());
	heading.SetFont(font);
	::DeleteFont(font);
	SetMsgHandled(true);

	initialized = true;

	return 0;
}

void CNoScanBasePage::SetReadOnlyWhileScanning() {
	// Nothing to do here...
}


}

}