#include "StdAfx.h"
#include "HistogramFrame.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "controllers/ScopeController.h"
#include "resource.h"
#include "parameters/Inputs.h"

namespace scope {
	namespace gui {

CHistogramFrame::CHistogramFrame(const uint32_t& _area, const uint32_t& _channels, const uint16_t& _range)
	: area(_area)
	, channels(_channels)
	, attached(false)
	, loghist(false)
	, statusstr(L"")
	, limitsstr(L"")
	, view(area, channels, _range)
	, framecount(0) {
}

CHistogramFrame::~CHistogramFrame() {
	// If we land here through an exception, OnDestroy is eventually not called thus we need to detach here to avoid
	// an invalid pointer in the DisplayController
	if ( attached ) {
		try {
			scope_controller.DetachFrame(this);
		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		attached = false;
	}
	// We have to stop the Active's worker thread before member destruction starts, since stuff executed in that thread could want to access CHistogramFrame member
	// and these get destroyed before the Active and its thread gets destroyed!
	Quit();
}

void CHistogramFrame::OnFinalMessage(HWND /*hWnd*/) {
	delete this;
}

bool CHistogramFrame::RunUpdateStatusbar(StopCondition* const sc) {
	UISetText(0, statusstr.c_str());
	std::wostringstream stream;
	for ( uint32_t c = 0 ; c < channels ; c++ )
		stream << L"(" << view.LowerLimit(c) << L", " << view.UpperLimit(c) << L") ";
	limitsstr = stream.str();
	UISetText(1, limitsstr.c_str());
	UIUpdateStatusBar();
	return true;
}

int CHistogramFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	// with this client width will be 512 pixel
	RECT client_rect;
	client_rect.top = 0;
	client_rect.left = 0;
	client_rect.bottom = 356;
	client_rect.right = 516;
	m_hWndClient = view.Create(m_hWnd, &client_rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	
	SetMenu(HMENU(NULL));

	toolbar = CreateSimpleToolBarCtrl(m_hWnd, IDR_HISTOGRAMFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX);

	AddToolbarButtonText(toolbar, IDC_OPTIMIZE, L"Optimize");
	AddToolbarButtonText(toolbar, IDC_FULLRANGE, L"Full range");

	CreateSimpleReBar(ATL_SIMPLE_REBAR_STYLE);
	AddSimpleReBarBand(toolbar);
	UIAddToolBar(toolbar);

	// Standard style but without SBARS_SIZEGRIP (no resize gripper)
	m_hWndStatusBar = m_wndStatusBar.Create(*this, ATL_IDS_IDLEMESSAGE,  WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    UIAddStatusBar (m_hWndStatusBar);
	// Create the status bar panes.
	int32_t anPanes[] = { IDPANE_STATUS , IDPANE_HISTOGRAMLIMITS };
 
    m_wndStatusBar.SetPanes ( anPanes, 2, false );
	UISetText(0, L"Stopped");
	UISetText(1, L"Limits ");
	UIUpdateStatusBar();

	UIEnable(IDC_CH1BUTTON, (channels>0));
	UIEnable(IDC_CH2BUTTON, (channels>1));
	UIEnable(IDC_CH3BUTTON, (channels>2));
	UIEnable(IDC_CH4BUTTON, (channels>3));

	UIUpdateToolBar();
	UpdateLayout();

	std::wostringstream stream;
	stream << L"Histogram (Area " << area+1 << L")";
	SetWindowText(stream.str().c_str());

	scope_controller.AttachFrame(this);
	attached = true;

	return 1;
}

void CHistogramFrame::OnDestroy() {
	try {
		scope_controller.DetachFrame(this);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	attached = false;
}

BOOL CHistogramFrame::OnIdle() {
	UIUpdateToolBar();
	//UIUpdateStatusBar();
	return FALSE;
}

void CHistogramFrame::OnOptimize(UINT uNotifyCode, int nID, CWindow wndCtl) {
	view.Optimize();
	Send(std::bind(&CHistogramFrame::RunUpdateStatusbar, this, std::placeholders::_1));
}

void CHistogramFrame::OnFullRange(UINT uNotifyCode, int nID, CWindow wndCtl) {
	view.FullRange();
	Send(std::bind(&CHistogramFrame::RunUpdateStatusbar, this, std::placeholders::_1));
}

void CHistogramFrame::OnLogHistogram(UINT uNotifyCode, int nID, CWindow wndCtl) {
	loghist = !loghist;
	toolbar.CheckButton(IDC_LOGHISTOGRAM, loghist);
}

LRESULT CHistogramFrame::OnUpdateHistogramLimits(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	Send(std::bind(&CHistogramFrame::RunUpdateStatusbar, this, std::placeholders::_1));
	return 0;
}

void CHistogramFrame::UpdateStatus(const RunState& _rs) {
	statusstr = _rs;
	Send(std::bind(&CHistogramFrame::RunUpdateStatusbar, this, std::placeholders::_1));
}

void CHistogramFrame::HistoGramAndRender(scope::SCOPE_MULTIIMAGECPTR_T const _multi) {
	framecount = _multi->GetImageNumber();
	// More than 6 elements in queue -> just throw away what is coming now...
	if ( ptq.Size() < 7 ) {
		Send([=](StopCondition* const sc) {
			// This calls the actual histogram calculation
			view.SetCurrentFrame(_multi, loghist);

			// This causes a WM_PAINT message, and CHistogramView's OnPaint calls the D2 renderer
			view.Invalidate(false);

			return true;
		});
		Send(std::bind(&CHistogramFrame::RunUpdateStatusbar, this, std::placeholders::_1));
	}
	else {
		DBOUT(L"CHistogramFrame::HistoGramAndRender dropping frames");
	}
}

}}