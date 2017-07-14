#include "stdafx.h"
#include "AreaChooseDlg.h"
#include "../resource.h"

namespace scope {
	namespace gui {

LRESULT CAreaChooseDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	std::array<uint32_t, 4> controls = { IDC_AREA1, IDC_AREA2, IDC_AREA3, IDC_AREA4 };

	// Attach radio buttons to controls
	for ( uint32_t a = 0 ; a < buttons.size() ; a++ )
		buttons[a].Attach(GetDlgItem(controls[a]));

	// Disable non-used radio buttons
	for ( uint32_t a = static_cast<uint32_t>(lParam) ; a < buttons.size() ; a++ )
		buttons[a].EnableWindow(FALSE);

	// Focus on the first button
	buttons[0].SetFocus();

	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CAreaChooseDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	int ret = 0;
	switch ( wID ) {
		case IDC_AREA1:
			ret = 0;
			break;
		case IDC_AREA2:
			ret = 1;
			break;
		case IDC_AREA3:
			ret = 2;
			break;
		case IDC_AREA4:
			ret = 3;
			break;
		default:
			ret = -1;
	}

	EndDialog(ret);
	return 0;
}

}}
