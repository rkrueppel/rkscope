#include "StdAfx.h"
#include "TimeSeriesSettingsPage.h"

namespace scope {
	namespace gui {

CTimeSeriesSettingsPage::CTimeSeriesSettingsPage(void)
	: CToolTipDialog(TTS_NOPREFIX)
	, start_timeseries_button(&scope_controller.StartTimeseriesButton)
	, frames_edit(&scope_controller.GuiParameters.timeseries.frames[0], true, true)
	, frames_progress(&scope_controller.FrameCounter[0])
	, firsttriggered_check(&scope_controller.GuiParameters.timeseries.triggered, true, true)
	, alltriggered_check(&scope_controller.GuiParameters.timeseries.alltriggered, true, true)
	, totaltime_edit(&scope_controller.GuiParameters.timeseries.totaltimes[0], true, true)
	, repeats_edit(&scope_controller.GuiParameters.timeseries.repeats, true, true)
	, repeats_progress(&scope_controller.RepeatCounter)
	, betweenrepeats_edit(&scope_controller.GuiParameters.timeseries.betweenrepeats, true, true)
	, overalltime_edit(&scope_controller.GuiParameters.timeseries.overalltime, true, true) {
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

	planes_list.Attach(GetDlgItem(IDC_PLANES_LIST));
	planes_list.InsertColumn(0, L"Plane", 0, 40);
	CString label(L"");
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		label.Format(L"FastZ %d", a+1);
		planes_list.InsertColumn(1+2*a, label, 0, 60);
		label.Format(L"Pockels %d", a+1);
		planes_list.InsertColumn(2+2*a, label, 0, 60);
	}
	UpdatePlanesList();

	SetMsgHandled(false);
	return 0;
}

LRESULT CTimeSeriesSettingsPage::OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	DBOUT(L"CTimeSeriesSettingsPage::OnAddPlane");

	// Get plane information for every area and add to timeseries plane vectors
	std::array<parameters::PlaneProperties, SCOPE_NAREAS> planes;
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		planes[a].position = scope_controller.GuiParameters.areas[a]->Currentframe().fastz();
		planes[a].pockels = scope_controller.GuiParameters.areas[a]->Currentframe().pockels();
	}
	scope_controller.GuiParameters.timeseries.planes.push_back(planes);

	UpdatePlanesList();

	return 0;
}

LRESULT CTimeSeriesSettingsPage::OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;
	// Delete the selected plane from the vector
	scope_controller.GuiParameters.timeseries.planes.erase(sel + std::begin(scope_controller.GuiParameters.timeseries.planes));
	UpdatePlanesList();
	return 0;
}

void CTimeSeriesSettingsPage::UpdatePlanesList() {
	uint32_t n = 0;
	CString tmp(L"");
	planes_list.DeleteAllItems();
	for ( auto& pa : scope_controller.GuiParameters.timeseries.planes ) {
		tmp.Format(L"Plane %d", n);
		planes_list.InsertItem(n, tmp, NULL);
		tmp.Format(L"%d", n);
		planes_list.SetItemText(n, 0, tmp);			// plane no
		uint32_t a = 0;
		for ( auto& p : pa ) {						// for each area fastz position and pockels
			tmp.Format(L"%.1f", p.position());
			planes_list.SetItemText(n, 2*a + 1, tmp);
			tmp.Format(L"%.2f", p.pockels());
			planes_list.SetItemText(n, 2*a + 2, tmp);
			a++;
		}
		n++;
	}
}

}}