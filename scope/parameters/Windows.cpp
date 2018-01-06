#include "stdafx.h"
#include "Windows.h"

namespace scope {
	namespace parameters {

		Window::Window(void)
			: type(L"None", L"WindowType")
			, area(0, 0, 1024, L"Area")
			, top(0, 0, 10000, L"Top")
			, left(0, 0, 10000, L"Left")
			, bottom(100, 0, 10000, L"Bottom")
			, right(100, 0, 10000, L"Right") {
		}

		void Window::Load(const wptree& pt) {
			type.SetFromPropertyTree(pt);
			area.SetFromPropertyTree(pt);
			top.SetFromPropertyTree(pt);
			left.SetFromPropertyTree(pt);
			bottom.SetFromPropertyTree(pt);
			right.SetFromPropertyTree(pt);
		}

		void Window::Save(wptree& pt) const {
			type.AddToPropertyTree(pt);
			area.AddToPropertyTree(pt);
			top.AddToPropertyTree(pt);
			left.AddToPropertyTree(pt);
			bottom.AddToPropertyTree(pt);
			right.AddToPropertyTree(pt);
		}

		WindowCollection::WindowCollection(void)
			: collection(0) {
		}

		void WindowCollection::AddWindow(const std::wstring& _type, const uint32_t& _area, HWND _hwnd) {
			Window wnd;
			wnd.type = _type;
			wnd.area = _area;
			RECT rect;
			GetWindowRect(_hwnd, &rect);
			wnd.top = rect.top;
			wnd.left = rect.left;
			wnd.bottom = rect.bottom;
			wnd.right = rect.right;
			collection.push_back(wnd);
		}

		void WindowCollection::Load(const wptree& pt) {
			try {
				// Load until get_child throws
				for ( uint32_t w = 0 ; w < 100 ; w++ ) {
					Window wnd;
					wnd.Load(pt.get_child(boost::str(boost::wformat(L"Window%d") % w)));
					collection.push_back(wnd);
				}
			}
			catch (...) { }
		}

		void WindowCollection::Save(wptree& pt) const {
			for ( uint32_t w = 0 ; w < collection.size() ; w++ ) {
				wptree wt;
				collection[w].Save(wt);
				pt.add_child(boost::str(boost::wformat(L"Window%d") % w), wt);
			}
		}

	}
}