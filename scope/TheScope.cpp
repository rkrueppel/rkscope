#include "StdAfx.h"
#include "TheScope.h"

std::atomic<bool> scope::TheScope::instanciated(false);

namespace scope {
	
	TheScope::TheScope(const uint32_t& _nareas, const std::wstring& _initialparameterpath)
		: nareas(_nareas)
		, daq_to_pipeline(_nareas)
		, pipeline_to_storage()
		, pipeline_to_display()
		, theDaq(_nareas, &daq_to_pipeline, parameters)
		, thePipeline(_nareas, &daq_to_pipeline, &pipeline_to_storage, &pipeline_to_display, parameters)
		, theStorage(_nareas, &pipeline_to_storage, parameters)
		, theDisplay(_nareas, &pipeline_to_display, parameters)
	{
		//Make sure that TheScope is instanciated only once
		std::assert(!instanciated);
		instanciated = true;

		// Loads initial parameter set into ScopeInterface's static GuiParameters
		theController.LoadParameters(_initialparameterspath);
	}
	
	void TheScope::CreateAndShowMainWindow() {
		wndmain = std::make_unique<scope::gui::CMainDlgFrame>(new gui::CMainDlgFrame());
		wndmain->SetScopeController(this);

		RECT rec = { 20,20,440,980 };						// 262x403
		if (wndmain->CreateEx(HWND(0), rec) == NULL)
			throw (std::exception("Main window creation failed"));

		wndmain->ShowWindow(SW_SHOWDEFAULT);

		// Set main window title to current Scope git commit hash
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Git commit " + revstr + L")";
		wndmain->SetWindowText(revstr.c_str());
	}

	void TheScope::Version() const {
		// Extract last commit date
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Last Git commit " + revstr + L")";
		DBOUT(revstr.c_str());
	}
}