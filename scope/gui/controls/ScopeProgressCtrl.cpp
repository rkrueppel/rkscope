#include "stdafx.h"
#include "ScopeProgressCtrl.h"
#include "helpers\helpers.h"

namespace scope {
	namespace gui {


CScopeProgressCtrl::CScopeProgressCtrl(ScopeNumber<double>& _scopenum)
	: created(false)
	, scopenum(_scopenum) {
	valueconnection = scopenum.ConnectGUI(std::bind(&CScopeProgressCtrl::UpdateControl, this));
}

CScopeProgressCtrl::~CScopeProgressCtrl() {
	valueconnection.disconnect();
}

BOOL CScopeProgressCtrl::AttachToDlgItem(HWND hWnd) {
	if ( SubclassWindow(hWnd) ) {
		created = true;
		SetRange(0, 100);
		UpdateControl();
		return TRUE;
	}
	return FALSE;
}

LRESULT CScopeProgressCtrl::OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	// Progress bar goes from 0 to 100
	SetPos( round2i32(scopenum.Value()/(scopenum.ul()-scopenum.ll()) * 100.0) );
	return 0;
}

void CScopeProgressCtrl::UpdateControl() {
	::PostMessage(m_hWnd, WM_UPDATECONTROL, NULL, NULL);
}

}

}