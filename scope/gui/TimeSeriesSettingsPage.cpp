#include "StdAfx.h"
#include "TimeSeriesSettingsPage.h"

namespace scope {
	namespace gui {

		CTimeSeriesSettingsPage::CTimeSeriesSettingsPage(parameters::Timeseries& _timeseriesparams, RunButtons& _runbuttons, ScopeCounters<config::nmasters>& _counters, std::vector<parameters::MasterArea>& _masterareas, std::vector<parameters::SlaveArea>& _slaveareas)
			: CToolTipDialog(TTS_NOPREFIX)
			, initialized(false)
			, timeseriesparams(_timeseriesparams)
			, masterareas(_masterareas)
			, slaveareas(_slaveareas)
			, start_timeseries_button(_runbuttons.starttimeseries)
			, frames_edit(_timeseriesparams.frames[AreaTypeHelper::Master][0], true, true)
			, frames_progress(_counters.framecounter[0])
			, firsttriggered_check(_timeseriesparams.triggered, true, true)
			, alltriggered_check(_timeseriesparams.alltriggered, true, true)
			, totaltime_edit(_timeseriesparams.totaltimes[AreaTypeHelper::Master][0], true, true)
			, repeats_edit(_timeseriesparams.repeats, true, true)
			, repeats_progress(_counters.repeatcounter)
			, betweenrepeats_edit(_timeseriesparams.betweenrepeats, true, true)
			, overalltime_edit(_timeseriesparams.overalltime, true, true) {
		}

		BOOL CTimeSeriesSettingsPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
			start_timeseries_button.AttachToDlgItem(GetDlgItem(IDC_STARTTIMESERIES_BUTTON));
			frames_edit.AttachToDlgItem(GetDlgItem(IDC_FRAMES_EDIT));
			frames_progress.AttachToDlgItem(GetDlgItem(IDC_FRAMES_PROGRESS));
			firsttriggered_check.AttachToDlgItem(GetDlgItem(IDC_FIRSTTRIGGERED));
			alltriggered_check.AttachToDlgItem(GetDlgItem(IDC_ALLTRIGGERED));
			totaltime_edit.AttachToDlgItem(GetDlgItem(IDC_TOTALTIME_EDIT));
			repeats_edit.AttachToDlgItem(GetDlgItem(IDC_REPEATS_EDIT));
			repeats_progress.AttachToDlgItem(GetDlgItem(IDC_REPEATS_PROGRESS));
			betweenrepeats_edit.AttachToDlgItem(GetDlgItem(IDC_BETWEENREPEATS_EDIT));
			overalltime_edit.AttachToDlgItem(GetDlgItem(IDC_OVERALLTIME_EDIT));
			
			addplane_button.Attach(GetDlgItem(IDC_ADDPLANE_BUTTON));
			deleteplane_button.Attach(GetDlgItem(IDC_DELETEPLANE_BUTTON));

			planes_list.Attach(GetDlgItem(IDC_PLANES_LIST));
			planes_list.InsertColumn(0, L"Plane", 0, 40);
			CString label(L"");
			for ( uint32_t a = 0 ; a < masterareas.size() ; a++ ) {
				label.Format(L"FastZ M%d", a+1);
				planes_list.InsertColumn(1+2*a, label, 0, 60);
				label.Format(L"Pockels M%d", a+1);
				planes_list.InsertColumn(2+2*a, label, 0, 60);
			}
			for (uint32_t a = 0; a < slaveareas.size(); a++) {
				label.Format(L"FastZ S%d", a + 1);
				planes_list.InsertColumn(1 + 2 * a, label, 0, 60);
				label.Format(L"Pockels S%d", a + 1);
				planes_list.InsertColumn(2 + 2 * a, label, 0, 60);
			}
			UpdatePlanesList();
			
			initialized = true;

			SetMsgHandled(false);
			return 0;
		}

		LRESULT CTimeSeriesSettingsPage::OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
			DBOUT(L"CTimeSeriesSettingsPage::OnAddPlane");

			// Get plane information for every area and add to timeseries plane vectors
			std::array < std::vector<parameters::PlaneProperties>, NAreaTypes> planes{std::vector<parameters::PlaneProperties>(masterareas.size()), std::vector<parameters::PlaneProperties>(slaveareas.size()) };
			for ( uint32_t a = 0 ; a < planes[AreaTypeHelper::Master].size(); a++ ) {
				planes[AreaTypeHelper::Master][a].position = masterareas[a].Currentframe().fastz();
				planes[AreaTypeHelper::Master][a].pockels = masterareas[a].Currentframe().pockels();
			}
			for (uint32_t a = 0; a < planes[AreaTypeHelper::Slave].size(); a++) {
				planes[AreaTypeHelper::Slave][a].position = slaveareas[a].Currentframe().fastz();
				planes[AreaTypeHelper::Slave][a].pockels = slaveareas[a].Currentframe().pockels();
			}
			timeseriesparams.planes.push_back(planes);

			UpdatePlanesList();

			return 0;
		}

		LRESULT CTimeSeriesSettingsPage::OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
			int32_t sel = planes_list.GetSelectedIndex();
			if ( sel == -1 )
				return 0;
			// Delete the selected plane from the vector
			timeseriesparams.planes.erase(sel + std::begin(timeseriesparams.planes));
			UpdatePlanesList();
			return 0;
		}

		void CTimeSeriesSettingsPage::UpdatePlanesList() {
			uint32_t n = 0;
			CString tmp(L"");
			planes_list.DeleteAllItems();
			// Iterate over planes
			for (uint32_t at = 0; at < NAreaTypes; at++) {
				for (auto& pa : timeseriesparams.planes[at]) {
					tmp.Format(L"Plane %s%d", ((at==0)?"M":"S"), n);
					planes_list.InsertItem(n, tmp, NULL);
					tmp.Format(L"%s%d", ((at == 0) ? "M" : "S"), n);
					planes_list.SetItemText(n, 0, tmp);			// plane no
					// iterate over areas in each plane
					uint32_t a = 0;
					for (auto& p : pa) {						// for each area fastz position and pockels
						tmp.Format(L"%.1f", p.position());
						planes_list.SetItemText(n, 2 * a + 1, tmp);
						tmp.Format(L"%.2f", p.pockels());
						planes_list.SetItemText(n, 2 * a + 2, tmp);
						a++;
					}
					n++;
				}
			}
		}

		void CTimeSeriesSettingsPage::SetReadOnlyWhileScanning(const bool& _ro) {
			if ( initialized ) {
				// ReadOnlyWhileScanning is true while scanning -> enabled = false
				BOOL state = !_ro;
				addplane_button.EnableWindow(state);
				deleteplane_button.EnableWindow(state);
			}
		}


	}
}