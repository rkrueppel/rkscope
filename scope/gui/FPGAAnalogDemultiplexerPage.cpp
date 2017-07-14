#include "stdafx.h"
#include "FPGAAnalogDemultiplexerPage.h"

namespace scope {
	namespace gui {

CFPGAAnalogDemultiplexerPage::CFPGAAnalogDemultiplexerPage(parameters::InputsFPGAAnalogDemultiplexer* const _parameters)
	: tohostoverflowA1_led(&_parameters->diagnosis.ToHostOverflowA1)
	, tohostoverflowA2_led(&_parameters->diagnosis.ToHostOverflowA2)
	, interloopoverflow_led(&_parameters->diagnosis.InterloopOverflow)
	, interlooptimeout_led(&_parameters->diagnosis.InterloopTimeout)
	, acquiring_led(&_parameters->diagnosis.Acquiring, SCOPELED_OKONTRUE)
	, aioverrange_led(&_parameters->diagnosis.AIOverRange)
	, baselinech1_edit(&_parameters->BaselineCh1)
	, baselinech2_edit(&_parameters->BaselineCh2)
	, cutoffch1_edit(&_parameters->CutoffCh1)
	, cutoffch2_edit(&_parameters->CutoffCh2)
	, bitshiftch1a1_edit(&_parameters->BitshiftA1Ch1)
	, bitshiftch2a1_edit(&_parameters->BitshiftA1Ch2)
	, bitshiftch1a2_edit(&_parameters->BitshiftA2Ch1)
	, bitshiftch2a2_edit(&_parameters->BitshiftA2Ch2) 
	, hist_range1_edit(&scope_controller.GuiParameters.areas[0]->histrange) 
	, hist_range2_edit(&scope_controller.GuiParameters.areas[1]->histrange) 
	{
}

BOOL CFPGAAnalogDemultiplexerPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	tohostoverflowA1_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA1_BUTTON));
	tohostoverflowA2_led.AttachToDlgItem(GetDlgItem(IDC_TOHOSTOVERFLOWA2_BUTTON));
	interloopoverflow_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPOVERFLOW_BUTTON));
	interlooptimeout_led.AttachToDlgItem(GetDlgItem(IDC_INTERLOOPTIMEOUT_BUTTON));
	acquiring_led.AttachToDlgItem(GetDlgItem(IDC_ACQUIRING_BUTTON));
	aioverrange_led.AttachToDlgItem(GetDlgItem(IDC_OVERRANGE_BUTTON));
	baselinech1_edit.AttachToDlgItem(GetDlgItem(IDC_BASELINECH1_EDIT));
	baselinech2_edit.AttachToDlgItem(GetDlgItem(IDC_BASELINECH2_EDIT));
	cutoffch1_edit.AttachToDlgItem(GetDlgItem(IDC_CUTOFFCH1_EDIT));
	cutoffch2_edit.AttachToDlgItem(GetDlgItem(IDC_CUTOFFCH2_EDIT));
	bitshiftch1a1_edit.AttachToDlgItem(GetDlgItem(IDC_BITSHIFTA1CH1_EDIT));
	bitshiftch2a1_edit.AttachToDlgItem(GetDlgItem(IDC_BITSHIFTA1CH2_EDIT));
	bitshiftch1a2_edit.AttachToDlgItem(GetDlgItem(IDC_BITSHIFTA2CH1_EDIT));
	bitshiftch2a2_edit.AttachToDlgItem(GetDlgItem(IDC_BITSHIFTA2CH2_EDIT));
	hist_range1_edit.AttachToDlgItem(GetDlgItem(IDC_HISTRANGE1_EDIT));
	hist_range2_edit.AttachToDlgItem(GetDlgItem(IDC_HISTRANGE2_EDIT));
	return 0;
}

}}