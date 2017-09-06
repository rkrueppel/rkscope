#include "StdAfx.h"
#include "MainDlgFrame.h"
#include "ChannelFrame.h"
#include "HistogramFrame.h"
#include "AboutDlg.h"
#include "controllers/ScopeController.h"
#include "controllers/ScopeLogger.h"
#include "LogFrame.h"

namespace scope {
	namespace gui {

std::array<UINT, 4> CMainDlgFrame::viewareas_ids = { ID_NEWVIEW_AREA1, ID_NEWVIEW_AREA2, ID_NEWVIEW_AREA3, ID_NEWVIEW_AREA4};
std::array<UINT, 4> CMainDlgFrame::histogramareas_ids = { ID_NEWHISTOGRAM_AREA1, ID_NEWHISTOGRAM_AREA2, ID_NEWHISTOGRAM_AREA3, ID_NEWHISTOGRAM_AREA4};

CMainDlgFrame::CMainDlgFrame(scope::ScopeController& _scope_controller, scope::DisplayController& _display_controller, scope::parameters::Scope& _guiparameters)
	: firstpaint(true)
	, scope_controller(_scope_controller)
	, display_controller(_display_controller)
	, currentconfigfile(L"")
	, guiparameters(_guiparameters)
	, m_dlgView(_scope_controller.runbuttons) {
}


void CMainDlgFrame::NewChannelFrame(const uint32_t& _area, const RECT& _rect) {
	RECT rect(_rect);					// We need non-const here
	CChannelFrame* pChild = new CChannelFrame(_area, *guiparameters.areas[_area], guiparameters.areas[_area]->daq.inputs->channels(), display_controller);
	// set the CMainDlgFrame as parent, so the childs receives WM_DESTROY when the parent gets destroyed (correct cleanup this way!!)
	pChild->CreateEx(m_hWnd, rect);
	pChild->ShowWindow(SW_SHOWDEFAULT);
}

void CMainDlgFrame::NewHistogramFrame(const uint32_t& _area, const RECT& _rect) {
	// Do not open histogram we alread have a histogram for that area (since calculation is done inside an Active
	// in CHistogramFrame this would mean twice the same calculation etc
	if ( !display_controller.HistogramAlreadyAttached(_area) ) {
		RECT rect(_rect);					// We need non-const here
		CHistogramFrame* pChild = new CHistogramFrame(_area, guiparameters.areas[_area]->daq.inputs->channels());
		// set the CMainDlgFrame as parent, so the childs get destroyed when the parent gets WM_DESTROY (correct cleanup this way!!)
		pChild->CreateEx(m_hWnd, rect);
		pChild->ShowWindow(SW_SHOWDEFAULT);
	}
}

void CMainDlgFrame::NewLogFrame(const RECT& _rect) {
	if ( !ScopeLogger::GetInstance().HasLogFrame() ) {
		RECT rect(_rect);				// We need non-const here
		CLogFrame* logchild = new CLogFrame;
		// set the CMainDlgFrame as parent, so the childs get destroyed when the parent gets destroyed (correct cleanup this way!!)
		logchild->CreateEx(m_hWnd, rect);
		logchild->ShowWindow(SW_SHOWDEFAULT);
	}
}

void CMainDlgFrame::RecreateWindows() {
	for ( const auto& w : guiparameters.frames.collection ) {
		RECT rect;
		rect.top = w.top;
		rect.left = w.left;
		rect.bottom = w.bottom;
		rect.right = w.right;
		if ( w.type() == L"CChannelFrame" )
			NewChannelFrame(w.area, rect);
		if ( w.type() == L"CHistogramFrame" )
			NewHistogramFrame(w.area, rect);
		if ( w.type() == L"CLogFrame" )
			NewLogFrame(rect);
	}
}

BOOL CMainDlgFrame::PreTranslateMessage(MSG* pMsg) {
	if (CMainDlgFrameType::PreTranslateMessage(pMsg))
		return TRUE;
	return FALSE;
}

BOOL CMainDlgFrame::OnIdle() {
	// this is called way too often, memory check therefore implemented differently, with OnTimer

	// Update scope run state status
	std::wstring str2(guiparameters.run_state());
	UISetText(0, str2.c_str());

	// Update shutter status in menu
	UISetCheck(ID_TOOLS_SHUTTEROPEN, scope_controller.GetShutterState(0));

	UIUpdateStatusBar();
	UIUpdateToolBar();
	return FALSE;
}

void CMainDlgFrame::QuitApplication() {
	scope_controller.PrepareQuit();
	PostMessage(WM_CLOSE);
}

void CMainDlgFrame::PrepareToolBarMenu(UINT nMenuID, HMENU hMenu) {
	static_assert(SCOPE_NAREAS <= 4, "No resource IDs defined for more than 4 areas, but easy to do if you need it");

	if (nMenuID == IDR_NEWCHANNEL_MENU) {
		CMenuHandle menu(hMenu);
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			std::wostringstream stream;
			stream << L"Area " << a+1;
			menu.InsertMenu(0, MF_STRING, viewareas_ids[a], stream.str().c_str());
		}
	}
	if (nMenuID == IDR_NEWHISTOGRAM_MENU) {
		CMenuHandle menu(hMenu);
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			std::wostringstream stream;
			stream << L"Area " << a+1;
			// Disable the menu entry if we alread have a histogram for that area (since calculation is done inside an Active
			// in CHistogramFrame this would mean twice the same calculation etc
			if ( display_controller.HistogramAlreadyAttached(a) )
				menu.InsertMenu(0, MF_STRING | MF_GRAYED, histogramareas_ids[a], stream.str().c_str());
			else
				menu.InsertMenu(0, MF_STRING, histogramareas_ids[a], stream.str().c_str());
		}
	}
}

