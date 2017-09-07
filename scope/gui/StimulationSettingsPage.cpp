#include "StdAfx.h"
#include "StimulationSettingsPage.h"
#include "resource.h"

namespace scope {
	namespace gui {

CStimulationSettingsPage::CStimulationSettingsPage(parameters::Stimulation& _stimparams)
	: stimenable(_stimparams.enable)
	, onset(_stimparams.onset, true)
	, duration(_stimparams.duration, true)
	, ontime(_stimparams.ontime, true)
	, offtime(_stimparams.offtime, true)
	, enabled(_stimparams.enable, true)
{
	stimenable.ConnectOther(std::bind(&CStimulationSettingsPage::EnableDisableControls, this));
}

void CStimulationSettingsPage::EnableDisableControls() {
	DBOUT(L"CStimulationSettings::EnableDisableControls\n");
	// Attention: ReadOnly ist !Enable
	const bool state = stimenable();
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