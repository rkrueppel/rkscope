#pragma once

#include "helpers/ScopeValue.h"
#include "ScopeDefines.h"

namespace scope {
	namespace gui {

/** An adapted CButton control as checkbox around a ScopeValue<bool>.
* @warning Needs message reflection in containing (dialog) window via REFLECT_NOTIFICATIONS() */
class CScopeCheckBoxCtrl :
	public CWindowImpl<CScopeCheckBoxCtrl, CButton> {

protected:
	/** true after window creation */
	bool created;

	/** pointer to underlying ScopeValue */
	ScopeValue<bool>* const scope_val;

	/** stores state of shift key */
	bool shiftstate;

	/** The connection object for the control state (connection to the scopenum rw state change) */
	boost::signals2::connection stateconnection;

	/** The connection object for the control state (connection to the scope_val value change) */
	boost::signals2::connection valueconnection;

public:
	BEGIN_MSG_MAP_EX(CScopeCheckBoxCtrl)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_UPDATECONTROL, OnUpdateControl)
		MSG_OCM_COMMAND(OnReflectedCommand)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	/** Gets a pointer to the ScopeValue<bool> and connects if desired
	* @param[in] _scopeval ScopeValue<bool> to which the check box control connects (taken by reference)
	* @param[in] _connectback change check box check when ScopeValue is changed
	* @param[in] _connectcontrolstate change check box enabled/disabled state when read/write state of the ScopeValue is changed */
	CScopeCheckBoxCtrl(ScopeValue<bool>* _scopeval, const bool& _connectback = false, const bool& _connectcontrolstate = false);

	/** Disconnects */
	~CScopeCheckBoxCtrl();

	/** Attach the control to a dialog item */
	bool AttachToDlgItem(HWND hWnd);

	/** @name Called via Win32 messages
	* @{ */

	/** Updates value on reflected BN_CLICKED */
	void OnReflectedCommand(UINT uNotifyCode, int nID, CWindow wndCtl);

	/** Switches to next control on return or tab and updates ScopeValue scope_val
	* Also sets state of shift key */
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/** Resets state of shift key */
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/** Update scope_val when leaving focus */
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/** Worker for UpdateControl */
	LRESULT OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/** @} */

	/** @name These send async Win32 messages
	* @{ */
	/** Updates the string inside the control from the ScopeValues' value. Do this asynchronously, thus Gui thread
	does the updating in OnUpdateControl and ScopeController (or whatever) thread can immediatly continue. */
	void UpdateControl();

	/** Updates the ScopeValue from the string inside the control */
	void UpdateValue();
	/** @} */

	/** Sets the enabled/disabled state of the control */
	void SetState(const bool& state);
};

}

}
