#pragma once

#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "../controllers/ScopeController.h"
#include "../resource.h"

namespace scope {
	namespace gui {

/** Property page for storage related options */
class CStorageSettingsPage :
	public CPropertyPageImpl<CStorageSettingsPage> {

protected:
	/** scope controller */
	scope::ScopeController scope_controller;

	/** Checkbox for automatic save option */
	CScopeCheckBoxCtrl autosave_checkbox;

	/** Checkbox for save live option */
	CScopeCheckBoxCtrl savelive_checkbox;

	/** Checkbox for use tiff tags option */
	CScopeCheckBoxCtrl usetifftags_checkbox;

	/** Checkbox for compress tiff option */
	CScopeCheckBoxCtrl compresstiff_checkbox;

	/** Edit control for storage folder */
	CScopeEditCtrl<std::wstring> folder_edit;


public:
	enum { IDD = IDD_STORAGE_PROPPAGE };

	CStorageSettingsPage();

	BEGIN_MSG_MAP(CStorageSettingsPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		COMMAND_HANDLER(IDC_FOLDER_BUTTON, BN_CLICKED, OnFolderButton)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** Handles dialog initialization */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

	/** Opens dialog for folder choosing */
	LRESULT OnFolderButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** @} */
};

}}