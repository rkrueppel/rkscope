#include "stdafx.h"
#include "ChannelFrame.h"
#include "ChannelView.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "direct2d/D2ChannelRender.h"
#include "helpers/Lut.h"
#include "controllers/ScopeController.h"
#include "controls/ScopeColorComboCtrl.h"
#include "resource.h"

namespace scope {
	namespace gui {

		std::array<uint32_t, 4> CChannelFrame::colorcombo_resources = { IDC_COLORCOMBO1, IDC_COLORCOMBO2, IDC_COLORCOMBO3, IDC_COLORCOMBO4 };

		CChannelFrame::CChannelFrame(const uint32_t& _area, parameters::Area& _areaparams, const uint32_t& _channels, DisplayController& _display_controller)
			: area(_area)
			// ScopeController::GuiParameters.areas[_area]->
			, areaparams(_areaparams)
			// ScopeController::GuiParameters.areas[_area]->daq.inputs->channels()
			, channels(_channels)
			, display_controller(_display_controller)
			, attached(false)
			, current_frame(std::make_shared<config::MultiImageType>(_area, _channels, _areaparams.Currentframe().yres(), areaparams.Currentframe().xres()))
			, framecountstr(L"Frame ")
			, mousepos(D2D1::Point2F(0.0f, 0.0f))
			, mouseposstr(L"(0, 0)")
			, statusstr(L"Stopped")
			, channel_colors(channels, None)
			, overlay(_areaparams.Currentframe().yres(), _areaparams.Currentframe().xres()) {
		}

		CChannelFrame::~CChannelFrame() {
			// If we land here through an exception, OnDestroy is eventually not called thus we need to detach here to avoid
			// an invalid pointer in the DisplayController
			if ( attached ) {
				try {
					display_controller.DetachFrame(this);
				} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
				attached = false;
			}
			// We have to stop the Active's worker thread before member destruction starts, since stuff executed in that thread could want to access CChannelFrame member
			// and these get destroyed before the Active and its thread gets destroyed!
			Quit();
		}

		void CChannelFrame::OnFinalMessage(HWND /*hWnd*/) {
			delete this;
		}

		void CChannelFrame::OnDestroy() {
			// try-catch, since DetachFrame could throw if DisplayControllerImpl::DetachFrame(gui::CChannelFrame* const) does
			// not find this CChannelFrame in its list
			try {
				display_controller.DetachFrame(this);
			} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
			attached = false;
		}

		BOOL CChannelFrame::OnIdle() {
			UIUpdateToolBar();
			//Send(std::bind(&CChannelFrame::RunUpdateStatusbar, this, std::placeholders::_1));
			return FALSE;
		}

		bool CChannelFrame::RunLayOverAndRender(StopCondition* const sc, config::MultiImageCPtrType const _multi) {
			// Resize the renderer or the overlay if necessary
			if ( (_multi->Linewidth() != view.GetRendererSize().width) || (_multi->Lines() != view.GetRendererSize().height) ) {
				view.ResizeContent(_multi->Linewidth(), _multi->Lines());
				mousepos.x = 0;							// to be safe not to request a pixel value outside of the image
				mousepos.y = 0;
				// Also update the scale text
				UpdateScaleText();
			}
			if ( (_multi->Linewidth() != overlay.Linewidth()) || (_multi->Lines() != overlay.Lines()) )
				overlay.Resize(_multi->Lines(), _multi->Linewidth());

			overlay.Create(_multi, channel_colors);
			overlay.ToD2Bitmap(view.GetBitmap());
			view.Render();

			return true;
		}

		bool CChannelFrame::RunUpdateStatusbar(StopCondition* const sc, config::MultiImageCPtrType _multi) {
			UISetText(0, statusstr.c_str());
	
			// Do not update mousepos at 0,0 (it is set to this values if the cursor leaves the window)
			if ( (mousepos.x != 0.0) && (mousepos.y != 0.0) ) {
				const uint32_t xui = static_cast<uint32_t>(std::floor(mousepos.x));
				const uint32_t yui = static_cast<uint32_t>(std::floor(mousepos.y));
				const std::vector<uint16_t> multipix(current_frame->GetMultiPixel(xui, yui));
				std::wstringstream stream;
				stream << L"(" << xui << L", " << yui << L") = (";
				for ( const auto& pix : multipix )
					stream << pix << L", ";
				mouseposstr = stream.str();
				mouseposstr.erase(std::end(mouseposstr)-2, std::end(mouseposstr)-1);		// Delete trailing ", "
				mouseposstr.append(L")");													// and replace with ")"
				UISetText(1, mouseposstr.c_str());
			}

			std::wostringstream stream;
			stream << L"Frame " << current_frame->GetImageNumber() << L" (" << std::setprecision(1) << std::fixed << current_frame->PercentComplete();
			stream << L" %, avg " << current_frame->GetAvgCount() << L"/" << current_frame->GetAvgMax() << L")";
			framecountstr = stream.str();
			UISetText(2, framecountstr.c_str());

			UIUpdateStatusBar();
			return true;
		}

