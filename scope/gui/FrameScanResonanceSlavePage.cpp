#include "StdAfx.h"
#include "FrameScanResonanceSlavePage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanResonanceSlavePage::CFrameScanResonanceSlavePage(const uint32_t& _area, parameters::SlaveArea* _areaparams, FPUButtons& _fpubuttons)
	: CNoScanBasePage(_area, _areaparams, _fpubuttons)
	, svresonanceparams(_areaparams->FrameResonance())
{

		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_RESONANCE_SLAVE_PROPPAGE);
}

BOOL CFrameScanResonanceSlavePage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CNoScanBasePage::OnInitDialog(wndFocus, lInitParam);

	// Add the additional dialog elements
	planes_list.DeleteAllItems();
	planes_list.Attach(GetDlgItem(IDC_VOLSCAN_PLANES_LIST));
	planes_list.InsertColumn(0, L"Plane", 0, 50);
	planes_list.InsertColumn(1, L"FastZ", 0, 60);
	planes_list.InsertColumn(2, L"Pockels", 0, 60);

	if(!svresonanceparams.planes.empty())
		svresonanceparams.planes.pop_back();

	// On initialization, set the first plane with the default FastZ  and Pockels values
	parameters::PlaneProperties planes;
	planes.position = svresonanceparams.fastz;
	planes.pockels = svresonanceparams.pockels;
	svresonanceparams.planes.push_back(planes);
	UpdatePlanesList();

	SetMsgHandled(true);
	return 0;
}

LRESULT CFrameScanResonanceSlavePage::OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	DBOUT(L"CFrameScanResonanceSlavePage::OnAddPlane");

	parameters::PlaneProperties planes;
	planes.position = svresonanceparams.fastz;
	planes.pockels = svresonanceparams.pockels;
	svresonanceparams.planes.push_back(planes);
	//scope_controller.UpdateResonancePlanes(area);
	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonanceSlavePage::OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;

	// Delete the selected plane from the vector
	svresonanceparams.planes.erase(sel + std::begin(svresonanceparams.planes));
	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonanceSlavePage::OnEditPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;
	// Get the current plane values
	parameters::PlaneProperties planes;
	planes.position = svresonanceparams.fastz;
	planes.pockels = svresonanceparams.pockels;
	// Update the selected plane
	svresonanceparams.planes.at(sel) = planes;	
	//scope_controller.UpdateResonancePlanes(area);
	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonanceSlavePage::OnResetPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	// Delete all the planes
	svresonanceparams.planes.clear();
	// Add the default plane
	parameters::PlaneProperties planes;
	planes.position = svresonanceparams.fastz;
	planes.pockels = svresonanceparams.pockels;
	svresonanceparams.planes.push_back(planes);

	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonanceSlavePage::OnPlanesSelect(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;
	parameters::PlaneProperties planes;
	planes = svresonanceparams.planes.at(sel);
	svresonanceparams.fastz = planes.position;
	svresonanceparams.pockels = planes.pockels;
	return 0;
}

void CFrameScanResonanceSlavePage::UpdatePlanesList() {
	planes_list.DeleteAllItems();
	CString tmp;
	for ( uint32_t p = 0 ; p < svresonanceparams.planes.size() ; p++ ) {
		tmp.Format(L"Plane %d", p);
		planes_list.InsertItem(p, tmp, NULL);
		tmp.Format(L"%d", p);
		planes_list.SetItemText(p, 0, tmp);			// plane no
		tmp.Format(L"%.1f", svresonanceparams.planes[p].position());
		planes_list.SetItemText(p, 1, tmp);			// fast z position
		tmp.Format(L"%.2f", svresonanceparams.planes[p].pockels());
		planes_list.SetItemText(p, 2, tmp);			// pockels
	}
}

}

}
