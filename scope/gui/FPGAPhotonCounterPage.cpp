#include "StdAfx.h"
#include "FPGAPhotonCounterPage.h"
#include "devices\InputsFPGA.h"

namespace scope {
	namespace gui {
	
CFPGAPhotonCounterPage::CFPGAPhotonCounterPage(parameters::InputsFPGAPhotonCounter* const _parameters)
	: myFPGA(nullptr)
	, tohostoverflow1_led(&_parameters->diagnosis.ToHostOverflowCh1)
	, tohostoverflow2_led(&_parameters->diagnosis.ToHostOverflowCh2)
	, interloopoverflow_led(&_parameters->diagnosis.InterloopOverflow)
	, interlooptimeout_led(&_parameters->diagnosis.InterloopTimeout)
	, acquiring_led(&_parameters->diagnosis.Acquiring, SCOPELED_OKONTRUE) {
}

BOOL CFPGAPhotonCounterPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	tohostoverflow1_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOW1_BUTTON));
	tohostoverflow2_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOW2_BUTTON));
	interloopoverflow_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPOVERFLOW_BUTTON));
	interlooptimeout_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPTIMEOUT1_BUTTON));
	acquiring_led.AttachToDlgItem(GetDlgItem(IDC_ACQUIRING_BUTTON));

	counthigh_button.Attach(GetDlgItem(IDC_COUNTHIGH_RADIO));
	countpulses_button.Attach(GetDlgItem(IDC_COUNTPULSES_RADIO));

	counthigh_button.SetCheck(BST_CHECKED);
	countpulses_button.SetCheck(BST_UNCHECKED);

	return 0;
}

LRESULT CFPGAPhotonCounterPage::OnCountModeChange(WORD code, WORD id, HWND wnd, BOOL&) {
	DBOUT(L"COUNTMODECHANGE");
	if ( id == IDC_COUNTHIGH_RADIO ) {
		//myFPGA->SetCountMode(true);
	}
	if ( id == IDC_COUNTPULSES_RADIO ) {
		//myFPGA->SetCountMode(false);
	}
	return 0;
}

}}