#include "stdafx.h"
#include "FrameScanBasePage.h"
#include "PresetNameDlg.h"

namespace scope {
	namespace gui {

		CFrameScanBasePage::CFrameScanBasePage(const uint32_t& _area, parameters::BaseArea& _areaparams, FPUButtons& _fpubuttons)
			: CNoScanBasePage(_area, _areaparams, _fpubuttons)
			, zoom_edit(_areaparams.FrameSaw()->zoom, true, true)
			, zoom_scroll(_areaparams.FrameSaw()->zoom, 0.1, 1, true, true)
			, xres_edit(_areaparams.FrameSaw()->xres, true, true)
			, yres_edit(_areaparams.FrameSaw()->yres, true, true)
			, xaspectratio_edit(_areaparams.FrameSaw()->xaspectratio, true, true)
			, yaspectratio_edit(_areaparams.FrameSaw()->yaspectratio, true, true)
			, averages_edit(_areaparams.daq.averages, true, true)
			, scannerdelay_edit(_areaparams.daq.scannerdelay, true, true)
			, xoffset_edit(_areaparams.FrameSaw()->xoffset, true, true)
			, yoffset_edit(_areaparams.FrameSaw()->yoffset, true, true)
			, framerate_edit(_areaparams.framerate, true, true)
			, frametime_edit(_areaparams.frametime, true, true)
			, linerate_edit(_areaparams.linerate, true, true)
			//, diagram(area, &scope_controller.GuiParameters)
		{
			
		}

		BOOL CFrameScanBasePage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
			// Call base class dialog initialization
			CNoScanBasePage::OnInitDialog(wndFocus, lInitParam);

			// attach the dialog controls to the edit objects
			presets_combo.Attach(GetDlgItem(IDC_PRESETS_COMBO));
			savepreset_button.Attach(GetDlgItem(IDC_SAVEPRESET_BUTTON));
			deletepreset_button.Attach(GetDlgItem(IDC_DELETEPRESET_BUTTON));
			zoom_edit.AttachToDlgItem(GetDlgItem(IDC_ZOOM_EDIT));
			zoom_scroll.AttachToDlgItem(GetDlgItem(IDC_ZOOM_SCROLL));
			xres_edit.AttachToDlgItem(GetDlgItem(IDC_PIXELX_EDIT));
			yres_edit.AttachToDlgItem(GetDlgItem(IDC_PIXELY_EDIT));
			xaspectratio_edit.AttachToDlgItem(GetDlgItem(IDC_ASPECTRATIOX_EDIT));
			yaspectratio_edit.AttachToDlgItem(GetDlgItem(IDC_ASPECTRATIOY_EDIT));
			averages_edit.AttachToDlgItem(GetDlgItem(IDC_AVERAGES_EDIT));
			scannerdelay_edit.AttachToDlgItem(GetDlgItem(IDC_SCANNERDELAY_EDIT));
			xoffset_edit.AttachToDlgItem(GetDlgItem(IDC_XOFFSET_EDIT));
			yoffset_edit.AttachToDlgItem(GetDlgItem(IDC_YOFFSET_EDIT));
			framerate_edit.AttachToDlgItem(GetDlgItem(IDC_FRAMERATE));
			frametime_edit.AttachToDlgItem(GetDlgItem(IDC_FRAMETIME));
			linerate_edit.AttachToDlgItem(GetDlgItem(IDC_LINERATE));

			//diagram.AttachToDlgItem(GetDlgItem(IDC_FPUDIAGRAM));

			// Set before by CNoScanBasePage::OnInitDialog
			initialized = initialized && true;

			SetMsgHandled(true);

			return 0;
		}

		LRESULT CFrameScanBasePage::OnPresetSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
			std::shared_ptr<std::wstring> strname = std::make_shared<std::wstring>(L"PresetName");
			CPresetNameDlg dlg(strname);
			if ( dlg.DoModal(::GetActiveWindow()) == 0 ) {
				//scope_controller.GuiParameters.areas[area]->SaveToPreset(*strname);
				FillPresetCombo();
				presets_combo.SelectString(0, strname->c_str());
			}
			return 0;
		}

		LRESULT CFrameScanBasePage::OnPresetDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
			int32_t sel = presets_combo.GetCurSel();
			if ( sel > 0 ) {
				std::vector<wchar_t> buff(presets_combo.GetLBTextLen(sel)+1, L' ');			// +1 since GetLBTextLen does not count terminating \0 ... sigh...
				presets_combo.GetLBText(sel, buff.data());
				//scope_controller.GuiParameters.areas[area]->DeletePreset(std::wstring(buff.begin(), buff.end()-1));
				FillPresetCombo();
			}
			return 0;
		}

		LRESULT CFrameScanBasePage::OnPresetSelection(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
			int32_t sel = presets_combo.GetCurSel();
			if ( sel > 0 ) {
				std::vector<wchar_t> buff(presets_combo.GetLBTextLen(sel)+1, L' ');			// +1 since GetLBTextLen does not count terminating \0 ... sigh...
				presets_combo.GetLBText(sel, buff.data());
				//scope_controller.GuiParameters.areas[area]->LoadFromPreset(std::wstring(buff.begin(), buff.end()-1));
			}
			return 0;
		}

		void CFrameScanBasePage::SetReadOnlyWhileScanning(const bool& _ro) {
			// Do not call window procedures on non-initialized dialog
			if ( initialized ) {
				// Check if to disable or to enable now
				// Since the other controls are connected to GuiParameters they are taken care of automatically
				presets_combo.EnableWindow(!_ro);
				savepreset_button.EnableWindow(!_ro);
				deletepreset_button.EnableWindow(!_ro);
			}
		}

		void CFrameScanBasePage::FillPresetCombo() {
			// Clear the box
			presets_combo.ResetContent();
			
			// custom preset allows defining new presets etc
			presets_combo.AddString(L"Custom...");
			
			// load the presets' names into the combo box
			//for ( const auto& p : scope_controller.GuiParameters.areas[area]->Currentframe().presets )
			//	presets_combo.AddString(p->name().c_str());

			// select the first preset (the custom one)
			presets_combo.SetCurSel(0);
		}


	}

}