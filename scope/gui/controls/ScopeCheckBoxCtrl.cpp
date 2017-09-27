#include "StdAfx.h"
#include "ScopeCheckBoxCtrl.h"

namespace scope {
	namespace gui {

		CScopeCheckBoxCtrl::CScopeCheckBoxCtrl(ScopeValue<bool>& _scopeval, const bool& _connectback, const bool& _connectcontrolstate)
			: created(false)
			, scope_val(_scopeval)
			, shiftstate(false) {
			if ( _connectback )
				valueconnection = scope_val.ConnectGUI(std::bind(&CScopeCheckBoxCtrl::UpdateControl, this));
			if ( _connectcontrolstate )
				stateconnection = scope_val.ConnectState(std::bind(&CScopeCheckBoxCtrl::SetState, this, std::placeholders::_1));
		}

		CScopeCheckBoxCtrl::~CScopeCheckBoxCtrl() {
			valueconnection.disconnect();
			stateconnection.disconnect();
		}

		bool CScopeCheckBoxCtrl::AttachToDlgItem(HWND hWnd) {
			if ( SubclassWindow(hWnd) ) {
				created = true;
				UpdateControl();
				return true;
			}
			return false;
		}

		void CScopeCheckBoxCtrl::OnReflectedCommand(UINT uNotifyCode, int nID, CWindow wndCtl) {
			if ( uNotifyCode == BN_CLICKED ) {
				UpdateValue();
			}
		}

		LRESULT CScopeCheckBoxCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
			switch (wParam) {
			// space key automatically triggers OnClicked
			case VK_RETURN:
			case VK_TAB:
				//UpdateValue(); We do not need this, since value is updated on click or on space by other member functions
				::PostMessage(GetParent(), WM_NEXTDLGCTL, shiftstate, 0L);
				return FALSE;
			case VK_SHIFT:
				shiftstate = true;
			}
			return DefWindowProc(uMsg, wParam, lParam);
		}

		LRESULT CScopeCheckBoxCtrl::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
			switch (wParam) {
			case VK_SHIFT:
				shiftstate = false;
			}
			return DefWindowProc(uMsg, wParam, lParam);
		}

		LRESULT CScopeCheckBoxCtrl::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
			UpdateValue();
			return DefWindowProc(uMsg, wParam, lParam);
		}

		LRESULT CScopeCheckBoxCtrl::OnUpdateControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
			bool val = scope_val.Value();
			if ( val )
				SetCheck(BST_CHECKED);
			else
				SetCheck(BST_UNCHECKED);
			return 0;
		}

		void CScopeCheckBoxCtrl::UpdateControl() {
			::PostMessage(m_hWnd, WM_UPDATECONTROL, NULL, NULL);
		}

		void CScopeCheckBoxCtrl::UpdateValue() {
			UINT state = GetCheck();
			if ( state == BST_CHECKED )
				scope_val.Set(true);
			if ( state == BST_UNCHECKED )
				scope_val.Set(false);
		}

		void CScopeCheckBoxCtrl::SetState(const bool& state) {
			if ( created )
				EnableWindow(state);
		}

}

}