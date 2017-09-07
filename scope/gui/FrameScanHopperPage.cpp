#include "StdAfx.h"
#include "FrameScanHopperPage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanHopperPage::CFrameScanHopperPage(const uint32_t& _area
	, const bool& _isslave
	, ScopeNumber<double>& _pockels
	, ScopeNumber<double>& _fastz
	, ScopeNumber<double>& _pixeltime
	, const double& _minpixeltime
	, ScopeNumber<double>& _fpux
	, ScopeNumber<double>& _fpuy
	, FPUButtons& _fpubuttons
	, parameters::ScannerVectorFramePlaneHopper& _scanvecframehopperparams
	, ScopeNumber<uint32_t>& _averages
	, ScopeNumber<double>& _scannerdelay
	, ScopeNumber<double>& _framerate
	, ScopeNumber<double>& _frametime
	, ScopeNumber<double>& _linerate
)
	: CFrameScanBasePage(_area, _isslave, _pockels, _fastz, _pixeltime, _minpixeltime, _fpux, _fpuy, _fpubuttons, _scanvecframehopperparams, _averages, _scannerdelay, _framerate, _frametime, _linerate)
	//scope_controller.GuiParameters.areas[area]->isslave()
	, hoppervecparams(_scanvecframehopperparams)
{
	
	// Overwrite base class enum { IDD = ... }
	// Use different dialog resources depending if this area is a slave area (only Pockels and ETL sliders) or a master area (full control complement)
	if ( isslave )
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SLAVE_PROPPAGE);
	else
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_HOPPER_PROPPAGE);
}

BOOL CFrameScanHopperPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CFrameScanBasePage::OnInitDialog(wndFocus, lInitParam);

	// Add the plane list
	// Also slave areas have this!
	planes_list.Attach(GetDlgItem(IDC_PLANES_LIST));
	planes_list.InsertColumn(0, L"Plane", 0, 40);
	planes_list.InsertColumn(1, L"FastZ", 0, 40);
	planes_list.InsertColumn(2, L"Pockels", 0, 55);

	UpdatePlanesList();

	SetMsgHandled(true);
	return 0;
}

void CFrameScanHopperPage::UpdatePlanesList() {
	planes_list.DeleteAllItems();
	CString tmp;
	for ( uint32_t p = 0 ; p < hoppervecparams.planes.size() ; p++ ) {
		planes_list.InsertItem(p, L"Slice", 0);
		tmp.Format(L"%d", p);
		planes_list.SetItemText(p, 0, tmp);							// Slice no
		tmp.Format(L"%.1f", hoppervecparams.planes[p].position());
		planes_list.SetItemText(p, 1, tmp);							// fast z position
		tmp.Format(L"%.2f", hoppervecparams.planes[p].pockels());
		planes_list.SetItemText(p, 2, tmp);							// pockels
	}
}

LRESULT CFrameScanHopperPage::OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	parameters::PlaneProperties plane(hoppervecparams.fastz(), hoppervecparams.pockels());
	hoppervecparams.planes.push_back(plane);

	UpdatePlanesList();

	return 0;
}

LRESULT CFrameScanHopperPage::OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;

	// Delete the selected plane from the vector
	hoppervecparams.planes.erase(sel + std::begin(hoppervecparams.planes));
	UpdatePlanesList();

	return 0;
}


}

}
