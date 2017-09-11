#pragma once

#include "helpers/ScopeButton.h"
#include "helpers/Helpers.h"

namespace scope {
	namespace gui {

/** An adapted CButton control around a ScopeButton.
* @warning Needs message reflection in containing (dialog) window via REFLECT_NOTIFICATIONS(). */
class CScopeButtonCtrl :
	public CWindowImpl<CScopeButtonCtrl, CButton> {

protected:
	/** true after window creation */
	bool created;

	/** pointer to the underlying ScopeButton */
	ScopeButton& scope_button;

	/** The connection object for the control state (connection to the scope_button) */
	boost::signals2::connection stateconnection;

	/** stores state of shift key */
	bool shiftstate;

public:
	BEGIN_MSG_MAP_EX(CScopeButtonCtrl)
		REFLECTED_COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnClicked)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	/** Gets a pointer to the ScopeButton and connects control enabled/disabled state if desired */
	CScopeButtonCtrl(ScopeButton& _butt, const bool& _connectcontrolstate = true);

	/** Disconnects control state */
	~CScopeButtonCtrl();

	/** @name Called via Win32 messages
	* @{ */
	/** Call ScopeButton's click method */
	LRESULT OnClicked(UINT uNotifyCode, int nID, CWindow wndCtl);

	/** Switches to next control on return or tab and updates ScopeValue scope_val
	* Also sets state of shift key */
    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/** Resets state of shift key */
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/** @} */

	/** @name Handles connection to underlying ScopeButton
	* @{ */
	void Connect(ScopeButton& _butt, const bool& _connectcontrolstate = true);
	void Disconnect();
	/** @} */

	/** Attach the control to a dialog item */
	bool AttachToDlgItem(HWND hWnd);
	
	/** Sets the enabled/disabled state of the control */
	void Enable(const bool& state);
};

}

}
