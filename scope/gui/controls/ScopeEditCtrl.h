#pragma once

#include "helpers/ScopeValue.h"
#include "helpers/ScopeString.h"
#include "ScopeDefines.h"

namespace scope {
	namespace gui {

/** An adapted CEdit control around a ScopeValue.
* @warning Needs message reflection in containing (dialog) window via REFLECT_NOTIFICATIONS() */
template <class ValT>
class CScopeEditCtrl
	: public CWindowImpl<CScopeEditCtrl<ValT>, CEdit> {

protected:
	/** true after window creation */
	bool created;

	/** pointer to underlying ScopeValue */
	ScopeValue<ValT>* const scope_val;

	/** The connection object for the control state (connection to the scope_val rw state change) */
	boost::signals2::connection stateconnection;

	/** The connection object for the control state (connection to the scope_val value change) */
	boost::signals2::connection valueconnection;

	/** stores state of shift key */
	bool shiftstate;

public:
	BEGIN_MSG_MAP_EX(CScopeEditCtrl)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_UPDATECONTROL, OnUpdateControl)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	/*CScopeEditImpl()				Default constructor is dangerous!
		: m_shiftstate(false)
		, m_ScopeValPtr(nullptr)
	{}*/

	/** Gets a pointer to the ScopeValue and connects for value changes and read/write status changes if desired
	* @param[in] _scopeval ScopeValue to which the edit control connects (taken by reference)
	* @param[in] _connectback change edit control displayed value/text when ScopeValue is changed
	* @param[in] _connectcontrolstate change edit control enabled/disabled state when read/write state of the ScopeValue is changed */
	CScopeEditCtrl(ScopeValue<ValT>* _scopeval, const bool& _connectback = false, const bool& _connectcontrolstate = false)
		: created(false)
		, scope_val(_scopeval)
		, shiftstate(false){
		if ( _connectback )
			valueconnection = scope_val->ConnectGUI(std::bind(&CScopeEditCtrl<ValT>::UpdateControl, this));
		if ( _connectcontrolstate )
			stateconnection = scope_val->ConnectState(std::bind(&CScopeEditCtrl<ValT>::SetState, this, std::placeholders::_1));
	}

	/** Disconnects everything */
	 ~CScopeEditCtrl() {
		valueconnection.disconnect();			// Does not throw if not connected
		stateconnection.disconnect();
	}

	/** Attach the control to a dialog item */
	bool AttachToDlgItem(HWND hWnd) {
		if ( SubclassWindow(hWnd) ) {
			created = true;
			UpdateControl();
			return true;
		}
		return false;
	}

	/** @name Called via Win32 messages
	* @{ */

	/** For ignoring tab and return without beep */
	LRESULT OnChar(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled) {
		//ignore without a beep
		switch (wParam) {
		case '\t': // tab
		case '\r': //Carriage return
			return 0;
			break;
		}
		return DefWindowProc(uMsg, wParam, lParam);
	}

	/** Switches to next control on return or tab and updates ScopeValue scope_val
	* Also sets state of shift key */
    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		switch (wParam) {
		case VK_RETURN:
		case VK_TAB:
			UpdateValue();
			::PostMessage(GetParent(), WM_NEXTDLGCTL, shiftstate, 0L);
			return FALSE;
		case VK_SHIFT:
			shiftstate = true;
		}
		return DefWindowProc(uMsg, wParam, lParam);
	}

	/** Resets state of shift key */
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		switch (wParam) {
		case VK_SHIFT:
			shiftstate = false;
		}
		return DefWindowProc(uMsg, wParam, lParam);
	}

	/** Update scope_val when leaving focus */
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		UpdateValue();
		return DefWindowProc(uMsg, wParam, lParam);
	}

	/** Worker for UpdateControl (explicit specialization for ScopeValue<CString> below). */
	LRESULT OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		if ( scope_val != nullptr )
			SetWindowText(scope_val->ToChar().c_str());
		return 0;
	}
	/** @} */

	/** Updates the string inside the control from the ScopeValues' value. Do this asynchronously, thus Gui thread
	does the updating in OnUpdateControl and ScopeController (or whatever) thread can immediatly continue. */
	void UpdateControl() {
		::PostMessage(m_hWnd, WM_UPDATECONTROL, NULL, NULL);
	}

	/** Updates the ScopeValue from the string inside the control. */
	void UpdateValue() {
		if ( scope_val != nullptr ) {
			const int32_t length = GetWindowTextLength()+1;			// +1 since terminating \0 is not counter
			std::vector<WCHAR> buff(length, L' ');
			GetWindowText(buff.data(), length);					// empty control gives only a \0
			scope_val->SetFromString(buff.data());
		}
	}

	/** Sets the enabled/disabled state of the control */
	void SetState(const bool& state) {
		if ( created )
			EnableWindow(state);
	}
};


}

}