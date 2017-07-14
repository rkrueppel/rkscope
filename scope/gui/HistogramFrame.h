#pragma once
#include "ScopeDefines.h"
#include "HistogramView.h"
#include "controllers/ScopeController.h"
#include "ScopeDatatypes.h"
#include "helpers/Active.h"
#include "ThirdParty/ToolbarHelper.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Traits for this window are standard CFramwWinTraits but without WS_THICKFRAME => no resize possible */
typedef CWinTraits<WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE> CHistogramFrameTraits;

/** Manages the display of histograms */
class CHistogramFrame
	: public CFrameWindowImpl<CHistogramFrame, CWindow, CHistogramFrameTraits>
	, public CUpdateUI<CHistogramFrame>
	, public CToolBarHelper<CHistogramFrame>
	, public CIdleHandler
	, public Active<bool> {

protected:
	/** Need to typedef this, otherwise problems with CHAIN_MSG_MAP macro... */
	typedef CFrameWindowImpl<CHistogramFrame, CWindow, CHistogramFrameTraits> CHistogramFrameType;

	/** our ScopeController here */
	scope::ScopeController scope_controller;

	/** which area is the histogram for */
	const uint32_t area;

	/** number of channels for this area */
	const uint32_t channels;

	/** are we attached to ScopeController? */
	bool attached;

	/** do we want a logarithmic count histogram */
	bool loghist;

	/** the toolbar */
	CToolBarCtrl toolbar;

	/** holds the current run status as string (we need a member since the lifetime of a local variable is too short for the
	* WTL UIUpdate mechanism). */
	std::wstring statusstr;

	/** holds the current limits as string (we need a member since the lifetime of a local variable is too short for the
	* WTL UIUpdate mechanism). */
	std::wstring limitsstr;

	/** the statusbar (multipane) */
	CMultiPaneStatusBarCtrl m_wndStatusBar;

	/** The view inside the client window */
	CHistogramView view;

	/** number of the currently displayed histogram */
	uint32_t framecount;

protected:
	/** Worker function for all statusbar updates which will run in the Active's thread.
	* Since UISetText does string buffer delete and new we need to protect it.
	* Additionally, UIUpdateStatusbar sends a settext message to the statusbar and does not return until the
	* message is processes. Taken together having a worker function for the Active, ensuring only one
	* call to UISetText and UIUpdateStatusbar at a time is the most robust solution. (Finally...) */
	bool RunUpdateStatusbar(StopCondition* const ac);

public:
	/** the command bar. public, CToolBarHelper needs this to compile... */
	CCommandBarCtrl m_CmdBar;

	DECLARE_FRAME_WND_CLASS(NULL, IDR_HISTOGRAM)

	/** Simple constructor */
	CHistogramFrame(const uint32_t& _area, const uint32_t& _channels);

	/** Detaches if not yet happened and quits */
	~CHistogramFrame();

	BEGIN_UPDATE_UI_MAP(CHistogramFrame)
		UPDATE_ELEMENT(IDC_OPTIMIZE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(IDC_FULLRANGE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(IDC_LOGHISTOGRAM, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(0, UPDUI_STATUSBAR)  // status
		UPDATE_ELEMENT(1, UPDUI_STATUSBAR)  // area
        UPDATE_ELEMENT(2, UPDUI_STATUSBAR)  // framecount
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CHistogramFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER_EX(IDC_OPTIMIZE, OnOptimize)
		COMMAND_ID_HANDLER_EX(IDC_FULLRANGE, OnFullRange)
		COMMAND_ID_HANDLER_EX(IDC_LOGHISTOGRAM, OnLogHistogram)
		MESSAGE_HANDLER(WM_UPDATEHISTOLIMITS, OnUpdateHistogramLimits)
		CHAIN_MSG_MAP(CToolBarHelper<CHistogramFrame>)
		CHAIN_MSG_MAP(CUpdateUI<CHistogramFrame>)
		CHAIN_MSG_MAP(CHistogramFrameType)
		CHAIN_CLIENT_COMMANDS()
	END_MSG_MAP()

	/** For toolbar combobox */
	void OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD_PTR dwItemData) {}

	/** @name Called via Win32 messages
	* @{ */
	/** Deletes this to free memory */
	virtual void OnFinalMessage(HWND /*hWnd*/);

	/** Updates toolbar and statusbar */
	virtual BOOL OnIdle();

	/** Creates view, toolbar, statusbar etc., resizes window correctly */
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	/** Detaches frame from ScopeController, because after OnDestroy the HWND is not valid anymore */
	void OnDestroy();

	/** Keeps an aspect ratio of 1 by adapting pRect */
	void OnSizing(UINT fwSide, LPRECT pRect);

	/** Calls CHistogramView::Optimize on view */
	void OnOptimize(UINT uNotifyCode, int nID, CWindow wndCtl);

	/** Calls CHistogramView::FullRange on view */
	void OnFullRange(UINT uNotifyCode, int nID, CWindow wndCtl);

	/** Switches between normal and logarithmic histogram */
	void OnLogHistogram(UINT uNotifyCode, int nID, CWindow wndCtl);
	/** @} */

	/** @name Called by CHistogramView via user defined Win32 message
	* @{ */
	/** Limit bar movements are handled in the client window (CHistogramView) but we want also an update of CHistogramFrame's statusbar. */
	LRESULT OnUpdateHistogramLimits(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/** @} */

	/** @name Called by the DisplayController
	* these run in the DisplayController's Run thread
	* @{ */
	/** Sends the worker function 'RunHistoGramAndRender' to the ActiveObject */
	void HistoGramAndRender(scope::SCOPE_MULTIIMAGECPTR_T const _multi);

	/** Updates the statusstr and send RunUpdateStatusbar to the Active object */
	void UpdateStatus(const RunState& _rs);
	/** @} */

	/** @return the area of this CHistogramFrame */
	uint32_t Area() const { return area; }
	
};

}}

