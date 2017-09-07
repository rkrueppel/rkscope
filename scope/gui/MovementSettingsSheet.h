#pragma once

#include "XYZControlPage.h"
#include "FPUControlPage.h"
#include "parameters/Area.h"
#include "parameters/Devices.h"
#include "TheScopeButtons.h"

namespace scope {
	namespace gui {

/** Settings sheet that has settings pages for all movement/stages related stuff,
* resided inside the CMovementPage */
class CMovementSettingsSheet
	: public CPropertySheetImpl<CMovementSettingsSheet> {

protected:
	/** a settings page for XYZ stage */
	CXYZControlPage xyzcontrolpage;

	/** settings pages for each FPU */
	std::vector<CFPUControlPage> fpupages;

public:
	/** Creates the CFPUControlPages */
	CMovementSettingsSheet(
		std::vector<parameters::Area>& _areaparamsvec
		, std::vector<FPUButtons>& _fpubuttonsvec
		, const double& _masterfovsizex
		, const double& _masterfovsizey
		, parameters::Stage& _stageparams
		, ZeroButtons& _zerobuttons);

	BEGIN_MSG_MAP(CMovementSettingsSheet)	  
		NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnSelChange)
		CHAIN_MSG_MAP(CPropertySheetImpl<CMovementSettingsSheet>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* Override Create and use Callback to switch to child window style during creation
	* since Property sheets are popups as standard (Windows not WTL I think)
	* @{ */
	/** Adds all the settingspages */
	HWND Create(const HWND hWndParent, const int nStartPage, const CRect & rc);

	/** We need this for whatever (see some WTL dialog tutorial)... */
	static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);

	LRESULT OnSelChange(WPARAM wParam, LPNMHDR pnmHdr, BOOL & bHandled);
	/** @} */
};

}}