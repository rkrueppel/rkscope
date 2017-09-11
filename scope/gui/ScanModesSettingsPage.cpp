#include "StdAfx.h"
#include "ScanModesSettingsPage.h"
#include "scanmodes/ScannerVectorFrameBasic.h"

namespace scope {
	namespace gui {

CScanModesSettingsPage::CScanModesSettingsPage(std::vector<ScanModeButtons>& _scanmodebuttonsvec)
	: scanmodebuttonsvec(_scanmodebuttonsvec)
	, framesaw_radio(_scanmodebuttonsvec[0].map.at(ScannerVectorTypeHelper::Sawtooth))
	, framebidi_radio(_scanmodebuttonsvec[0].map.at(ScannerVectorTypeHelper::Bidirectional))
	, framehopper_radio(_scanmodebuttonsvec[0].map.at(ScannerVectorTypeHelper::Planehopper))
	, frameresonancebidi_radio(_scanmodebuttonsvec[0].map.at(ScannerVectorTypeHelper::ResonanceBiDi))
	, frameresonancehopper_radio(_scanmodebuttonsvec[0].map.at(ScannerVectorTypeHelper::ResonanceHopper))
	, linestraight_radio(_scanmodebuttonsvec[0].map.at(ScannerVectorTypeHelper::LineStraight)) {
	// start with buttons connected to area 0
}

BOOL CScanModesSettingsPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	areas_list.Attach(GetDlgItem(IDC_AREAS_LIST));
	framesaw_radio.AttachToDlgItem(GetDlgItem(IDC_FRAME_SAWTOOTH_RADIO));
	framebidi_radio.AttachToDlgItem(GetDlgItem(IDC_FRAME_BIDI_RADIO));
	framehopper_radio.AttachToDlgItem(GetDlgItem(IDC_FRAME_PLANEHOPPER_RADIO));
	frameresonancebidi_radio.AttachToDlgItem(GetDlgItem(IDC_FRAME_RESONANT_BIDI_RADIO));
	frameresonancehopper_radio.AttachToDlgItem(GetDlgItem(IDC_FRAME_RESONANT_PLANEHOPPER_RADIO));
	linestraight_radio.AttachToDlgItem(GetDlgItem(IDC_LINE_STRAIGHT_RADIO));

	// Disable scan modes that are not supported by builtin scanner
	framesaw_radio.Enable(ScannerSupportedVectors::IsBuiltinSupported(ScannerVectorTypeHelper::Sawtooth));
	framebidi_radio.Enable(ScannerSupportedVectors::IsBuiltinSupported(ScannerVectorTypeHelper::Bidirectional));
	framehopper_radio.Enable(ScannerSupportedVectors::IsBuiltinSupported(ScannerVectorTypeHelper::Planehopper));
	frameresonancebidi_radio.Enable(ScannerSupportedVectors::IsBuiltinSupported(ScannerVectorTypeHelper::ResonanceBiDi));
	frameresonancehopper_radio.Enable(ScannerSupportedVectors::IsBuiltinSupported(ScannerVectorTypeHelper::ResonanceHopper));
	linestraight_radio.Enable(ScannerSupportedVectors::IsBuiltinSupported(ScannerVectorTypeHelper::LineStraight));

	// Add areas to list box and select area 0
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		std::wostringstream stream;
		stream << L"Area " << a+1;
		areas_list.AddString(stream.str().c_str());
	}
	areas_list.SetCurSel(0);

	SetChecks(0);

	SetMsgHandled(false);
	return 0;
}

LRESULT CScanModesSettingsPage::OnAreaChange(WORD, WORD, HWND, BOOL&) {
	int32_t sel = areas_list.GetCurSel();
	
	if ( sel == LB_ERR )
		return 0;
	assert( (sel >= 0) && (sel < SCOPE_NAREAS) );

	// Disconnect from old areas ScanMode buttons
	framesaw_radio.Disconnect();
	framebidi_radio.Disconnect();
	framehopper_radio.Disconnect();
	frameresonancebidi_radio.Disconnect();
	frameresonancehopper_radio.Disconnect();
	linestraight_radio.Disconnect();

	// Connect to selected area's ScanMode buttons and uncheck all
	framesaw_radio.Connect(scanmodebuttonsvec[sel].map.at(ScannerVectorTypeHelper::Sawtooth));
	framesaw_radio.SetCheck(BST_UNCHECKED);
	framebidi_radio.Connect(scanmodebuttonsvec[sel].map.at(ScannerVectorTypeHelper::Bidirectional));
	framebidi_radio.SetCheck(BST_UNCHECKED);
	framehopper_radio.Connect(scanmodebuttonsvec[sel].map.at(ScannerVectorTypeHelper::Planehopper));
	framehopper_radio.SetCheck(BST_UNCHECKED);
	frameresonancebidi_radio.Connect(scanmodebuttonsvec[sel].map.at(ScannerVectorTypeHelper::ResonanceBiDi));
	frameresonancebidi_radio.SetCheck(BST_UNCHECKED);
	frameresonancehopper_radio.Connect(scanmodebuttonsvec[sel].map.at(ScannerVectorTypeHelper::ResonanceHopper));
	frameresonancehopper_radio.SetCheck(BST_UNCHECKED);
	linestraight_radio.Connect(scanmodebuttonsvec[sel].map.at(ScannerVectorTypeHelper::LineStraight));
	linestraight_radio.SetCheck(BST_UNCHECKED);

	SetChecks(sel);

	return 0;
}

void CScanModesSettingsPage::SetChecks(const uint32_t& _a) {
	// Copy the settings of the current area from the master area
/*	scope_controller.GuiParameters.areas[_a]->scanmode() = scope_controller.GuiParameters.areas[0]->scanmode();

	// Check the one that corresponds to the scan mode of this area (if the scan mode is supported by the built-in scanners is checked inside Scope_Controller::SetScanMode()
	switch ( scope_controller.GuiParameters.areas[_a]->scanmode().t ) {
		case ScannerVectorTypeHelper::Sawtooth:
			framesaw_radio.SetCheck(BST_CHECKED);
			break;
		case ScannerVectorTypeHelper::Bidirectional:
			framebidi_radio.SetCheck(BST_CHECKED);
			break;
		case ScannerVectorTypeHelper::Planehopper:
			framehopper_radio.SetCheck(BST_CHECKED);
			break;
		case ScannerVectorTypeHelper::ResonanceBiDi:
			frameresonancebidi_radio.SetCheck(BST_CHECKED);
			break;
		case ScannerVectorTypeHelper::ResonanceHopper:
			frameresonancehopper_radio.SetCheck(BST_CHECKED);
			break;
		case ScannerVectorTypeHelper::LineStraight:
			linestraight_radio.SetCheck(BST_CHECKED);
			break;
	}*/
}


}

}
