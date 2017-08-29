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
		, theFPUs(_nareas)
		, theStage()
		, theController()
	{
		//Make sure that TheScope is instanciated only once
		std::assert(!instanciated);
		instanciated = true;

		// Loads initial parameters
		LoadParameters(_initialparameterspath);
		
		// Now initialize the hardware with the loaded parameters
		// Give guiparameters by reference, so hardware has parameters and can connect to ScopeNumbers
		theStage.Initialize(guiparameters.stage);
		theFPUs.Initialize(guiparameters);
		
		theFPUS.ConnectButtons(theController.fpubuttonsvec);
	}
	
	void TheScope::CreateAndShowMainWindow() {
		wndmain = std::make_unique<scope::gui::CMainDlgFrame>(new gui::CMainDlgFrame(theController, theDisplay, guiparameters));

		RECT rec = { 20,20,440,980 };						// 262x403
		if (wndmain->CreateEx(HWND(0), rec) == NULL)
			throw (std::exception("Main window creation failed"));

		wndmain->ShowWindow(SW_SHOWDEFAULT);

		// Set main window title to current Scope git commit hash
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Git commit " + revstr + L")";
		wndmain->SetWindowText(revstr.c_str());
		
		// Connect the quit button
		theController.runbuttons.quit.Connect(std::bind(&CMainDlgFrame::QuitApplication, wndmain));
		
	}

	void TheScope::Version() const {
		// Extract last commit date
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Last Git commit " + revstr + L")";
		DBOUT(revstr.c_str());
	}

}