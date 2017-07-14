#include "stdafx.h"
#include "XYZControlPage.h"

namespace scope {
	namespace gui {

CXYZControlPage::CXYZControlPage(void)
	: xpos_edit(&scope_controller.GuiParameters.stage.xpos, true)
	, ypos_edit(&scope_controller.GuiParameters.stage.ypos, true)
	, zpos_edit(&scope_controller.GuiParameters.stage.zpos, true)
	, setzero_button(&scope_controller.StageZeroButton) {
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