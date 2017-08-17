#include "stdafx.h"
#include "DAQmxPage.h"

namespace scope {
	namespace gui {

CDAQmxPage::CDAQmxPage(parameters::InputsDAQmx& _parameters)
	: daqmxversion_edit(_parameters.diagnosis.daqmxversion) {
}

BOOL CDAQmxPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	daqmxversion_edit.AttachToDlgItem(GetDlgItem(IDC_DAQMXVERSION_EDIT));

	return 0;
}

}}