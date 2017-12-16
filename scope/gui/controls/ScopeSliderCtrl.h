#pragma once

#include "helpers/ScopeNumber.h"

namespace scope {
	namespace gui {

/** An adapted CTrackBarCtrl control around a ScopeNumber<double>. Implements a slider control, either with incremental or continuous value changes.\n
* ScopeNumber = slider * scalefactor + offset viceversa slider = (ScopeNumber - offset) / scalefactor\n
* If not incrementing we have only 10 ticks (although we slider positions from 0...100)
* @warning Needs message reflection in containing (dialog) window via REFLECT_NOTIFICATIONS() */
class CScopeSliderCtrl
	: public CWindowImpl<CScopeSliderCtrl, CTrackBarCtrl> {

protected:
	/** true if window created */
	bool created;

	/** Pointer to the underlying ScopeNumber */
	ScopeNumber<double>* const scopenum;

	/** The connection object for the control state (connection to the scopenum rw state change) */
	boost::signals2::connection stateconnection;

	/** The connection object for the control state (connection to the scopenum value change) */
	boost::signals2::connection valueconnection;

	/** if != 0, then scopenum can only be changed by multiples of increment and the slider should display tick marks accordingly */
	const double increment;

	/** scalefactor is increment or range/100 (if not incrementing) */
	const double scalefactor;

	/** is ScopeNumber lower limit */
	const double offset;

	/** the upper end of the slider range (100 if not incrementing, otherwise range/increment) */
	const int32_t rangehigh;

	/** lower end of the slider range, usually 0 */
	const int32_t rangelow;

	/** stores state of shift key */
	bool shiftstate;

public:
	BEGIN_MSG_MAP_EX(CScopeSliderCtrl)
		MSG_OCM_VSCROLL(OnScroll)
		MSG_OCM_HSCROLL(OnScroll)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_UPDATECONTROL, OnUpdateControl)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	/** Gets a pointer to the ScopeNumber<double> and connects if desired
	* @param[in] _scopenum ScopeNumber to which the slider connects
	* @param[in] _increment discrete increment of underlying ScopeNumber or 0 for (nearly) continuous changing of the ScopeNumber (slider range is 0-100 and converted accordingly)
	* @param[in] _connectback change slider value when ScopeNumber is changed
	* @param[in] _connectcontrolstate change slider enabled/disabled state when read/write state is set at the ScopeNumber */
	CScopeSliderCtrl(ScopeNumber<double>* _scopenum, const double& _increment = 0, const bool& _connectback = false, const bool& _connectcontrolstate = false);

	/** Disconnects everything */
	~CScopeSliderCtrl();

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
	void OnScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);

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
};

}

}