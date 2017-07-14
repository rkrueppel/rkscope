#include "StdAfx.h"
#include "StimulationSettingsPage.h"
#include "resource.h"

namespace scope {
	namespace gui {

CStimulationSettingsPage::CStimulationSettingsPage(void)
	: onset(&scope_controller.GuiParameters.stimulation.onset, true)
	, duration(&scope_controller.GuiParameters.stimulation.duration, true)
	, ontime(&scope_controller.GuiParameters.stimulation.ontime, true)
	, offtime(&scope_controller.GuiParameters.stimulation.offtime, true)
	, enabled(&scope_controller.GuiParameters.stimulation.enable, true) {
	scope_controller.GuiParameters.stimulation.enable.ConnectOther(std::bind(&CStimulationSettingsPage::EnableDisableControls, this));
}

void CStimulationSettingsPage::EnableDisableControls() {
	DBOUT(L"CStimulationSettings::EnableDisableControls\n");
	// Attention: ReadOnly ist !Enable
	bool state = scope_controller.GuiParameters.stimulation.enable();
	onset.SetState(state);
	duration.SetState(state);
	ontime.SetState(state);
	offtime.SetState(state);
}

BOOL CStimulationSettingsPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	// attach the dialog controls to the edit objects
	onset.AttachToDlgItem(GetDlgItem(IDC_ONSET));
	duration.AttachToDlgItem(GetDlgItem(IDC_DURATION));
	ontime.AttachToDlgItem(GetDlgItem(IDC_ONTIME));
	offtime.AttachToDlgItem(GetDlgItem(IDC_OFFTIME));
	enabled.AttachToDlgItem(GetDlgItem(IDC_ENABLE));

	// set depending on initial stimulation.enable value
	EnableDisableControls();

	SetMsgHandled(false);
	return 0;
}

}}