LRESULT CMainDlgFrame::OnCreate(LPCREATESTRUCT lParam) {
	// Disable close button in title bar
	HMENU m = GetSystemMenu(FALSE);
	EnableMenuItem(m, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(HMENU(NULL));

	// Create toolbar
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX);
	
	// Add dropdown menus
	AddToolBarDropDownMenu(hWndToolBar, IDC_NEWCHANNELDROPDOWN, IDR_NEWCHANNEL_MENU);
	AddToolbarButtonText(hWndToolBar, IDC_NEWCHANNELDROPDOWN, L"New View");
	AddToolBarDropDownMenu(hWndToolBar, IDC_NEWHISTOGRAMDROPDOWN, IDR_NEWHISTOGRAM_MENU);
	AddToolbarButtonText(hWndToolBar, IDC_NEWHISTOGRAMDROPDOWN, L"New Histogram");

	// Create the rebar
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// Standard style but without SBARS_SIZEGRIP (no resize gripper)
	m_hWndStatusBar = m_wndStatusBar.Create(*this, ATL_IDS_IDLEMESSAGE,  WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	UIAddStatusBar (m_hWndStatusBar);
	// Create the status bar panes.
	int32_t anPanes[] = { IDPANE_STATUS, IDPANE_MEMORY, IDPANE_CONFIG };
	// Set status bar startup text
	m_wndStatusBar.SetPanes ( anPanes, 3, false );
	UISetText(0, L"Stopped");
	m_wndStatusBar.SetPaneText(0, L"Stopped");
	UISetText(1, L"Memory");
	UISetText(2, currentconfigfile.c_str());
	UIUpdateStatusBar();
	
	// Progress bar inside status bar (in case this is needed sometime...)
	/*CRect rBar;
	m_wndStatusBar.GetRect(0,&rBar);
	CProgressBarCtrl wndProgressBar;
	wndProgressBar.Create(m_wndStatusBar, rBar, NULL, WS_CHILD|WS_VISIBLE);
	wndProgressBar.SetRange(0,100);
	wndProgressBar.SetPos(50);
	wndProgressBar.SetWindowTextW(L"Test");
	UIUpdateStatusBar();
	*/

	// Update layout to move everything in position
	UpdateLayout();

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	// Fiddle with the window positioning to get a dialog view together with menu and toolbar
	RECT toolbarrect;
	::GetWindowRect(hWndToolBar, &toolbarrect);
	ScreenToClient(&toolbarrect);
	RECT commandbarrect;
	::GetWindowRect(hWndCmdBar, &commandbarrect);
	ScreenToClient(&commandbarrect);

	// Move the dialog view below the toolbar
	RECT rec = {10,10,280,350};
	HWND wnd = m_dlgView.Create(m_hWnd);
	::SetWindowPos(wnd,HWND_BOTTOM,0,53,430,939,NULL);

	// Set timer to 1000ms to regularly check and display memory consumption (see OnTimer)
	SetTimer(1, 1000);

	return 0;
}

void CMainDlgFrame::OnPaint(CDCHandle dc) {
	// We recreate windows from here (after CMainDlgFrame is visible), if we would do it in OnCreate they would be the top windows!
	if ( firstpaint ) {
		// Recreate windows from stored positions in parameters xml file
		RecreateWindows();
		firstpaint = false;
	}
	SetMsgHandled(false);
}

BOOL CMainDlgFrame::OnForwardMsg(LPMSG Msg, DWORD nUserData) {
	return m_dlgView.PreTranslateMessage(Msg);
}

void CMainDlgFrame::OnTimer(UINT_PTR nIDEvent) {
	if ( nIDEvent == 1 ) {
		// Update memory consumption status
		PROCESS_MEMORY_COUNTERS pmc;
		HANDLE proc = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ::GetCurrentProcessId());
		::GetProcessMemoryInfo(proc, &pmc, sizeof(pmc));
		CloseHandle(proc);
		CString str(L"");
		str.Format(L"%d Mb", pmc.WorkingSetSize/1024/1024);
		UISetText(1, str);
		UIUpdateStatusBar();
	}
}

