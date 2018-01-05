#pragma once

#include "parameters/Scope.h"
#include "controllers/ScopeController.h"
#include "controllers\DaqController.h"
#include "controllers/DisplayController.h"
#include "TheScopeButtons.h"
#include "TheScopeCounters.h"
#include "MainDlgView.h"
#include "ThirdParty/ToolbarHelper.h"
#include "resource.h"

namespace scope {
	namespace gui {

		/** Traits for this window are standard CFrameWinTraits but without WS_THICKFRAME => no resize possible */
		typedef CWinTraits<WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE> CMainDlgFrameTraits;

		/** The main application dialog frame */
		class CMainDlgFrame :
			public CAeroImpl<CMainDlgFrame>,
			public CFrameWindowImpl<CMainDlgFrame, CWindow, CMainDlgFrameTraits>,
			public CUpdateUI<CMainDlgFrame>,
			public CToolBarHelper<CMainDlgFrame>,
			public CIdleHandler,
			public CMessageFilter {

		protected:
			/** Need to typedef this, otherwise problems with CHAIN_MSG_MAP macro... */
			typedef CFrameWindowImpl<CMainDlgFrame, CWindow, CMainDlgFrameTraits> CMainDlgFrameType;

			/** to recreate stored windows */
			bool firstpaint;

			/** the status bar */
			CMultiPaneStatusBarCtrl m_wndStatusBar;

			/** the view with the dialog controls */
			CMainDlgView m_dlgView;

			scope::ScopeController& scope_controller;

			scope::DaqController& daq_controller;
			
			scope::DisplayController& display_controller;
			
			std::wstring currentconfigfile;
			
			/** Reference to TheScope's gui parameters */
			scope::parameters::Scope& guiparameters;

			/** ids for the new view toolbar dropdown menu items. See PrepareToolBarMenu. */
			static std::array<UINT, 4> viewareas_ids;

			/** ids for the new histogram toolbar dropdown menu items. See PrepareToolBarMenu. */
			static std::array<UINT, 4> histogramareas_ids;

		protected:
			/** Opens a new CChannelFrame */
			void NewChannelFrame(const uint32_t& _area, const AreaTypeHelper::Mode& _areatype, const RECT& _rect);
			
			/** Opens a new CHistogramFrame */
			void NewHistogramFrame(const uint32_t& _area, const AreaTypeHelper::Mode& _areatype, const RECT& _rect);

			/** Opens a CLogFrame */
			void NewLogFrame(const RECT& _rect);

			/** Opens windows as saved */
			void RecreateWindows();

		public:
			/** the command bar. public for CToolBarHelper to compile. */
			CCommandBarCtrl m_CmdBar;

			CMainDlgFrame(
				scope::ScopeController& _scope_controller
				, scope::DaqController& _daq_controller
				, scope::DisplayController& _display_controller
				, scope::parameters::Scope& _guiparameters
				, RunButtons& _runbuttons
				, std::array<FPUButtons, config::nmasters>& _masterfpubuttons
				, std::array<FPUButtons, config::nslaves>& _slavefpubuttons
				, std::array<ScanModeButtons, config::nmasters>& _scanmodebuttons
				, StackButtons& _stackbuttons
				, ZeroButtons& _zerobuttons
				, ScopeCounters<config::nmasters>& _counters);

			/** Used in CToolBarHelper to fill the dropdown menus */
			virtual void PrepareToolBarMenu(UINT nMenuID, HMENU hMenu);

			DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

			BEGIN_UPDATE_UI_MAP(CMainDlgFrame)
				UPDATE_ELEMENT(ID_TOOLS_SHUTTEROPEN, UPDUI_MENUPOPUP)
				UPDATE_ELEMENT(0, UPDUI_STATUSBAR)  // status
				UPDATE_ELEMENT(1, UPDUI_STATUSBAR)  // memory
				UPDATE_ELEMENT(2, UPDUI_STATUSBAR)  // config
			END_UPDATE_UI_MAP()

			BEGIN_MSG_MAP(CMainDlgFrame)
				MSG_WM_FORWARDMSG(OnForwardMsg)
				MSG_WM_CREATE(OnCreate)
				MSG_WM_PAINT(OnPaint)
				MSG_WM_TIMER(OnTimer)
				MSG_WM_DESTROY(OnDestroy)
				COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
				COMMAND_ID_HANDLER(IDC_NEWCHANNELDROPDOWN, OnNewChannelFrame)
				COMMAND_ID_HANDLER(ID_NEWVIEW_AREA1, OnNewChannelFrame)
				COMMAND_ID_HANDLER(ID_NEWVIEW_AREA2, OnNewChannelFrame)
				COMMAND_ID_HANDLER(ID_NEWVIEW_AREA3, OnNewChannelFrame)
				COMMAND_ID_HANDLER(ID_NEWVIEW_AREA4, OnNewChannelFrame)
				COMMAND_ID_HANDLER(IDC_NEWHISTOGRAMDROPDOWN, OnNewHistogramFrame)
				COMMAND_ID_HANDLER(ID_NEWHISTOGRAM_AREA1, OnNewHistogramFrame)
				COMMAND_ID_HANDLER(ID_NEWHISTOGRAM_AREA2, OnNewHistogramFrame)
				COMMAND_ID_HANDLER(ID_NEWHISTOGRAM_AREA3, OnNewHistogramFrame)
				COMMAND_ID_HANDLER(ID_NEWHISTOGRAM_AREA4, OnNewHistogramFrame)
				COMMAND_ID_HANDLER(ID_OPEN_LOGBOOK, OnOpenLogbook)
				COMMAND_ID_HANDLER(ID_FILE_LOADPARAMETERS, OnLoadParameters)
				COMMAND_ID_HANDLER(ID_FILE_SAVEPARAMETERS, OnSaveParameters)
				COMMAND_ID_HANDLER(ID_FILE_SAVEDEFAULTPARAMETERS, OnSaveDefaultParameters)
				COMMAND_ID_HANDLER(ID_TOOLS_ZEROGALVOOUTPUTS, OnZeroGalvoOutputs)
				COMMAND_ID_HANDLER(ID_TOOLS_SHUTTEROPEN, OnShutterOpen)
				COMMAND_ID_HANDLER(ID_TOOLS_SAVEWINDOWPOSITIONS, OnSaveWindowPositions)
				COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
				CHAIN_MSG_MAP(CToolBarHelper<CMainDlgFrame>)
				CHAIN_MSG_MAP(CUpdateUI<CMainDlgFrame>)
				CHAIN_MSG_MAP(CMainDlgFrameType)
				CHAIN_CLIENT_COMMANDS()
			END_MSG_MAP()

			/** For toolbar combobox */
			void OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD_PTR dwItemData) {}

