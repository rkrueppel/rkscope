#include "StdAfx.h"
#include "FrameScanResonancePage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanResonancePage::CFrameScanResonancePage(const uint32_t& _area)
	: CFrameScanBasePage(_area, scope_controller.GuiParameters.areas[_area]->FrameResonance())
	, ycutoff_edit(&scope_controller.GuiParameters.areas[_area]->FrameResonance().ycutoff, true, true)
	, yretrace_edit(&scope_controller.GuiParameters.areas[_area]->FrameResonance().yretrace, true, true)
	, waitstorage_edit(&scope_controller.GuiParameters.areas[_area]->FrameResonance().waitafterenqueuestorage, true, true)
	, waitdisplay_edit(&scope_controller.GuiParameters.areas[_area]->FrameResonance().waitafterenqueuedisplay, true, true) {

	zoom_scroll.SetSmallIncrement(1);

	// Overwrite base class enum { IDD = ... }
	// Use different dialog resources depending if this area is a slave area (only Pockels and ETL sliders) or a master area (full control complement)
	if ( scope_controller.GuiParameters.areas[area]->isslave() )
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_RESONANCE_SLAVE_PROPPAGE);
	else
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_RESONANCE_PROPPAGE);
}

BOOL CFrameScanResonancePage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CFrameScanBasePage::OnInitDialog(wndFocus, lInitParam);
	
	// slave areas do not have these controls
	if ( !scope_controller.GuiParameters.areas[area]->isslave() ) {
		// Add the additional dialog elements
		ycutoff_edit.AttachToDlgItem(GetDlgItem(IDC_YCUTOFF_EDIT));
		yretrace_edit.AttachToDlgItem(GetDlgItem(IDC_YRETRACE_EDIT));
		waitstorage_edit.AttachToDlgItem(GetDlgItem(IDC_WAITSTORAGE_EDIT));
		waitdisplay_edit.AttachToDlgItem(GetDlgItem(IDC_WAITDISPLAY_EDIT));
	}

	planes_list.Attach(GetDlgItem(IDC_VOLSCAN_PLANES_LIST));
	planes_list.InsertColumn(0, L"Plane", 0, 50);
	planes_list.InsertColumn(1, L"FastZ", 0, 60);
	planes_list.InsertColumn(2, L"Pockels", 0, 60);

	if(!scope_controller.GuiParameters.areas[area]->FrameResonance().planes.empty())
		scope_controller.GuiParameters.areas[area]->FrameResonance().planes.pop_back();

	// On initialization, set the first plane with the default FastZ  and Pockels values
	parameters::PlaneProperties planes;
	planes.position = scope_controller.GuiParameters.areas[area]->FrameResonance().fastz;
	planes.pockels = scope_controller.GuiParameters.areas[area]->FrameResonance().pockels;
	scope_controller.GuiParameters.areas[area]->FrameResonance().planes.push_back(planes);
	UpdatePlanesList();

	SetMsgHandled(true);
	return 0;
}

LRESULT CFrameScanResonancePage::OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	DBOUT(L"CFrameScanResonancePage::OnAddPlane");

	parameters::PlaneProperties planes;
	planes.position = scope_controller.GuiParameters.areas[area]->FrameResonance().fastz;
	planes.pockels = scope_controller.GuiParameters.areas[area]->FrameResonance().pockels;
	scope_controller.GuiParameters.areas[area]->FrameResonance().planes.push_back(planes);
	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonancePage::OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;

	// Delete the selected plane from the vector
	scope_controller.GuiParameters.areas[area]->FrameResonance().planes.erase(sel + std::begin(scope_controller.GuiParameters.areas[area]->FrameResonance().planes));
	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonancePage::OnEditPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;
	// Get the current plane values
	parameters::PlaneProperties planes;
	planes.position = scope_controller.GuiParameters.areas[area]->FrameResonance().fastz;
	planes.pockels = scope_controller.GuiParameters.areas[area]->FrameResonance().pockels;
	// Update the selected plane
	scope_controller.GuiParameters.areas[area]->FrameResonance().planes.at(sel) = planes;	
	scope_controller.UpdateResonancePlanes(area);
	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonancePage::OnResetPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	scope_controller.GuiParameters.areas[area]->FrameResonance().planes.clear();
	// Add the default plane
	parameters::PlaneProperties planes;
	planes.position = scope_controller.GuiParameters.areas[area]->FrameResonance().fastz;
	planes.pockels = scope_controller.GuiParameters.areas[area]->FrameResonance().pockels;
	scope_controller.GuiParameters.areas[area]->FrameResonance().planes.push_back(planes);

	UpdatePlanesList();
	return 0;
}

LRESULT CFrameScanResonancePage::OnPlanesSelect(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;
	parameters::PlaneProperties planes;
	planes = scope_controller.GuiParameters.areas[area]->FrameResonance().planes.at(sel);
	scope_controller.GuiParameters.areas[area]->FrameResonance().fastz = planes.position;
	scope_controller.GuiParameters.areas[area]->FrameResonance().pockels = planes.pockels;
	return 0;
}

void CFrameScanResonancePage::UpdatePlanesList() {
	planes_list.DeleteAllItems();
	CString tmp;
	for ( uint32_t p = 0 ; p < scope_controller.GuiParameters.areas[area]->FrameResonance().planes.size() ; p++ ) {
		tmp.Format(L"Plane %d", p);
		planes_list.InsertItem(p, tmp, NULL);
		tmp.Format(L"%d", p);
		planes_list.SetItemText(p, 0, tmp);			// plane no
		tmp.Format(L"%.1f", scope_controller.GuiParameters.areas[area]->FrameResonance().planes[p].position());
		planes_list.SetItemText(p, 1, tmp);			// fast z position
		tmp.Format(L"%.2f", scope_controller.GuiParameters.areas[area]->FrameResonance().planes[p].pockels());
		planes_list.SetItemText(p, 2, tmp);			// pockels
	}
}

}

}
