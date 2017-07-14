#include "stdafx.h"
#include "FPGAResonanceScannerPage.h"
#include "parameters\Inputs.h"

namespace scope {
	namespace gui {
	
CFPGAResonanceScannerPage::CFPGAResonanceScannerPage(parameters::InputsFPGAResonanceScanner* const _parameters)
	: tohostoverflow1_led(&_parameters->diagnosis.ToHostOverflowCh1)
	, tohostoverflow2_led(&_parameters->diagnosis.ToHostOverflowCh2)
	, interloopoverflow_led(&_parameters->diagnosis.InterloopOverflow)
	, interlooptimeout_led(&_parameters->diagnosis.InterloopTimeout)
	, acquiring_led(&_parameters->diagnosis.Acquiring, SCOPELED_OKONTRUE)
	, aioverrange_led(&_parameters->diagnosis.AIOverRange)
	, bitshiftch1_edit(&_parameters->BitshiftCh1)
	, baselinech1_edit(&_parameters->BaselineCh1)
	, bitshiftch2_edit(&_parameters->BitshiftCh2)
	, baselinech2_edit(&_parameters->BaselineCh2) {
}

BOOL CFPGAResonanceScannerPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	tohostoverflow1_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOW1_BUTTON));
	tohostoverflow2_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOW2_BUTTON));
	interloopoverflow_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPOVERFLOW_BUTTON));
	interlooptimeout_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPTIMEOUT1_BUTTON));
	acquiring_led.AttachToDlgItem(GetDlgItem(IDC_ACQUIRING_BUTTON));
	aioverrange_led.AttachToDlgItem(GetDlgItem(IDC_OVERRANGE_BUTTON2));
	bitshiftch1_edit.AttachToDlgItem(GetDlgItem(IDC_BITSHIFTCH1_EDIT));
	baselinech1_edit.AttachToDlgItem(GetDlgItem(IDC_BASELINECH1_EDIT));
	bitshiftch2_edit.AttachToDlgItem(GetDlgItem(IDC_BITSHIFTCH2_EDIT));
	baselinech2_edit.AttachToDlgItem(GetDlgItem(IDC_BASELINECH2_EDIT));

	return 0;
}

}}