void CMainDlgFrame::OnDestroy() {
	m_CmdBar.AttachMenu(NULL);
	m_dlgView.DestroyWindow();

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	DBOUT(L"CMainDlgFrame::OnDestroy()\n");
	SetMsgHandled(false);								// for default destroy stuff
}

LRESULT CMainDlgFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	QuitApplication();
	return 0;
}

LRESULT CMainDlgFrame::OnNewChannelFrame(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	uint32_t area = 0;		// Default if we clicked on the big IDC_NEWCHANNELDROPDOWN button (not the dropdown arrow)

	// Check on which dropdown item was clicked
	auto ait = std::find(std::begin(viewareas_ids), std::end(viewareas_ids), wID);
	if ( ait != std::end(viewareas_ids) )
		area = std::distance(std::begin(viewareas_ids), ait);

	const RECT rect = { 330, 5, 586, 261 };
	NewChannelFrame(area, rect);

	return 0;
}

LRESULT CMainDlgFrame::OnNewHistogramFrame(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	uint32_t area = 0;		// Default if we clicked on the big IDC_NEWHISTOGRAMDROPDOWN button (not the dropdown arrow)

	auto ait = std::find(std::begin(histogramareas_ids), std::end(histogramareas_ids), wID);
	if ( ait != std::end(histogramareas_ids) )
		area = std::distance(std::begin(histogramareas_ids), ait);	// If dropdown menu item/area select accordingly

	// with this CHistogramView/D2HistogramRender will be 512 pixel wide
	RECT rect = { 330, 5, 330+532, 5+350 };
	NewHistogramFrame(area, rect);

	return 0;
}

LRESULT CMainDlgFrame::OnOpenLogbook(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	RECT rect = { 430, 5, 786, 261 };
	NewLogFrame(rect);
	return 0;
}

LRESULT CMainDlgFrame::OnZeroGalvoOutputs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	scope_controller.ZeroGalvoOutputs();
	return 0;
}

LRESULT CMainDlgFrame::OnShutterOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	static bool bOpen = false;
	bOpen = !bOpen;
	UISetCheck(ID_TOOLS_SHUTTEROPEN, bOpen);
	scope_controller.OpenCloseShutter(0, bOpen);
	return 0;
}

