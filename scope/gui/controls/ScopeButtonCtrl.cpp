#include "StdAfx.h"
#include "ScopeButtonCtrl.h"

namespace scope {
	namespace gui {

CScopeButtonCtrl::CScopeButtonCtrl(ScopeButton& _butt, const bool& _connectcontrolstate)
	: created(false)
	, scope_button(_butt) {
	// Connect and enable/disable
	if ( _connectcontrolstate ) {
		stateconnection = scope_button.ConnectControlState(std::bind(&CScopeButtonCtrl::Enable, this, std::placeholders::_1));
		Enable(scope_button.Enabled());
	}
}


CScopeButtonCtrl::~CScopeButtonCtrl() {
	stateconnection.disconnect();
}

bool CScopeButtonCtrl::AttachToDlgItem(HWND hWnd) {
	if ( SubclassWindow(hWnd) ) {
		created = true;
		return true;
	}
	return false;
}

void CScopeButtonCtrl::Connect(ScopeButton& _butt, const bool& _connectcontrolstate) {
	std::lock_guard<std::mutex> lock(mutex);
	scope_button = _butt;
	// Connect and enable/disable
	if ( _connectcontrolstate ) {
		stateconnection.disconnect();
		stateconnection = scope_button.ConnectControlState(std::bind(&CScopeButtonCtrl::Enable, this, std::placeholders::_1));
		Enable(scope_button.Enabled());
	}
}

void CScopeButtonCtrl::Disconnect() {
	std::lock_guard<std::mutex> lock(mutex);
	stateconnection.disconnect();			// Does not throw if not connected
	///scope_button = nullptr;
}

LRESULT CScopeButtonCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
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

LRESULT CScopeButtonCtrl::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	switch (wParam) {
	case VK_SHIFT:
		shiftstate = false;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CScopeButtonCtrl::OnClicked(UINT uNotifyCode, int nID, CWindow wndCtl) {
	std::lock_guard<std::mutex> lock(mutex);
	if ( scope_button != nullptr )
		scope_button.Click();
	return 0;
}
void CScopeButtonCtrl::Enable(const bool& state) {
	if (created ) {
		DBOUT(L"CScopeButtonCtrl enable/disable " << state);
		EnableWindow(state);
	}
}

}

}