			/** @name Called via Win32 messages
			* @{ */
			/** Hands over to CFrameWindowImpl */
			virtual BOOL PreTranslateMessage(MSG* pMsg);

			/** Update status-/toolbar */
			virtual BOOL OnIdle();

			/** Update the memory consumption display */
			void OnTimer(UINT_PTR nIDEvent);

			/** Creates the frame with all its stuff and the CMainDialogView inside */
			LRESULT OnCreate(LPCREATESTRUCT lParam);

			/** Recreates stored frames on first call */
			void OnPaint(CDCHandle dc);

			/** Forwards to the view */
			BOOL OnForwardMsg(LPMSG Msg, DWORD nUserData);

			/** Destroys view and removes message stuff */
			void OnDestroy();

			/** Calls QuitApplication */
			LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Opens a new CChannelFrame, determines which dropdown menu item/area was choosen */
			LRESULT OnNewChannelFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Opens a new CHistogramFrame, determines which dropdown menu item/area was choosen */
			LRESULT OnNewHistogramFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Opens the LogFrame window */
			LRESULT OnOpenLogbook(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Opens dialog to load ScopeParameters */
			LRESULT OnLoadParameters(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Opens dialog to save ScopeParameters */
			LRESULT OnSaveParameters(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Opens dialog to save the default ScopeParameters */
			LRESULT OnSaveDefaultParameters(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/**  Calls ScopeController::ZeroGalvoOutputs */
			LRESULT OnZeroGalvoOutputs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Calls ScopeController::OpenShutter */
			LRESULT OnShutterOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Calls ScopeController::SaveCurrentWindowPositions */
			LRESULT OnSaveWindowPositions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

			/** Opens the about dialog */
			LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
			/** @} */

			/** @name Called internally or from ScopeController
			* { */
			/** Calls ScopeController::PrepareQuit and sends close message and subsequently quits the whole application */
			void QuitApplication();
			
			void ChangeScanMode(const uint32_t& _area, const ScannerVectorType& _mode);
			
			/** Set controls that are not coupled to ScopeNumbers etc. (e.g. presets combo box) to read only while scanning */
			void SetReadOnlyWhileScanning(const bool& _ro);
			/** } */
		};

	}
}