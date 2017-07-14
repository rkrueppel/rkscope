#include "stdafx.h"
#include "ScopeSliderCtrl.h"
#include "helpers/Helpers.h"

namespace scope {
	namespace gui {

CScopeSliderCtrl::CScopeSliderCtrl(ScopeNumber<double>* _scopenum, const double& _increment, const bool& _connectback, const bool& _connectcontrolstate)
	: created(false)
	, scopenum(_scopenum)
	, increment(_increment)
	, scalefactor((increment!=0)?increment:(scopenum->ul()-scopenum->ll()) / 100)
	, offset(scopenum->ll())
	, rangehigh((increment!=0)?static_cast<int32_t>((scopenum->ul()-scopenum->ll())/increment):100)
	, rangelow(0) {
	if ( _connectback )
		valueconnection = scopenum->ConnectGUI(std::bind(&CScopeSliderCtrl::UpdateControl, this));
	if ( _connectcontrolstate )
		stateconnection = scopenum->ConnectState(std::bind(&CScopeSliderCtrl::SetState, this, std::placeholders::_1));
}

CScopeSliderCtrl::~CScopeSliderCtrl() {
	valueconnection.disconnect();
	stateconnection.disconnect();
}

BOOL CScopeSliderCtrl::AttachToDlgItem(HWND hWnd) {
	if ( SubclassWindow(hWnd) ) {
		created = true;
		SetRange(static_cast<int32_t>(rangelow), static_cast<int32_t>(rangehigh), TRUE);
		if ( increment == 0 )
			SetTicFreq(10);				// if not incrementing have only 10 ticks (although we have values from 0...100)
		UpdateControl();
		return TRUE;
	}
	return FALSE;
}

void CScopeSliderCtrl::OnScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar) {
	// adjust scopenum on dragging the slider
	// nPos 0 at top, but bottom should be 0
	if ( nSBCode == SB_THUMBPOSITION )
		scopenum->Set(std::abs(static_cast<int32_t>(nPos)-rangehigh) * scalefactor + offset);
	// and on clicking into trackbar
	if ( (nSBCode == SB_PAGEUP) || (nSBCode == SB_PAGEDOWN) || (nSBCode == SB_PAGELEFT) || (nSBCode == SB_PAGERIGHT) )
		scopenum->Set(std::abs(GetPos()-rangehigh) * scalefactor + offset);
}

LRESULT CScopeSliderCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
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

LRESULT CScopeSliderCtrl::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	switch (wParam) {
	case VK_SHIFT:
		shiftstate = false;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CScopeSliderCtrl::OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	// nPos 0 at top, but bottom should be 0
	SetPos(round2i32(abs((scopenum->Value()-offset)/scalefactor-rangehigh)));
	return 0;
}

void CScopeSliderCtrl::UpdateControl() {
	::PostMessage(m_hWnd, WM_UPDATECONTROL, NULL, NULL);
}

void CScopeSliderCtrl::SetState(const bool& state) {
	if ( created )
		EnableWindow(state);
}

}

}