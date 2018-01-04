#include "StdAfx.h"
#include "ChannelView.h"
#include "config\config_choices.h"

namespace scope {
	namespace gui {

		int CChannelView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
			renderer.Create(m_hWnd, lpCreateStruct->cx, lpCreateStruct->cy);
			return 1;
		}

		BOOL CChannelView::PreTranslateMessage(MSG* pMsg) {
			pMsg;
			return false;
		}

		void CChannelView::OnPaint(CDCHandle /*dc*/) {
			PAINTSTRUCT paint;
			ATLASSERT(BeginPaint(&paint));
			renderer.Render();
			EndPaint(&paint);
			ValidateRect(NULL);
		}

		void CChannelView::OnSize(UINT /*type*/, CSize size) {
			if ( !renderer.Resize(size.cx, size.cy) )
				 ATLASSERT(Invalidate(FALSE));			// Why? If !Size than resize went wrong anyway?! (The lines are copied from example in Help)
		}

		void CChannelView::OnDisplayChange(UINT /*bpp*/, CSize /*resolution*/) {
			renderer.Render();
		}

		LRESULT CChannelView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
			::SendMessage(GetParent().m_hWnd, WM_UPDATEMOUSEPIXEL, wParam, lParam);
			bHandled = true;
			return 0;
		}

		D2D1_SIZE_F CChannelView::GetRendererSize() const {
			return renderer.GetSize();
		}

		void CChannelView::Render() {
			renderer.Render();
		}

		void CChannelView::ResizeContent(const uint32_t& _xres, const uint32_t& _yres) {
			renderer.ResizeBitmap(_xres, _yres);
		}

		ID2D1Bitmap* CChannelView::GetBitmap() {
			return renderer.Bitmap();
		}

		void CChannelView::UpdateScaleText(const std::wstring& _text) {
			renderer.UpdateScaleText(_text);
		}

	}
}

