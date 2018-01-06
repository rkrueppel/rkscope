#include "stdafx.h"
#include "ScopeFOVDiagram.h"
#include "parameters\Scope.h"
#include "helpers\helpers.h"

namespace scope {
	namespace gui {

		CScopeFOVDiagram::CScopeFOVDiagram(const uint32_t& _area, std::vector<std::unique_ptr<parameters::BaseArea>>& _allareas, const double& _masterfovsizex, const double& _masterfovsizey)
			: area(_area)
			, allareas(_allareas)
			, totalfovx(_masterfovsizex)
			, totalfovy(_masterfovsizey) {
			auto* thisarea = allareas[area].get();
			connections.push_back(thisarea->fpuxystage.xpos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea->fpuxystage.ypos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea->micronperpixelx.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea->micronperpixely.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea->Currentframe().xoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea->Currentframe().yoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
		}

		CScopeFOVDiagram::~CScopeFOVDiagram() {
			for ( auto& c : connections )
				c.disconnect();
		}

		bool CScopeFOVDiagram::AttachToDlgItem(HWND hWnd) {
			if ( SubclassWindow(hWnd) ) {
				return true;
			}
			return false;
		}

		void CScopeFOVDiagram::UpdateDiagram() {
			if ( ::IsWindow(m_hWnd) )
				Invalidate();
		}

		void CScopeFOVDiagram::DrawItem(LPDRAWITEMSTRUCT lpdis) {
			CDCHandle dc = lpdis->hDC;

			dc.SaveDC();
		 
			// Paint background black
			dc.FillSolidRect ( &lpdis->rcItem, RGB(0,0,0) );

			for (uint32_t a = 0; a < allareas.size(); a++) {
				// current fov
				const double zoomedfovx = allareas[a].get()->micronperpixelx() * allareas[a].get()->Currentframe().xres();
				const double zoomedfovy = allareas[a].get()->micronperpixely() * allareas[a].get()->Currentframe().yres();

				// center of zoomed fov in totalfov, add FPU movement offset and scanner offset
				const double zoomedx = allareas[a].get()->fpuxystage.xpos() + allareas[a].get()->XOffsetInMicron();
				const double zoomedy = allareas[a].get()->fpuxystage.ypos() + allareas[a].get()->YOffsetInMicron();
				const double scalerx = (lpdis->rcItem.right - lpdis->rcItem.left) / totalfovx;
				const double scalery = (lpdis->rcItem.bottom - lpdis->rcItem.top) / totalfovy;

				// Convert to screen coordinates
				CPoint topleft(round2i16((totalfovx / 2 + zoomedx - zoomedfovx / 2) * scalerx), round2i16((totalfovy / 2 + zoomedy - zoomedfovy / 2) * scalery));
				CPoint bottomright(round2i16((totalfovx / 2 + zoomedx + zoomedfovx / 2) * scalerx), round2i16((totalfovy / 2 + zoomedy + zoomedfovy / 2) * scalery));
				CRect rect(topleft, bottomright);

				// Paint this area's FOV white, the other areas' gray
				if (area == a)
					dc.FillSolidRect(&rect, RGB(255, 255, 255));
				else
					dc.FillSolidRect(&rect, RGB(128, 128, 128));
			}
			
			dc.RestoreDC(-1);
		}

	}
}