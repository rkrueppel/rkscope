#pragma once

#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeProgressCtrl.h"
#include "TheScopeButtons.h"
#include "TheScopeCounters.h"
#include "parameters/Area.h"
#include "parameters/Runstates.h"
#include "ThirdParty/ToolTipDialog.h"
#include "resource.h"

namespace scope {
	struct ScopeControllerCounters;
}

namespace scope {
	namespace gui {

		/** Property page for settings of behavior triggered scanning */
		class CBehaviorSettingsPage
			: public CPropertyPageImpl<CBehaviorSettingsPage>
			, public CToolTipDialog<CBehaviorSettingsPage> {

		protected:
			/** true if window is initialized */
			bool initialized;
			
			/** start behavior triggered scanning */
			CScopeButtonCtrl start_behavior_button;

			/** Reference to TheScope's gui parameter's Behavior parameters */
			parameters::Behavior& behaviorparameters;
			
			/** Reference to TheScope's gui parameter's Area parameters vector */
			std::vector<parameters::Area>& areaparamsvec;

			/** list box for all planes */
			CListViewCtrl planes_list;

			/** Edit control (disabled) that displays current framecount of curren trial */
			CScopeEditCtrl<double> framecount_edit;

			/** Edit control (disabled) that displays elapsed total time since start of first trial */
			CScopeEditCtrl<double> totaltime_edit;

			/** Edit control (disabled) that displays current trial number */
			CScopeEditCtrl<double> trialcount_edit;
			
			/** Button for adding a plane */
			CButton addplane_button;
			
			/** Button for deleting a plane */
			CButton deleteplane_button;

		public:
			enum { IDD = IDD_BEHAVIOR_PROPPAGE };

			CBehaviorSettingsPage(RunButtons& _runbuttons, ScopeCounters& _scopecounters, parameters::Behavior& _behaviorparameters, std::vector<parameters::Area>& _areaparamsvec);

			BEGIN_MSG_MAP_EX(CBehaviorSettingsPage)	
				MSG_WM_INITDIALOG(OnInitDialog);
				COMMAND_HANDLER(IDC_ADDPLANE_BUTTON, BN_CLICKED, OnAddPlane)
				COMMAND_HANDLER(IDC_DELETEPLANE_BUTTON, BN_CLICKED, OnDeletePlane)
				CHAIN_MSG_MAP(CToolTipDialog<CBehaviorSettingsPage>)
				REFLECT_NOTIFICATIONS()
			END_MSG_MAP()

			/** @name Called via Win32 messages
			* @{ */
			/** Handles dialog initialization */
			BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
			LRESULT OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			LRESULT OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			/** @} */

			/** updates the list view ctrl on add/delete plane */
			void UpdatePlanesList();
			
			/** disables controls other than connected buttons and values when acquisition is running */
			void SetReadOnlyWhileScanning(const bool& _ro);
		};

	}
}