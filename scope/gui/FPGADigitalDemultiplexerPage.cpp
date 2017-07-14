#include "StdAfx.h"
#include "FPGADigitalDemultiplexerPage.h"

namespace scope {
	namespace gui {

CFPGADigitalDemultiplexerPage::CFPGADigitalDemultiplexerPage(parameters::InputsFPGADigitalDemultiplexer* const _parameters)
	: tohostoverflowA1Ch1_led(&_parameters->diagnosis.ToHostOverflowA1Ch1)
	, tohostoverflowA1Ch2_led(&_parameters->diagnosis.ToHostOverflowA1Ch2)
	, tohostoverflowA2Ch1_led(&_parameters->diagnosis.ToHostOverflowA2Ch1)
	, tohostoverflowA2Ch2_led(&_parameters->diagnosis.ToHostOverflowA2Ch2)
	, interloopoverflow_led(&_parameters->diagnosis.InterloopOverflow)
	, interlooptimeout_led(&_parameters->diagnosis.InterloopTimeout)
	, acquiring_led(&_parameters->diagnosis.Acquiring, SCOPELED_OKONTRUE)
	, laserfreq_edit(&_parameters->diagnosis.MeasuredLaserFreq) {
}

BOOL CFPGADigitalDemultiplexerPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	tohostoverflowA1Ch1_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA1CH1_BUTTON));
	tohostoverflowA1Ch2_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA1CH2_BUTTON));
	tohostoverflowA2Ch1_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA2CH1_BUTTON));
	tohostoverflowA2Ch2_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA2CH2_BUTTON));
	interloopoverflow_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPOVERFLOW_BUTTON));
	interlooptimeout_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPTIMEOUT_BUTTON));
	acquiring_led.AttachToDlgItem(GetDlgItem(IDC_ACQUIRING_BUTTON));
	laserfreq_edit.AttachToDlgItem(GetDlgItem(IDC_LASERFREQ_EDIT));

	return 0;
}


}}