#pragma once

#include "ScopeDefines.h"
#include "controllers/ScopeController.h"
#include "ChannelView.h"
#include "helpers/ScopeOverlay.h"
#include "helpers/ScopeOverlayResonanceSW.h"
#include "helpers/Active.h"
#include "ThirdParty/ToolbarHelper.h"
#include "resource.h"

// Forward declarations
class ColorProps;
class D2ChannelRender;
namespace scope {
	class ColorProps;
	class SCOPE_MULTIIMAGE_T;
	typedef std::shared_ptr<const SCOPE_MULTIIMAGE_T> SCOPE_MULTIIMAGECPTR_T;
}

namespace scope {
	/** All classes for the user interface, all derived from WTL classes */
	namespace gui {

/** Manages the display of images */
class CChannelFrame
	: public CFrameWindowImpl<CChannelFrame>
	, public CUpdateUI<CChannelFrame>
	, public CToolBarHelper<CChannelFrame>
	, public CIdleHandler
	, public Active<bool> {

protected:
	/** the ScopeController kept handy here */
	scope::ScopeController scope_controller;
	
	/** area for this CChannelFrame */
	const uint32_t area;
	
	/** number of channels in this area */												
	const uint32_t channels;
	
	/** are we attached to ScopeController? */											
	bool attached;
	
	/** The view inside the client window */														
	CChannelView view;

	/** currently displayed image */													
	scope::SCOPE_MULTIIMAGECPTR_T current_frame;
	
	/** Holds the current frame count etc as string. We need a class member for this since UIUpdateStatus bar runs asynchronously. A local string would get deleted once it is out of scope. */												
	std::wstring framecountstr;

	/** current mouse position in device independent pixels (for the renderer) */
	D2D1_POINT_2F mousepos;
	
	/** holds the current mouse position as string */									
	std::wstring mouseposstr;
	
	/** holds the current run status as string */												
	std::wstring statusstr;
	
	/** the statusbar */													
	CMultiPaneStatusBarCtrl m_wndStatusBar;

	/** the toolbar */
	CToolBarCtrl toolbar;
	
	/** Ordered by channel number. */			
	std::vector<ColorProps> channel_colors;

	/** Overlay of channels of the currently displayed image, use shared_ptr because it is used from different thread (e.g. the Active's worker thread) */
	scope::SCOPE_OVERLAY_T overlay;

	/** The resource IDs of the channel color combo boxes */
	static std::array<uint32_t, 4> colorcombo_resources;
 
protected:
	/** Worker function which will run in the Active's thread.
	* Calculates overlay, copies it to a Direct2D bitmap and renders */
	bool RunLayOverAndRender(StopCondition* const sc, scope::SCOPE_MULTIIMAGECPTR_T const _multi);

	/** Worker function for all statusbar updates which will run in the Active's thread.
	* Since UISetText does string buffer delete and new we need to protect it.
	* Additionally, UIUpdateStatusbar sends a settext message to the statusbar and does not return until the
	* message is processes. Taken together having a worker function for the Active, ensuring only one
	* call to UISetText and UIUpdateStatusbar at a time is the most robust solution. (Finally...) */
	bool RunUpdateStatusbar(StopCondition* const sc, scope::SCOPE_MULTIIMAGECPTR_T const _multi);

	/** Updates the scale text with current values from scope_controller::GuiParameters */
	void UpdateScaleText();

	/** Unchecks all scale buttons */
	void UncheckScaleButtons();

public:
	/** the command bar. public, CToolBarHelper needs this to compile... */
	CCommandBarCtrl m_CmdBar;

	DECLARE_FRAME_WND_CLASS(NULL, IDR_CHANNELFRAME)
	
	/** Initialize everything
	* @param[in] _area which the CChannelFrame is for */
	CChannelFrame(const uint32_t& _area);

	/** Detach if not yet happened and quit */
	~CChannelFrame();

	BEGIN_UPDATE_UI_MAP(CChannelFrame)
		UPDATE_ELEMENT(IDC_SAMESIZE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(IDC_DOUBLESIZE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(IDC_HALFSIZE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(0, UPDUI_STATUSBAR)  // status
		UPDATE_ELEMENT(1, UPDUI_STATUSBAR)  // area
        UPDATE_ELEMENT(2, UPDUI_STATUSBAR)  // framecount
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CChannelFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZING(OnSizing)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MESSAGE_HANDLER(WM_UPDATEMOUSEPIXEL, OnUpdateMousePixel)
		COMMAND_ID_HANDLER_EX(IDC_SAMESIZE, OnSameSize)
		COMMAND_ID_HANDLER_EX(IDC_DOUBLESIZE, OnDoubleSize)
		COMMAND_ID_HANDLER_EX(IDC_HALFSIZE, OnHalfSize)
		CHAIN_MSG_MAP(CToolBarHelper<CChannelFrame>)
		CHAIN_MSG_MAP(CUpdateUI<CChannelFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CChannelFrame>)
		CHAIN_CLIENT_COMMANDS()
	END_MSG_MAP()

	/** For toolbar combobox */
	void OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD_PTR dwItemData);

	/** @name Called via Win32 messages
	* @{ */
	/** delete this to free the memory */
	virtual void OnFinalMessage(HWND /*hWnd*/);

	/** Updates toolbar and statusbar */
	virtual BOOL OnIdle();

	/** Create view, toolbar, and statusbar etc. Resize window correctly. */
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	/** Detaches frame from ScopeController, because after OnDestroy the HWND is not valid anymore */
	void OnDestroy();

	/** Keeps the aspect ratio.
	* We subtract 20 for the window border and 92 for the tool- and statusbar to end up with the correct
	* size ChannelFrame must have for correct aspect ratio in ChannelView */
	void OnSizing(UINT fwSide, LPRECT pRect);

	/** Handles mouse wheel events to change zoom or pockels cell value (wheel+Ctrl) */
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	/** Sets window size to match the real frame size */
	void OnSameSize(UINT uCode, int nID, HWND hwncCtrl);

	/** Sets window size to double the real frame size */
	void OnDoubleSize(UINT uCode, int nID, HWND hwncCtrl);

	/** Sets window size to half the real frame size */
	void OnHalfSize(UINT uCode, int nID, HWND hwncCtrl);

	/** @} */

	/** Sets window size to the multipliers of the real frame size */
	void OnMultSize(size_t xres_mult, size_t yres_mult);

	/** @name Called by CChannelView via user defined Win32 message
	* @{ */
	/** Mouse movements are received in the client window (CChannelView) with the correct coordinates. CChannelView passes this on to CChannelFrame which uses this function to
	* process and show intensity values of the corresponding image pixel */
	LRESULT OnUpdateMousePixel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/** @} */

	/** @name Called by the DisplayController
	* these run in the DisplayController's Run thread
	* @{ */
	/** Updates the statusstr and send RunUpdateStatusbar to the Active object */
	virtual void UpdateStatus(const RunState& _rs);

	/** Sets upper and lower limit of displayed colors for a channel */
	virtual void SetHistogramLimits(const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper);

	/** Sends the worker function 'RunLayOverAndRender' to the ActiveObject
	* @param[in] _multi Pointer to the current multi image */
	virtual void LayOverAndRender(scope::SCOPE_MULTIIMAGECPTR_T const _multi);

	/** Updates scale text. Calls CChannelView::UpdateScale */
	virtual void UpdateScaleText(const std::wstring& _text);
	/** @} */

	/** @return the CChannelFrame's area */
	uint32_t Area() const { return area; }
};

}}