#include "stdafx.h"
#include "MovementPage.h"
#include "MovementSettingsSheet.h"

namespace scope {
	namespace gui {
		
		CMovementPage::CMovementPage(
			std::vector<std::unique_ptr<parameters::BaseArea>>& _allareas
			, std::vector<FPUButtons>& _fpubuttons
			, const double& _masterfovsizex
			, const double& _masterfovsizey
			, config::XYZStageParametersType& _stageparams
			, ZeroButtons& _zerobuttons
		)
			: settings_sheet(_allareas, _fpubuttons, _masterfovsizex, _masterfovsizey, _stageparams, _zerobuttons)
		{
			
		}


		BOOL CMovementPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
			// Replace placeholder with scan settings property sheet
			HWND hWndPlaceHolder = GetDlgItem(IDC_MOVEMENTSETTINGS_HOLDER);
			::ShowWindow(hWndPlaceHolder, FALSE);	
			CRect rc;	
			::GetWindowRect(hWndPlaceHolder, &rc);		
			ScreenToClient(&rc);
			settings_sheet.Create(m_hWnd, 0, rc);

			SetMsgHandled(false);
			return 0;
		}

	}
}