LRESULT CMainDlgFrame::OnSaveWindowPositions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// This gets parameters::Windows of CChannelFrames and CHistogramFrames that are attached to the display controller
	guiparameters.frames = display_controller.GetWindowCollection();
	guiparameters.frames.AddWindow(L"CLogFrame", 0, ScopeLogger::GetInstance().GetLogFrameWindow());
	
	return 0;
}

LRESULT CMainDlgFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CAboutDlg dlg;
	dlg.DoModal(::GetActiveWindow());
	return 0;
}

LRESULT CMainDlgFrame::OnLoadParameters(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Open dialog to choose XML file
	COMDLG_FILTERSPEC fileTypes[] = {{ L"XML parameter file", L"*.xml" }};
	CShellFileOpenDialog dlg(NULL, FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST, L"xml", fileTypes, 1);
	dlg.GetPtr()->SetTitle(L"Select parameter file");
	IShellItem *psiFolder;
	SHCreateItemFromParsingName(L"C:\\Temp\\", NULL, IID_PPV_ARGS(&psiFolder));
	dlg.GetPtr()->SetFolder(psiFolder);

	// Crashes without GetDesktopWindow (see here: http://comments.gmane.org/gmane.comp.windows.wtl/16780), CMainDlgFrame is probably not a top level window?!
	if ( IDOK == dlg.DoModal(::GetDesktopWindow()) ) {
		CString filepath(L"");
		dlg.GetFilePath(filepath);
		DBOUT(L"Filepath " << filepath.GetString());
		guiparameters.Load(std::wstring(filepath.GetString()));
		currentconfigfile = filepath.substr(filepath.find_last_of(L'\\') + 1, std::wstring::npos);
	}

	psiFolder->Release();
	return 0;
}

LRESULT CMainDlgFrame::OnSaveParameters(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {	
	// Open dialog to for file saving
	COMDLG_FILTERSPEC fileTypes[] = {{ L"XML parameter file", L"*.xml" }};
	CShellFileSaveDialog dlg(NULL, FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT, L"xml", fileTypes, 1);
	dlg.GetPtr()->SetTitle(L"Select parameter file");
	IShellItem *psiFolder;
	SHCreateItemFromParsingName(L"C:\\Temp\\", NULL, IID_PPV_ARGS(&psiFolder));
	dlg.GetPtr()->SetFolder(psiFolder);

	// Crashes without GetDesktopWindow (see here: http://comments.gmane.org/gmane.comp.windows.wtl/16780), CMainDlgFrame is probably not a top level window?!
	if ( IDOK != dlg.DoModal(::GetDesktopWindow()) )
		return 0;

	CString filepath;
	dlg.GetFilePath(filepath);
	DBOUT(L"Filepath " << filepath.GetString());
	guiparameters.Save(std::wstring(filepath.GetString()));

	psiFolder->Release();
	return 0;
}

LRESULT CMainDlgFrame::OnSaveDefaultParameters(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Open dialog to for file saving
	COMDLG_FILTERSPEC fileTypes[] = {{ L"XML parameter file", L"*.xml" }};
	CShellFileSaveDialog dlg(NULL, FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT | FOS_FILEMUSTEXIST, L"xml", fileTypes, 1);
	dlg.GetPtr()->SetTitle(L"Select parameter file");
	IShellItem *psiFolder;
	SHCreateItemFromParsingName(L"C:\\Temp\\", NULL, IID_PPV_ARGS(&psiFolder));
	dlg.GetPtr()->SetFolder(psiFolder);

	// Crashes without GetDesktopWindow (see here: http://comments.gmane.org/gmane.comp.windows.wtl/16780), CMainDlgFrame is probably not a top level window?!
	if ( IDOK == dlg.DoModal(::GetDesktopWindow()) ) {
		CString filepath;
		dlg.GetFilePath(filepath);
		DBOUT(L"Filepath " << filepath.GetString());
		// These are initialized via the default constructor
		scope::parameters::Scope parameters;
		parameters.Save(std::wstring(filepath.GetString()));
	}
	return 0;
}

void CMainDlgFrame::ChangeScanMode(const uint32_t& _area, const ScannerVectorType& _mode) {
	m_dlgView.ChangeScanMode(_area, _mode);
}

}}