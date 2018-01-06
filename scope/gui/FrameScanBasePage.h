#pragma once

#include "NoScanBasePage.h"
#include "ThirdParty/ToolTipDialog.h"
#include "controls/ScopeSliderCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controls/ScopeUpDownCtrl.h"
#include "controls/ScopeScrollbarCtrl.h"
#include "controls/ScopeFOVDiagram.h"
#include "scanmodes/ScannerVectorFrameBasic.h"
#include "parameters/Area.h"
#include "TheScopeButtons.h"
#include "resource.h"

namespace scope {
namespace gui {

		/** Base class for all frame scan property pages.
		* Uses CToolTipDialog for displaying help tooltips. Add strings with the same ID as a control for help to that control. */
		class CFrameScanBasePage
			: public CNoScanBasePage {

		protected:
			/** Connection object to the ScopeController::ReadOnlyWhileScanning for setting controls RW state */ 
			//boost::signals2::connection rwstateconnection;

			/** combo box with predefined scan options (resolution,dwell time, scanner delay etc etc) */
			CComboBox presets_combo;

			/** save preset button, only here for easy disabling during scanning */
			CButton savepreset_button;

			/** delete preset button, only here for easy disabling during scanning */
			CButton deletepreset_button;

			/** zoom scrollbar control */
			CScopeScrollbarCtrl zoom_scroll;

			/** zoom edit control */
			CScopeEditCtrl<double> zoom_edit;

			/** x resolution edit control */
			CScopeEditCtrl<uint32_t> xres_edit;

			/** y resolution edit control */
			CScopeEditCtrl<uint32_t> yres_edit;

			/** x aspect ratio edit control */
			CScopeEditCtrl<double> xaspectratio_edit;

			/** y aspect ratio edit control */
			CScopeEditCtrl<double> yaspectratio_edit;

			/** number of averages edit control */
			CScopeEditCtrl<uint32_t> averages_edit;

			/** scannerdelay in us */
			CScopeEditCtrl<double> scannerdelay_edit;

			/** offset of the x scanner center position (-1..1, relative to maximum FOV) */
			CScopeEditCtrl<double> xoffset_edit;

			/** offset of the y scanner center position (-1..1, relative to maximum FOV) */
			CScopeEditCtrl<double> yoffset_edit;

			/** framerate edit control (edit disabled) */
			CScopeEditCtrl<double> framerate_edit;

			/** frametime edit control (edit disabled) */
			CScopeEditCtrl<double> frametime_edit;

			/** linerate edit control (edit disabled) */
			CScopeEditCtrl<double> linerate_edit;

			/** diagram displaying FOV positions */
			//CScopeFOVDiagram diagram;

		public:
			/** Dialog resource ID, do not forget to overwrite in derived class constructor! */
			enum { IDD = IDD_FRAMESCAN_SAW_PROPPAGE };

			CFrameScanBasePage(const uint32_t& _area, parameters::BaseArea* _areaparams, FPUButtons& _fpubuttons);

			/** Disconnect from ScopeController::ReadOnlyWhileScanning */
			virtual ~CFrameScanBasePage() {};

			BEGIN_MSG_MAP_EX(CFrameScanBasePage)	
				MSG_WM_INITDIALOG(OnInitDialog);
				COMMAND_HANDLER(IDC_SAVEPRESET_BUTTON, BN_CLICKED, OnPresetSave)
				COMMAND_HANDLER(IDC_DELETEPRESET_BUTTON, BN_CLICKED, OnPresetDelete)
				COMMAND_HANDLER(IDC_PRESETS_COMBO, CBN_SELCHANGE, OnPresetSelection)
				CHAIN_MSG_MAP(CNoScanBasePage)
				REFLECT_NOTIFICATIONS()
			END_MSG_MAP()

			/** @name Called via Win32 messages
			* @{ */
			BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

			virtual LRESULT OnPresetSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			virtual LRESULT OnPresetDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			virtual LRESULT OnPresetSelection(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			/** @} */

			/** disables controls other than connected buttons and values when acquisition is running */
			void SetReadOnlyWhileScanning(const bool& _ro) override;

			virtual void FillPresetCombo();

		};

	}

}