#include "stdafx.h"
#include "ScopeScrollbarCtrl.h"
#include "helpers/ScopeNumber.h"
#include "helpers/helpers.h"

namespace scope {
	namespace gui {

CScopeScrollbarCtrl::CScopeScrollbarCtrl(ScopeNumber<double>* _scopenum, const double& _smallincrement, const double& _largeincrement, const bool& _connectback, const bool& _connectcontrolstate)
	: created(false)
	, scopenum(_scopenum)
	, smallincrement((_smallincrement==0.0)?(scopenum->ul()-scopenum->ll()) / 100:_smallincrement)
	, largeincrement((_largeincrement==0.0)?(scopenum->ul()-scopenum->ll()) / 20:_largeincrement) {
	if ( _connectback )
		valueconnection = scopenum->ConnectGUI(std::bind(&CScopeScrollbarCtrl::UpdateControl, this));
	if ( _connectcontrolstate )
		stateconnection = scopenum->ConnectState(std::bind(&CScopeScrollbarCtrl::SetState, this, std::placeholders::_1));
}

CScopeScrollbarCtrl::~CScopeScrollbarCtrl() {
	valueconnection.disconnect();
	stateconnection.disconnect();
}

BOOL CScopeScrollbarCtrl::AttachToDlgItem(HWND hWnd) {
	if ( SubclassWindow(hWnd) ) {
		created = true;
		SetScrollRange(0, 100, TRUE);
		UpdateControl();
		return TRUE;
	}
	return FALSE;
}

void CScopeScrollbarCtrl::OnScroll(int nSBCode, short nPos, CScrollBar pScrollBar) {
	// adjust scopenum on dragging the slider
	// nPos 0 at top, but bottom should be 0, thus 100-nPos
	if ( nSBCode == SB_THUMBPOSITION )
		scopenum->Set(scopenum->ll() + static_cast<double>(100 - nPos) * (scopenum->ul()-scopenum->ll())/100 );
	// on clicking on the arrows at the end of the scrollbar
	if ( (nSBCode == SB_LINEUP) || (nSBCode == SB_LINELEFT) )
		scopenum->Set(scopenum->Value() + smallincrement);
	if ( (nSBCode == SB_LINEDOWN) || (nSBCode == SB_LINERIGHT) )
		scopenum->Set(scopenum->Value() - smallincrement);
	// and on clicking into scrollbar
	if ( (nSBCode == SB_PAGEUP) ||  (nSBCode == SB_PAGELEFT) )
		scopenum->Set(scopenum->Value() + largeincrement);
	if ( (nSBCode == SB_PAGEDOWN) || (nSBCode == SB_PAGERIGHT) )
		scopenum->Set(scopenum->Value() - largeincrement);
}

LRESULT CScopeScrollbarCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	switch (wParam) {
	case VK_RETURN:
	case VK_TAB:
		::PostMessage(GetParent(), WM_NEXTDLGCTL, shiftstate, 0L);
		return FALSE;
	case VK_SHIFT:
		shiftstate = true;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CScopeScrollbarCtrl::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	switch (wParam) {
	case VK_SHIFT:
		shiftstate = false;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CScopeScrollbarCtrl::OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	// nPos 0 at top, but bottom should be 0, thus 100-
	SetScrollPos(100 - round2i32( (scopenum->Value()-scopenum->ll()) / ((scopenum->ul()-scopenum->ll())/100) ) );
	return 0;
}

void CScopeScrollbarCtrl::UpdateControl() {
	::PostMessage(m_hWnd, WM_UPDATECONTROL, NULL, NULL);
}

void CScopeScrollbarCtrl::SetState(const bool& state) {
	if ( created )
		EnableWindow(state);
}

void CScopeScrollbarCtrl::SetSmallIncrement(const double& incr) {
	smallincrement = (incr==0.0)?(scopenum->ul()-scopenum->ll()) / 100:incr;
}

}

}