		void CChannelFrame::UpdateScaleText() {
			std::wostringstream stream;
			stream << std::setprecision(1) << std::fixed;
			stream << L"FOV " << std::setw(2) << areaparams.micronperpixelx() * areaparams.Currentframe().xres();
			stream << L"µm x " << std::setw(2) << areaparams.micronperpixely()* areaparams.Currentframe().yres();
			stream << L"µm (Offset " << std::setw(2) << areaparams.XOffsetInMicron();
			stream << L"µm, " << std::setw(2) << areaparams.YOffsetInMicron() << L"µm)";
			view.UpdateScaleText(stream.str());
		}

		void CChannelFrame::UncheckScaleButtons() {
			toolbar.CheckButton(IDC_SAMESIZE, FALSE);
			toolbar.CheckButton(IDC_DOUBLESIZE, FALSE);
			toolbar.CheckButton(IDC_HALFSIZE, FALSE);
		}

		int CChannelFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
			m_hWndClient = view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

			SetMenu(HMENU(NULL));

			toolbar = CreateSimpleToolBarCtrl(m_hWnd, IDR_CHANNELFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX);
			CreateSimpleReBar(ATL_SIMPLE_REBAR_STYLE);
			AddSimpleReBarBand(toolbar);
			UIAddToolBar(toolbar);

			// Add buttons texts
			AddToolbarButtonText(toolbar, IDC_SAMESIZE, L"x1");
			AddToolbarButtonText(toolbar, IDC_DOUBLESIZE, L"x2");
			AddToolbarButtonText(toolbar, IDC_HALFSIZE, L"x0.5");
	
			// Create toolbar combo box for every channel
			assert(channels <= 4);			// resource IDs for more than 4 channels not implemented, but easy to add if you need to
			for ( uint32_t ch = 0 ; ch < channels ; ch++ ) {
				CScopeColorComboCtrl combo;
				combo.m_hWnd = CreateToolbarComboBoxEx(toolbar, colorcombo_resources[ch], 5);
				combo.InitializeColors();				// Load color icons and text

				if ( ch == 0 )
					combo.SetCurSel(1);					// First channel gray
				else
					combo.SetCurSel(0);					// Other channels 'None'
			}
			// First channel is gray
			channel_colors.at(0).SetColor(Gray);

			//CreateSimpleStatusBar();
			m_hWndStatusBar = m_wndStatusBar.Create(*this);
			UIAddStatusBar (m_hWndStatusBar);
			// Create the status bar panes.
			int32_t anPanes[] = { IDPANE_STATUS , IDPANE_PIXELINFO, IDPANE_FRAMECOUNT };
 
			m_wndStatusBar.SetPanes ( anPanes, 3, false );
			UISetText(0, statusstr.c_str());
			//m_wndStatusBar.SetPaneText(0, L"Stopped");
			UISetText(1, mouseposstr.c_str());
			UISetText(2, framecountstr.c_str());
			UIUpdateStatusBar();

			UIUpdateToolBar();
			UpdateLayout();

			std::wostringstream stream;
			stream << L"Image (Area " << area+1 << L")";
			SetWindowText(stream.str().c_str());

			display_controller.AttachFrame(this);
			attached = true;

			RECT Rect;					// Size correctly, regardless what parent window said
			GetWindowRect(&Rect);
			const double AspectRatio = static_cast<double>(areaparams.Currentframe().xres()) / static_cast<double>(areaparams.Currentframe().yres());
			const double width = Rect.right-Rect.left;				// Do no subtract 20 or 92 here, since GetWindowRect is without borders!
			const double height = Rect.bottom - Rect.top;
			const int32_t targetHeight = round2i32(width / AspectRatio)+92;		// But add here, since MoveWindow wants total window size
			const int32_t targetWidth = round2i32(height * AspectRatio)+20;			//	There must be a nicer way but I do not know enough Win32
			MoveWindow(Rect.left, Rect.top, targetWidth, targetHeight);

			// Resize the bitmap in CChannelView's D2ChannelRender (because it is initially 0x0)
			view.ResizeContent(areaparams.Currentframe().xres(), areaparams.Currentframe().yres());

			// Demagnify large frames
			if ( areaparams.Currentframe().xres() >= 1024 )
				OnHalfSize(0, 0, NULL);
			else
				OnSameSize(0, 0, NULL);

			UpdateScaleText();

			return 1;
		}

