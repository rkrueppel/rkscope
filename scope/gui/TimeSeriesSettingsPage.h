#pragma once

#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeProgressCtrl.h"
#include "TheScopeButtons.h"
#include "TheScopeCounters.h"
#include "parameters/Runstates.h"
#include "parameters/Area.h"
#include "ThirdParty/ToolTipDialog.h"
#include "resource.h"

namespace scope {
	namespace gui {

		/** Property page for settings of simeseries scanning */
		class CTimeSeriesSettingsPage
			: public CPropertyPageImpl<CTimeSeriesSettingsPage>
			, public CToolTipDialog<CTimeSeriesSettingsPage> {

		protected:
			/** true if window is initialized */
			bool initialized;
			
			/** Reference to TheScope's gui parameter's Timeseries parameters */
			parameters::Timeseries& timeseriesparams;
			
			/** Reference to TheScope's gui parameter's Area parameters vector */
			std::vector<parameters::MasterArea>& masterareas;
			std::vector<parameters::SlaveArea>& slaveareas;

			/** start timeseries scanning */
			CScopeButtonCtrl start_timeseries_button;

			/** number of frames to scan */
			CScopeEditCtrl<uint32_t> frames_edit;

			/** progress indicator for frames of a single repeat */
			CScopeProgressCtrl frames_progress;

			/** is the first timeseries triggered */
			CScopeCheckBoxCtrl firsttriggered_check;

			/** are all timeseries triggered */
			CScopeCheckBoxCtrl alltriggered_check;

			/** do the frames and totaltime hold for all areas */
			CButton forallareas;

			/** the total time the timeseries will take */
			CScopeEditCtrl<double> totaltime_edit;

			/** How often should the timeseries be repeated */
			CScopeEditCtrl<uint32_t> repeats_edit;

			/** Progress indicator for repeats */
			CScopeProgressCtrl repeats_progress;

			/** Time in seconds between the end of one and the next timeseries */
			CScopeEditCtrl<double> betweenrepeats_edit;

			/** time in seconds for all timeseries */
			CScopeEditCtrl<double> overalltime_edit;

			/** list box for all planes */
			CListViewCtrl planes_list;
			
			/** Button for adding a plane */
			CButton addplane_button;
			
			/** Button for deleting a plane */
			CButton deleteplane_button;

		public:
			enum { IDD = IDD_TIMESERIES_PROPPAGE };

			CTimeSeriesSettingsPage(parameters::Timeseries& _timeseriesparams, RunButtons& _runbuttons, ScopeCounters<config::nmasters>& _counters, std::vector<parameters::MasterArea>& _masterareas, std::vector<parameters::SlaveArea>& _slaveareas);

			BEGIN_MSG_MAP_EX(CTimeSeriesSettingsPage)	
				MSG_WM_INITDIALOG(OnInitDialog);
				COMMAND_HANDLER(IDC_ADDPLANE_BUTTON, BN_CLICKED, OnAddPlane)
				COMMAND_HANDLER(IDC_DELETEPLANE_BUTTON, BN_CLICKED, OnDeletePlane)
				CHAIN_MSG_MAP(CToolTipDialog<CTimeSeriesSettingsPage>)
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