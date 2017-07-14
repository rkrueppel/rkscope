#include "stdafx.h"
#include "PresetNameDlg.h"
#include "resource.h"

namespace scope {
	namespace gui {

CPresetNameDlg::CPresetNameDlg(std::shared_ptr<std::wstring> _name)
	: name(_name) {
}

LRESULT CPresetNameDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	edit.Attach(GetDlgItem(IDC_NAME_EDIT));
	CenterWindow(GetParent());
	edit.SetWindowText(name->c_str());
	edit.SetSel(0, name->length());
	edit.SetFocus();
	return FALSE;			// return FALSE, otherwise focus is not given to the edit control!
}

LRESULT CPresetNameDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	int ret = 0;
	int32_t length = edit.GetWindowTextLength()+1;		// +1 since terminating \0 is not counter
	std::vector<wchar_t> buff(length, L' ');				
	edit.GetWindowText(buff.data(), length);				// empty control gives only a \0
	name->assign(buff.begin(), buff.end()-1);

	if ( wID == IDCANCEL ) {
		*name = L"";
		ret = -1;
	}

	EndDialog(ret);
	return 0;
}

}}
