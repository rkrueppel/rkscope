#pragma once
#include "../resource.h"

namespace scope {
	namespace gui {

/** Simple dialog for naming a preset*/
class CPresetNameDlg
	: public CDialogImpl<CPresetNameDlg> {

protected:
	/** the name edit control */
	CEdit edit;

	/** pointer to a string to write the name into */
	std::shared_ptr<std::wstring> const name;

public:
	enum { IDD = IDD_PRESETNAME_DIALOG };

	/** Get a reference to a CString to write the name into */
	CPresetNameDlg(std::shared_ptr<std::wstring> _name);

	BEGIN_MSG_MAP(CPresetNameDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** Initializes the dialog */
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	/** @return the choosen name or L"" if canceled */
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	/** @} */
};

}}
