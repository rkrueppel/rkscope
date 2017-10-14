#include "stdafx.h"
#include "resource.h"
#include "TheScope.h"
#include "controllers/ScopeLogger.h"
#include "version.h"

/** @file scope/scope.cpp This is the main file for the Scope.exe */

CAppModule _Module;

uint8_t mylutfunc(const double& e) {
	return (uint8_t)(255*std::sin(e));
}

/** Run the main message loop */
int Run(HINSTANCE hInstance) {
	//LUT_function<> sinus(mylutfunc, M_PI/255);
	int nRet = -1;
	MSG msg;
	// force message queue to be created
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	// See if default.xml exists
	CString filepath = L"default.xml";
	bool founddefault = false;
	HANDLE defaultfile = CreateFile(filepath.GetString(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// If no default file found open a dialog for choosing an xml file
	INT_PTR dlgret = IDCANCEL;
	if ( defaultfile == INVALID_HANDLE_VALUE ) {
		COMDLG_FILTERSPEC fileTypes[] = {{ L"Scope XML parameter file", L"*.xml" }};
		CShellFileOpenDialog dlg(NULL, FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST, L"xml", fileTypes, 1);
		dlg.GetPtr()->SetTitle(L"Select Scope XML parameter file");
		IShellItem *psiFolder;
		SHCreateItemFromParsingName(L"C:\\Temp\\", NULL, IID_PPV_ARGS(&psiFolder));
		dlg.GetPtr()->SetFolder(psiFolder);
		dlgret = dlg.DoModal();
		if ( IDOK == dlgret ) 
			dlg.GetFilePath(filepath);
		psiFolder->Release();
	}
	// Convert file handle to path and close handle
	else {
		founddefault = true;
		std::vector<wchar_t> buff(512);
		GetFinalPathNameByHandle(defaultfile, buff.data(), 512, FILE_NAME_NORMALIZED);
		filepath.SetString(buff.data());
		CloseHandle(defaultfile);
	}

	// If we found default.xml or choose a file in the dialog, proceed
	if ( founddefault || (dlgret == IDOK) ) {
		DBOUT(L"Filepath " << filepath.GetString());

		scope::TheScope myscope(1, std::wstring(filepath.GetString()));
		myscope.Version();
		
		// Set the loggers folder to the storage folder configured in the xml file
		
		// Sometimes, in debug build an mutex access violation pops up around here, do some cosmetic variations in the code and recompile
		std::this_thread::sleep_for(std::chrono::milliseconds(20));				// Move this around on debug build mutex bug
			
		scope::ScopeLogger::GetInstance().SetFilepath(myscope.GetStorageFolder()+L"\\"+GetCurrentDateString());

		// Create the main window
		myscope.CreateAndShowMainWindow();

		// Log startup scope stuff
		std::wstring msg2(L"This is Scope (Git commit ");
		msg2 += CA2W(STR(LASTGITCOMMIT));
		msg2 += L")";
		scope::ScopeLogger::GetInstance().Log(msg2, scope::log_info);
		msg2 = L"Using configuration file " + filepath;
		scope::ScopeLogger::GetInstance().Log(msg2, scope::log_info);

		// Run the main message loop
		nRet = theLoop.Run();
		
		myscope.PrepareQuit();
	}
	// if we did not only display a message box and exit
	else {
		::MessageBox(NULL, L"No default parameter file found and no parameter file choosen!", L"Choose wisely next time", MB_OK | MB_ICONWARNING | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
		// Open dialog to for file saving
		COMDLG_FILTERSPEC fileTypes[] = { { L"XML parameter file", L"*.xml" } };
		CShellFileSaveDialog dlg(NULL, FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT | FOS_FILEMUSTEXIST, L"xml", fileTypes, 1);
		dlg.GetPtr()->SetTitle(L"Select parameter file");
		IShellItem *psiFolder;
		SHCreateItemFromParsingName(L"C:\\Temp\\", NULL, IID_PPV_ARGS(&psiFolder));
		dlg.GetPtr()->SetFolder(psiFolder);

		// Crashes without GetDesktopWindow (see here: http://comments.gmane.org/gmane.comp.windows.wtl/16780), CMainDlgFrame is probably not a top level window?!
		if (IDOK == dlg.DoModal(::GetDesktopWindow())) {
			CString filepath;
			dlg.GetFilePath(filepath);
			DBOUT(L"Filepath " << filepath.GetString());
			// These are initialized via the default constructor
			scope::parameters::Scope defparams(SCOPE_NAREAS);
			defparams.Save(std::wstring(filepath.GetString()));
		}
	}
	
	_Module.RemoveMessageLoop();
	scope::ScopeLogger::GetInstance().Shutdown();
	return nRet;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow) {
	// Uncomment these to get a memory dump on exit

	// Enable CRT debug reporting on assertions
	//_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	
	// Enable CRT dump memory leaks on exit
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// Be sure this succeeds, otherwise e.g. a library (Galil1.lib did this) could have called CoInitialize before!!
	ATLASSERT(SUCCEEDED(hRes));

	// Just to be sure we do not have problems because someone tries to run this on XP...
	assert( RunTimeHelper::IsWin7() );

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(HWND(NULL), 0, 0, 0L);

	// Initialize common controls (do not forget to link, see stdafx.h)
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES | ICC_STANDARD_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	assert(SUCCEEDED(hRes));

	// Load library for RichEdit Control
	HMODULE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());
	assert(hInstRich != NULL);

	int32_t nRet = Run(hInstance);

	// Unload RichEdit library
	::FreeLibrary(hInstRich);

	//ReportLiveObjects();

	_Module.Term();
	// Uninitialize COM
	::CoUninitialize();

	// And goodbye...
	return nRet;
}
