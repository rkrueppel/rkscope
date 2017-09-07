#include "StdAfx.h"
#include "BehaviorSettingsPage.h"

namespace scope {
	namespace gui {

CBehaviorSettingsPage::CBehaviorSettingsPage(RunButtons& _runbuttons
	, ScopeControllerCounters& _scopecontrollercounters
	, parameters::Behavior& _behaviorparameters
)
	: CToolTipDialog(TTS_NOPREFIX)
	, start_behavior_button(_runbuttons.startbehavior)
	, behaviorparameters(_behaviorparameters)
	, framecount_edit(_scopecontrollercounters.framecounter[0])
	, totaltime_edit(_scopecontrollercounters.totaltime)
	, trialcount_edit(_scopecontrollercounters.trialcounter) {
}

BOOL CBehaviorSettingsPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	start_behavior_button.AttachToDlgItem(GetDlgItem(IDC_STARTBEHAVIOR_BUTTON));
	framecount_edit.AttachToDlgItem(GetDlgItem(IDC_FRAMES_EDIT));
	totaltime_edit.AttachToDlgItem(GetDlgItem(IDC_TOTALTIME_EDIT));
	trialcount_edit.AttachToDlgItem(GetDlgItem(IDC_TRIALS_EDIT));

	planes_list.Attach(GetDlgItem(IDC_PLANES_LIST));
	planes_list.InsertColumn(0, L"Plane", 0, 40);
	std::wostringstream stream;
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		stream.str(L"");
		stream << L"FastZ " << a+1;
		planes_list.InsertColumn(1+2*a, stream.str().c_str(), 0, 60);
		stream.str(L"");
		stream << L"Pockels " << a+1;
		planes_list.InsertColumn(2+2*a, stream.str().c_str(), 0, 60);
	}
	UpdatePlanesList();

	SetMsgHandled(false);
	return 0;
}

LRESULT CBehaviorSettingsPage::OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	DBOUT(L"CBehaviorSettingsPage::OnAddPlane");

	// Get plane information for every area and add to timeseries plane vectors
	std::vector<parameters::PlaneProperties> planes;
	/*for ( uint32_t a = 0 ; a < nareas ; a++ ) {
		//planes[a].position = scope_controller.GuiParameters.areas[a]->Currentframe().fastz();
		//planes[a].pockels = scope_controller.GuiParameters.areas[a]->Currentframe().pockels();
	}*/
	behaviorparameters.planes.push_back(planes);

	UpdatePlanesList();

	return 0;
}

LRESULT CBehaviorSettingsPage::OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;
	// Delete the selected plane from the vector
	behaviorparameters.planes.erase(sel + std::begin(behaviorparameters.planes));
	UpdatePlanesList();
	return 0;
}

void CBehaviorSettingsPage::UpdatePlanesList() {
	uint32_t n = 0;
	std::wostringstream stream;
	planes_list.DeleteAllItems();
	for ( const auto& pa : behaviorparameters.planes ) {
		stream.str(L"");
		stream << L"Plane " << n;
		planes_list.InsertItem(n, stream.str().c_str(), NULL);
		stream.str(L"");
		stream << n;
		planes_list.SetItemText(n, 0, stream.str().c_str());			// plane no
		uint32_t a = 0;
		for ( const auto& p : pa ) {						// for each area fastz position and pockels
			stream.str(L"");
			stream << std::setprecision(1) << p.position();
			planes_list.SetItemText(n, 2*a + 1, stream.str().c_str());
			stream.str(L"");
			stream << std::setprecision(2) << p.pockels();
			planes_list.SetItemText(n, 2*a + 2, stream.str().c_str());
			a++;
		}
		n++;
	}
}

}}