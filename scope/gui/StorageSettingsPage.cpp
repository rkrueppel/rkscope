#include "StdAfx.h"
#include "StorageSettingsPage.h"

namespace scope {
	namespace gui {

CStorageSettingsPage::CStorageSettingsPage(parameters::Storage& _storageparams)
	: folder(_storageparams.folder)
	, folder_edit(_storageparams.folder, true, true)
	, autosave_checkbox(_storageparams.autosave, true, true)
	, savelive_checkbox(_storageparams.savelive, true, true)
	, usetifftags_checkbox(_storageparams.usetifftags, true, true)
	, compresstiff_checkbox(_storageparams.compresstiff, true, true) {
}

BOOL CStorageSettingsPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	folder_edit.AttachToDlgItem(GetDlgItem(IDC_FOLDER_TEXT));
	autosave_checkbox.AttachToDlgItem(GetDlgItem(IDC_AUTOSAVE_CHECK));
	savelive_checkbox.AttachToDlgItem(GetDlgItem(IDC_SAVELIVE_CHECK));
	usetifftags_checkbox.AttachToDlgItem(GetDlgItem(IDC_USETIFFTAGS));
	compresstiff_checkbox.AttachToDlgItem(GetDlgItem(IDC_COMPRESSTIFF));

	SetMsgHandled(false);
	return 0;
}

LRESULT CStorageSettingsPage::OnFolderButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	CFolderDialog dlgFolder(NULL, L"Pick a folder", BIF_RETURNONLYFSDIRS);

	// Open folder dialog and save result into GuiParameters
	if ( dlgFolder.DoModal() == IDOK )
		folder = std::wstring(dlgFolder.m_szFolderPath);

	return 0;
}

}}
