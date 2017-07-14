#pragma once
#include "../resource.h"

namespace scope {
	namespace gui {

/** Simple dialog for choosing an area */
class CAreaChooseDlg
	: public CDialogImpl<CAreaChooseDlg> {

protected:
	/** buttons for area choosing */
	std::array<CButton, 4> buttons;

public:
	enum { IDD = IDD_AREACHOOSE };

	BEGIN_MSG_MAP(CAreaChooseDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_AREA1, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_AREA2, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_AREA3, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_AREA4, OnCloseCmd)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** Initializes the dialog */
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	/** @return the choosen area (0...3) or -1 if canceled */
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	/** @} */
};

}}
