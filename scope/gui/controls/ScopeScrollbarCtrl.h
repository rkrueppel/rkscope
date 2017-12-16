#pragma once

#include "helpers/ScopeNumber.h"

namespace scope {
	namespace gui {

/** An adapted CScrollB ar control around a ScopeNumber<double>.
* @warning Needs message reflection in containing (dialog) window via REFLECT_NOTIFICATIONS() */
class CScopeScrollbarCtrl
	: public CWindowImpl<CScopeScrollbarCtrl, CScrollBar> {

protected:
	/** true if window created */
	bool created;

	/** pointer to underlying ScopeValue */
	ScopeNumber<double>& scopenum;

	/** The connection object for the control state (connection to the scopenum rw state change) */
	boost::signals2::connection stateconnection;

	/** The connection object for the control state (connection to the scopenum value change) */
	boost::signals2::connection valueconnection;

	/** if != 0, then scopenum is changed by increment on click on scrollbar arrows, otherwise by range/100 */
	double smallincrement;

	/** if != 0, then scopenum is changed by largeincrement on click inside scrollbar, otherwise by range/20 */
	double largeincrement;

	/** stores state of shift key */
	bool shiftstate;

public:

	BEGIN_MSG_MAP_EX(CScopeScrollbarCtrl)
		MSG_OCM_VSCROLL(OnScroll)
		MSG_OCM_HSCROLL(OnScroll)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_UPDATECONTROL, OnUpdateControl)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()


	/** Gets a pointer to the ScopeNumber<double> and connects if desired
	* @param[in] _scopenum ScopeNumber to which the slider connects
	* @param[in] _smallincrement discrete increment of underlying ScopeNumber on scrollbar buttons or set to 0 for 1/100th of scopenum range
	* @param[in] _largeincrement discrete increment of underlying ScopeNumber on click into scrollbar or set to 0 for 1/20th of scopenum range
	* @param[in] _connectback change scrollbar value when ScopeNumber is changed
	* @param[in] _connectcontrolstate change scrollbar enabled/disabled state when read/write state of the ScopeNumber is changed */
	CScopeScrollbarCtrl(ScopeNumber<double>& _scopenum, const double& _smallincrement, const double& _largeincrement, const bool& _connectback = false, const bool& _connectcontrolstate = false);

	/** Disconnects everything */
	~CScopeScrollbarCtrl();

	/** Attach the control to a dialog item and set range and tick frequency */
	BOOL AttachToDlgItem(HWND hWnd);

	/** @name Called via Win32 messages
	* @{ */
	/** Switches to next control on return or tab and updates ScopeValue scope_val
	* Also sets state of shift key */
    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/** Resets state of shift key */
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/** Updates the ScopeNumber from the slider position */
	void OnScroll(int nSBCode, short nPos, CScrollBar pScrollBar);

	/** Worker for UpdateControl */
	LRESULT OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	// LRESULT OnUpdateValue(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); not needed, since we do OnScroll
	/** @}

	/** @name These send async Win32 messages
	* @{ */
	/** Updates the slider position from the ScopeNumber */
	void UpdateControl();
	/** @} */

	/** Sets the enabled/disabled control state */
	void SetState(const bool& state);

	/** Sets the small increment */
	void SetSmallIncrement(const double& incr);
};

}
}