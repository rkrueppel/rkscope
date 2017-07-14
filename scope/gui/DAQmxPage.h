#pragma once

#include "ScopeDefines.h"
#include "controls/ScopeLEDCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controllers/ScopeController.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Class for the DAQmx inputs property page */
class CDAQmxPage :
	public CPropertyPageImpl<CDAQmxPage> {

protected:

public:
	enum { IDD = IDD_DAQMX_PROPPAGE };

	/** DAQmx library version number */
	CScopeEditCtrl<std::wstring> daqmxversion_edit;

	/** Connect to the FIFOStatus ScopeNumbers */
	CDAQmxPage(parameters::InputsDAQmx* const _parameters);

	BEGIN_MSG_MAP(CDAQmxPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}
