#include "stdafx.h"
#include "FPGANoiseOutputPage.h"

namespace scope {
	namespace gui {

CFPGANoiseOutputPage::CFPGANoiseOutputPage(parameters::InputsFPGANoiseOutput* const _parameters)
	: tohostoverflowA1Ch1_led(&_parameters->diagnosis.ToHostOverflowA1Ch1)
	, tohostoverflowA1Ch2_led(&_parameters->diagnosis.ToHostOverflowA1Ch2)
	, tohostoverflowA2Ch1_led(&_parameters->diagnosis.ToHostOverflowA2Ch1)
	, tohostoverflowA2Ch2_led(&_parameters->diagnosis.ToHostOverflowA2Ch2)
	, acquiring_led(&_parameters->diagnosis.Acquiring, SCOPELED_OKONTRUE) {
}

BOOL CFPGANoiseOutputPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	tohostoverflowA1Ch1_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA1CH1_BUTTON));
	tohostoverflowA1Ch2_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA1CH2_BUTTON));
	tohostoverflowA2Ch1_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA2CH1_BUTTON));
	tohostoverflowA2Ch2_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA2CH2_BUTTON));
	acquiring_led.AttachToDlgItem(GetDlgItem(IDC_ACQUIRING_BUTTON));

	return 0;
}

}}