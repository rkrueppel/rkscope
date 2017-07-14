#include "stdafx.h"
#include "FrameScanBasePage.h"
#include "PresetNameDlg.h"

namespace scope {
namespace gui {

CFrameScanBasePage::CFrameScanBasePage(const uint32_t& _area, parameters::ScannerVectorFrameBasic& _scanvecparams)
	: CNoScanBasePage(_area, _scanvecparams)
	, zoom_edit(&_scanvecparams.zoom, true, true)
	, zoom_scroll(&_scanvecparams.zoom, 0.1, 1, true, true)
	, xres_edit(&_scanvecparams.xres, true, true)
	, yres_edit(&_scanvecparams.yres, true, true)
	, xaspectratio_edit(&_scanvecparams.xaspectratio, true, true)
	, yaspectratio_edit(&_scanvecparams.yaspectratio, true, true)
	, averages_edit(&scope_controller.GuiParameters.areas[area]->daq.averages, true, true)
	, scannerdelay_edit(&scope_controller.GuiParameters.areas[area]->daq.scannerdelay, true, true)
	, xoffset_edit(&_scanvecparams.xoffset, true, true)
	, yoffset_edit(&_scanvecparams.yoffset, true, true)
	, framerate_edit(&scope_controller.GuiParameters.areas[area]->framerate, true, true)
	, frametime_edit(&scope_controller.GuiParameters.areas[area]->frametime, true, true)
	, linerate_edit(&scope_controller.GuiParameters.areas[area]->linerate, true, true)
	, diagram(area, &scope_controller.GuiParameters) {
	
	// For disabling save preset button and presets combo during scanning
	rwstateconnection = scope_controller.ReadOnlyWhileScanning.ConnectGUI(std::bind(&CFrameScanBasePage::SetReadOnlyWhileScanning, this));
}

CFrameScanBasePage::~CFrameScanBasePage() {
	// Disconnect
	rwstateconnection.disconnect();
}

BOOL CFrameScanBasePage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	// Call base class dialog initialization
	CNoScanBasePage::OnInitDialog(wndFocus, lInitParam);

	// attach the dialog controls to the edit objects
	presets_combo.Attach(GetDlgItem(IDC_PRESETS_COMBO));
	savepreset_button.Attach(GetDlgItem(IDC_SAVEPRESET_BUTTON));
	deletepreset_button.Attach(GetDlgItem(IDC_DELETEPRESET_BUTTON));
	zoom_edit.AttachToDlgItem(GetDlgItem(IDC_ZOOM_EDIT));
	zoom_scroll.AttachToDlgItem(GetDlgItem(IDC_ZOOM_SCROLL));
	xres_edit.AttachToDlgItem(GetDlgItem(IDC_PIXELX_EDIT));
	yres_edit.AttachToDlgItem(GetDlgItem(IDC_PIXELY_EDIT));
	xaspectratio_edit.AttachToDlgItem(GetDlgItem(IDC_ASPECTRATIOX_EDIT));
	yaspectratio_edit.AttachToDlgItem(GetDlgItem(IDC_ASPECTRATIOY_EDIT));
	averages_edit.AttachToDlgItem(GetDlgItem(IDC_AVERAGES_EDIT));
	scannerdelay_edit.AttachToDlgItem(GetDlgItem(IDC_SCANNERDELAY_EDIT));
	xoffset_edit.AttachToDlgItem(GetDlgItem(IDC_XOFFSET_EDIT));
	yoffset_edit.AttachToDlgItem(GetDlgItem(IDC_YOFFSET_EDIT));
	framerate_edit.AttachToDlgItem(GetDlgItem(IDC_FRAMERATE));
	frametime_edit.AttachToDlgItem(GetDlgItem(IDC_FRAMETIME));
	linerate_edit.AttachToDlgItem(GetDlgItem(IDC_LINERATE));

	diagram.AttachToDlgItem(GetDlgItem(IDC_FPUDIAGRAM));

	// Set before by CNoScanBasePage::OnInitDialog
	initialized = initialized && true;

	SetMsgHandled(true);

	return 0;
}

LRESULT CFrameScanBasePage::OnPresetSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	std::shared_ptr<std::wstring> strname = std::make_shared<std::wstring>(L"PresetName");
	CPresetNameDlg dlg(strname);
	if ( dlg.DoModal(::GetActiveWindow()) == 0 ) {
		scope_controller.GuiParameters.areas[area]->SaveToPreset(*strname);
		FillPresetCombo();
		presets_combo.SelectString(0, strname->c_str());
	}
	return 0;
}

LRESULT CFrameScanBasePage::OnPresetDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = presets_combo.GetCurSel();
	if ( sel > 0 ) {
		std::vector<wchar_t> buff(presets_combo.GetLBTextLen(sel)+1, L' ');			// +1 since GetLBTextLen does not count terminating \0 ... sigh...
		presets_combo.GetLBText(sel, buff.data());
		scope_controller.GuiParameters.areas[area]->DeletePreset(std::wstring(buff.begin(), buff.end()-1));
		FillPresetCombo();
	}
	return 0;
}

LRESULT CFrameScanBasePage::OnPresetSelection(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = presets_combo.GetCurSel();
	if ( sel > 0 ) {
		std::vector<wchar_t> buff(presets_combo.GetLBTextLen(sel)+1, L' ');			// +1 since GetLBTextLen does not count terminating \0 ... sigh...
		presets_combo.GetLBText(sel, buff.data());
		scope_controller.GuiParameters.areas[area]->LoadFromPreset(std::wstring(buff.begin(), buff.end()-1));
	}
	return 0;
}

void CFrameScanBasePage::SetReadOnlyWhileScanning() {
	// Do not call window procedures on non-initialized dialog
	if ( initialized ) {
		// Check if to disable or to enable now
		// Since the other controls are connected to GuiParameters they are taken care of automatically
		BOOL enabler = (scope_controller.ReadOnlyWhileScanning()==true)?FALSE:TRUE;
		presets_combo.EnableWindow(enabler);
		savepreset_button.EnableWindow(enabler);
		deletepreset_button.EnableWindow(enabler);
	}
}

void CFrameScanBasePage::FillPresetCombo() {
	// Clear the box
	presets_combo.ResetContent();
	
	// custom preset allows defining new presets etc
	presets_combo.AddString(L"Custom...");
	
	// load the presets' names into the combo box
	for ( const auto& p : scope_controller.GuiParameters.areas[area]->Currentframe().presets )
		presets_combo.AddString(p->name().c_str());

	// select the first preset (the custom one)
	presets_combo.SetCurSel(0);
}



}

}