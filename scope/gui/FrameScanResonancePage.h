#pragma once

#include "FrameScanBasePage.h"
#include "controls/ScopeSliderCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controllers/ScopeController.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Property page for frame scan with resonance scanner */
class CFrameScanResonancePage
	: public CFrameScanBasePage {

protected:
	parameters::ScannerVectorFrameResonance& svresonanceparams;

	/** cutoff fraction for y scanner */
	CScopeEditCtrl<double> ycutoff_edit;		

	/** retrace fraction for y scanner */
	CScopeEditCtrl<double> yretrace_edit;

	/** time to wait after image was enqueued to storage */
	CScopeEditCtrl<uint32_t> waitstorage_edit;

	/** time to wait after image was enqueued to display */
	CScopeEditCtrl<uint32_t> waitdisplay_edit;

	/** list box for all planes */
	CListViewCtrl planes_list;

	const bool isslave;

public:
	/** create the edit objects (e.g. ScopeEditImpl) and tie them to the global variables (ScopeValues)
	* set connect back to true -> on change of ScopeValue (e.g. by ScopeController) the dialog control will
	* be updated accordingly */
	CFrameScanResonancePage(const uint32_t& _area, parameters::ScannerVectorFrameResonance& _svresonanceparams, const bool _isslave);

	// Only needed if we add handlers here (see e.g. CFrameScanHopperPage)
	BEGIN_MSG_MAP(CFrameScanResonancePage)
		COMMAND_HANDLER(IDC_ADDVOLPLN_BUTTON, BN_CLICKED, OnAddPlane)
		COMMAND_HANDLER(IDC_DELVOLPLN_BUTTON, BN_CLICKED, OnDeletePlane)
		COMMAND_HANDLER(IDC_EDITVOLPLN_BUTTON, BN_CLICKED, OnEditPlane)
		COMMAND_HANDLER(IDC_RESVOLPLN_BUTTON, BN_CLICKED, OnResetPlane)
		NOTIFY_HANDLER(IDC_VOLSCAN_PLANES_LIST, NM_CLICK , OnPlanesSelect)
		CHAIN_MSG_MAP(CFrameScanBasePage)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

	/** Add a new plane with the current FastZ and Pockel values **/
	LRESULT OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/** Delete the currently selected plane **/
	LRESULT OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/** Edit the currently selected plane **/
	LRESULT OnEditPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/** Delete all the planes and add a plane with current FastZ and Pockel values **/
	LRESULT OnResetPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/** Updates the current FastZ and Pockel values with that of the selected plane **/
	LRESULT OnPlanesSelect(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
	/** @} */

	/** Update the planes list after any of the above changes **/
	void UpdatePlanesList();
};

}}