		/** Override of CToolBarHelper */
		void CChannelFrame::OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD_PTR dwItemData) {
			uint32_t selchannel = 0;
			// Check which channel was clicked
			for ( auto& cc : colorcombo_resources ) {
				if (nID == cc) {
					// Set which color was selected
					channel_colors.at(selchannel).SetColor(ColorEnum(nSel));
					DBOUT(L"Channel " << selchannel << L" selected color " <<  ColorEnum(nSel) << L" String: " << gColorStrings[nSel].GetString());
				}
				selchannel++;
			}
		}

		void CChannelFrame::OnSizing(UINT fwSide, LPRECT pRect) {
			const double width = pRect->right - pRect->left - 20;
			const double height = pRect->bottom - pRect->top - 92;
			const double AspectRatio = static_cast<double>(areaparams.Currentframe().xres()) 
				/ static_cast<double>(areaparams.Currentframe().yres());
			if (fwSide == WMSZ_LEFT || fwSide == WMSZ_RIGHT || fwSide == WMSZ_RIGHT + WMSZ_BOTTOM || fwSide == WMSZ_LEFT + WMSZ_BOTTOM ) { 
			  const int targetHeight = round2ui32(width / AspectRatio) + 92;	// Here we have to add the 92 again
			  pRect->bottom = pRect->top + targetHeight; 
			} 
			else if (fwSide == WMSZ_TOP || fwSide == WMSZ_BOTTOM || fwSide == WMSZ_LEFT + WMSZ_TOP || fwSide == WMSZ_RIGHT + WMSZ_TOP ) { 
			  const int targetWidth = round2ui32(height * AspectRatio) + 20; 
			  pRect->right = pRect->left + targetWidth; 
			}
		}

		LRESULT CChannelFrame::OnUpdateMousePixel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
			D2D1_POINT_2F dips(d2d::DPIScale::PixelsToDips(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			const D2D1_SIZE_F size(view.GetRendererSize());
			dips.x = dips.x / size.width * current_frame->Linewidth();
			dips.y = dips.y / size.height * current_frame->Lines();
			mousepos = dips;
			Send(std::bind(&CChannelFrame::RunUpdateStatusbar, this, std::placeholders::_1, current_frame));
			bHandled = true;
			return 0;
		}

		BOOL CChannelFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
			if ( nFlags == MK_CONTROL )
				areaparams.Currentframe().pockels += (zDelta/120) * 0.1;

			if ( nFlags == NULL ) {
				const double oldzoom = areaparams.Currentframe().zoom();
				areaparams.Currentframe().zoom.Set(oldzoom + (zDelta/120)*0.1, true, false);			// Wait with update until offset set
				areaparams.Currentframe().CoerceOffset();											// Force new offset limits

				const double offx = (2* mousepos.x / current_frame->Linewidth() - 1) / areaparams.Currentframe().zoom();
				const double offy = (2* mousepos.y / current_frame->Lines() - 1) / areaparams.Currentframe().zoom();
				areaparams.Currentframe().xoffset.Set(offx, true, false);
				areaparams.Currentframe().yoffset.Set(offy, true, true);								// Now request update
			}

			return false;
		}

		void CChannelFrame::OnSameSize(UINT uCode, int nID, HWND hwncCtrl) {
			ResizeClient(areaparams.Currentframe().xres() + 20
				, areaparams.Currentframe().yres() + 92);
			UncheckScaleButtons();
			toolbar.CheckButton(IDC_SAMESIZE);
		}

		void CChannelFrame::OnDoubleSize(UINT uCode, int nID, HWND hwncCtrl) {
			ResizeClient(areaparams.Currentframe().xres()*2 + 20
				, areaparams.Currentframe().yres()*2 + 92);
			UncheckScaleButtons();
			toolbar.CheckButton(IDC_DOUBLESIZE);
		}

		void CChannelFrame::OnHalfSize(UINT uCode, int nID, HWND hwncCtrl) {
			ResizeClient(static_cast<int>(areaparams.Currentframe().xres()*0.5) + 20
				, static_cast<int>(areaparams.Currentframe().yres()*0.5) + 92);
			UncheckScaleButtons();
			toolbar.CheckButton(IDC_HALFSIZE);
		}

		void CChannelFrame::OnMultSize(size_t xres_mult, size_t yres_mult) {
			ResizeClient(areaparams.Currentframe().xres()*xres_mult + 20,
				areaparams.Currentframe().yres()*yres_mult + 92);
		}

		void CChannelFrame::SetHistogramLimits(const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper) {
			channel_colors.at(_channel).SetLowerLimit(_lower);
			channel_colors.at(_channel).SetUpperLimit(_upper);

			// Force rerender if we are not scanning (otherwise LayOverAndRender is called from DisplayControllerImpl::Run anyway)
			//if ( scope_controller.GuiParameters.run_state() == RunStateHelper::Stopped )
				LayOverAndRender(current_frame);
		}

		void CChannelFrame::UpdateStatus(const scope::RunState& _rs) {
			statusstr = _rs;
			Send(std::bind(&CChannelFrame::RunUpdateStatusbar, this, std::placeholders::_1, current_frame));
		}

		void CChannelFrame::LayOverAndRender(config::MultiImageCPtrType const _multi) {
			current_frame = _multi;
			// More than 6 elements in queue -> just throw away what is coming now...
			if ( ptq.Size() < 7 ) {
				Send(std::bind(&CChannelFrame::RunLayOverAndRender, this, std::placeholders::_1, current_frame));
				Send(std::bind(&CChannelFrame::RunUpdateStatusbar, this, std::placeholders::_1, current_frame));
			}
			else {
				DBOUT(L"CChannelFrame::LayOverAndRender dropping frame " << current_frame->GetImageNumber());
			}
		}

		void CChannelFrame::UpdateScaleText(const std::wstring& _text) {
			view.UpdateScaleText(_text);
		}



	}

}
