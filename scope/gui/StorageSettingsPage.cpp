#include "StdAfx.h"
#include "StorageSettingsPage.h"

namespace scope {
	namespace gui {

CStorageSettingsPage::CStorageSettingsPage()
	: folder_edit(&scope_controller.GuiParameters.storage.folder, true, true)
	, autosave_checkbox(&scope_controller.GuiParameters.storage.autosave, true, true)
	, savelive_checkbox(&scope_controller.GuiParameters.storage.savelive, true, true)
	, usetifftags_checkbox(&scope_controller.GuiParameters.storage.usetifftags, true, true)
	, compresstiff_checkbox(&scope_controller.GuiParameters.storage.compresstiff, true, true) {
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
		scope_controller.GuiParameters.storage.folder = std::wstring(dlgFolder.m_szFolderPath);

	return 0;
}

}}
