#pragma once

#include "helpers/ScopeNumber.h"

namespace scope {
	namespace gui {

/** An adapted CUpDownCtrl control around a ScopeNumber.
* Do not use buddy features etc. in the resource editor, we rely solely on the ScopeNumber magic.
* @warning Needs message reflection in containing (dialog) window via REFLECT_NOTIFICATIONS() */
template <class ValT>
class CScopeUpDownCtrl
	: public CWindowImpl<CScopeUpDownCtrl<ValT>, CUpDownCtrl> {

protected:
	/** true if window created */
	bool created;

	/** pointer to the underlying ScopeNumber */
	ScopeNumber<ValT>* const scopeval;

	/** The connection object for the control state (connection to the scopenum rw state change) */
	boost::signals2::connection stateconnection;

	/** ScopeNumber is incremented/decremented by this value */
	const ValT increment;

	/** does the next OnUpdateControl increment (true) or decrement (false) */
	bool incdec;

public:
	BEGIN_MSG_MAP_EX(CScopeUpDownCtrl)
		MSG_OCM_NOTIFY(OnUpDown)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	/** Gets a pointer to the ScopeNumber and connects if desired
	* @param[in] _scopeval ScopeNumber to which the up down control connects (taken by reference)
	* @param[in] _increment discrete increment of underlying ScopeNumber, should be bigger than 0
	* @param[in] _connectback change ScopeNumber when up/down is clicked
	* @param[in] _connectcontrolstate change up/down enabled/disabled state when read/write state is set at the ScopeNumber */
	CScopeUpDownCtrl(ScopeNumber<ValT>* _scopeval, const ValT& _increment, const bool& _connectback = false, const bool& _connectcontrolstate = false)
		: scopeval(_scopeval)
		, increment(_increment) {
		// We do not use connectback since the control has no visible value (although an internal iPos, which we do not use)
		if ( _connectcontrolstate )
			stateconnection = scopeval->ConnectState(std::bind(&CScopeUpDownCtrl<ValT>::SetState, this, std::placeholders::_1));
	}

	/** Disconnects everything */
	~CScopeUpDownCtrl(void) {
		stateconnection.disconnect();
	}

	/** Attach the control to a dialog item */
	bool AttachToDlgItem(HWND hWnd) {
		if ( SubclassWindow(hWnd) ) {
			created = true;
			return true;
		}
		return false;
	}

	/** @name Called via Win32 messages
	* @{ */
	/** Checks if the reflected notify is for this window and an UDN_DELTAPOS and calls UpdateVale accordingly. */
	LRESULT OnUpDown(int idCtrl, LPNMHDR pnmh) {
		if ( pnmh->hwndFrom == m_hWnd ) {
			if ( pnmh->code == UDN_DELTAPOS ) {
				LPNMUPDOWN lpnmud = (LPNMUPDOWN)pnmh;
				if ( lpnmud->iDelta > 0 )
					incdec = true;
				else
					incdec = false;
				UpdateValue();
			}
		}
		return 0;
	}
	/** @} */

	/** Updates the ScopeValue by incrementing or decrementing */
	void UpdateValue() {
		if ( incdec )
			scopeval->Set(scopeval->Value()-increment);
		else
			scopeval->Set(scopeval->Value()+increment);
	}

	/** Sets the enabled/disabled control state */
	void SetState(const bool& state) {
		if ( created )
			EnableWindow(state);
	}
};

}

}
