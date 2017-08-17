#pragma once

#include "helpers/ScopeNumber.h"

namespace scope {
	namespace gui {

#define SCOPELED_OKONTRUE 0
#define SCOPELED_WARNINGONTRUE 1

/** An adapted CBitmapButton control around a ScopeNumber<bool> to show true or false as green or red LED. */
class CScopeLEDCtrl
	: public CBitmapButtonImpl<CScopeLEDCtrl> {

protected:
	/** pointer to underlying ScopeNumber */
	ScopeNumber<bool>& scopebool;

	/** The connection object for the control state (connection to the scopenum value change) */
	boost::signals2::connection valueconnection;

	/** the LED style
	* - SCOPELED_OKONTRUE: led is green/ok on true and red on false
	* - SCOPELED_WARNINGONTRUE: led shows warning sign on true and green/ok on false */
	const uint32_t style;

	/** image for green LED */
	static CImageList imagegreen;

	/** image for red LED */
	static CImageList imagered;

	/** image for warning LED */
	static CImageList imagewarning;


public:
	/** Gets a pointer to the ScopeNumber<bool> and connects */
	CScopeLEDCtrl(ScopeNumber<bool>& _scopebool, const uint32_t& _style = SCOPELED_WARNINGONTRUE);

	/** Disconnects */
	~CScopeLEDCtrl();

	/** Sets LED according to value of scopebool */
	void UpdateControl();

	/** Sets LED to true */
	void SetTrue();

	/** Sets LED to false */
	void SetFalse();

	/** override of CBitmapButtonImpl DoPaint() in CBmpBtn class to erase background (otherwise weird overlay of icons) */
	void DoPaint(CDCHandle dc);

	/** Attach the control to a dialog item */
	bool AttachToDlgItem(HWND hWnd);
};

}
}