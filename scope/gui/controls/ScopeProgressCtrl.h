#pragma once

#include "helpers/ScopeNumber.h"
#include "ScopeDefines.h"				// to get WM_UPDATECONTROL

namespace scope {
	namespace gui {

/** An adapted CProgressBarCtrl control around a ScopeNumber<double>. Progress bar is scaled from scopenum's lower to upper limit.
* @warning Needs message reflection in containing (dialog) window via REFLECT_NOTIFICATIONS() */
class CScopeProgressCtrl
	: public CWindowImpl<CScopeProgressCtrl, CProgressBarCtrl> {

protected:
	/** true if window created */
	bool created;

	/** Pointer to the underlying ScopeNumber */
	ScopeNumber<double>* const scopenum;

	/** The connection object for the control state (connection to the scopenum value change) */
	boost::signals2::connection valueconnection;

public:
	public:
	BEGIN_MSG_MAP_EX(CScopeProgressCtrl)
		MESSAGE_HANDLER(WM_UPDATECONTROL, OnUpdateControl)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	/** Gets a pointer to the ScopeNumber<double>, and connects
	* @param[in] _scopenum ScopeNumber to which the progress bar connects */
	CScopeProgressCtrl(ScopeNumber<double>* _scopenum);

	/** Disconnects */
	~CScopeProgressCtrl();

	/** Attach the control to a dialog item and set range */
	BOOL AttachToDlgItem(HWND hWnd);

	/** @name Called via Win32 messages
	* @{ */
	/** Worker for UpdateControl */
	LRESULT OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	// LRESULT OnUpdateValue(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); not needed, since we do OnScroll
	/** @}

	/** @name These send async Win32 messages
	* @{ */
	/** Updates the position from the ScopeNumber */
	void UpdateControl();
	/** @} */
};

}

}