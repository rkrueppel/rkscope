#include "stdafx.h"
#include "XYZControlPage.h"

namespace scope {
	namespace gui {

CXYZControlPage::CXYZControlPage(parameters::Stage& _stageparams, ZeroButtons& _zerobuttons)
	: xpos_edit(_stageparams.xpos, true)
	, ypos_edit(_stageparams.ypos, true)
	, zpos_edit(_stageparams.zpos, true)
	, setzero_button(ZeroButtons.stage) {
}

BOOL CXYZControlPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	xpos_edit.AttachToDlgItem(GetDlgItem(IDC_POSX_EDIT));
	ypos_edit.AttachToDlgItem(GetDlgItem(IDC_POSY_EDIT));
	zpos_edit.AttachToDlgItem(GetDlgItem(IDC_POSZ_EDIT));
	setzero_button.AttachToDlgItem(GetDlgItem(IDC_SETZERO_BUTTON));

	SetMsgHandled(false);
	return 0;
}

}
}