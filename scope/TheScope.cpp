#include "StdAfx.h"
#include "TheScope.h"

namespace scope {
	
	std::atomic<bool> TheScope::instanciated(false);
	
	TheScope::TheScope(const uint32_t& _nareas)
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
}