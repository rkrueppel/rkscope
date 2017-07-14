#include "stdafx.h"
#include "MovementPage.h"
#include "MovementSettingsSheet.h"

namespace scope {
	namespace gui {

BOOL CMovementPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Replace placeholder with scan settings property sheet
	HWND hWndPlaceHolder = GetDlgItem(IDC_MOVEMENTSETTINGS_HOLDER);
	::ShowWindow(hWndPlaceHolder, FALSE);	
	CRect rc;	
	::GetWindowRect(hWndPlaceHolder, &rc);		
	ScreenToClient(&rc);
	settings_sheet.Create(m_hWnd, 0, rc);

	SetMsgHandled(false);
	return 0;
}

}
}