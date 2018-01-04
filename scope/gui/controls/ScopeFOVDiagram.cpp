#include "stdafx.h"
#include "ScopeFOVDiagram.h"
#include "parameters\Scope.h"
#include "helpers\helpers.h"

namespace scope {
	namespace gui {

		CScopeFOVDiagram::CScopeFOVDiagram(const uint32_t& _area, const AreaTypeHelper::Mode& _areatype, std::vector<parameters::MasterArea>& _masterareas,  std::vector<parameters::SlaveArea>& _slaveareas, const double& _masterfovsizex, const double& _masterfovsizey)
			: area(_area)
			, areatype(_areatype)
			, masterareas(_masterareas)
			, slaveareas(_slaveareas)
			, totalfovx(_masterfovsizex)
			, totalfovy(_masterfovsizey) {
			auto& thisarea = (_areatype == AreaTypeHelper::Master) ? (parameters::BaseArea&)masterareas[_area] : (parameters::BaseArea&)slaveareas[_area];
			connections.push_back(thisarea.fpuxystage.xpos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea.fpuxystage.ypos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea.micronperpixelx.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea.micronperpixely.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea.Currentframe().xoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(thisarea.Currentframe().yoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
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

			for (uint32_t at = 0; at < 2; at++) {
				for (uint32_t a = 0; a < ((at==AreaTypeHelper::Master)?masterareas.size():slaveareas.size()); a++) {
					auto& thisarea = ((at == AreaTypeHelper::Master) ? (parameters::BaseArea&)masterareas[a] : (parameters::BaseArea&)slaveareas[a]);
					// current fov
					const double zoomedfovx = thisarea.micronperpixelx() * thisarea.Currentframe().xres();
					const double zoomedfovy = thisarea.micronperpixely() * thisarea.Currentframe().yres();

					// center of zoomed fov in totalfov, add FPU movement offset and scanner offset
					const double zoomedx = thisarea.fpuxystage.xpos() + thisarea.XOffsetInMicron();
					const double zoomedy = thisarea.fpuxystage.ypos() + thisarea.YOffsetInMicron();
					const double scalerx = (lpdis->rcItem.right - lpdis->rcItem.left) / totalfovx;
					const double scalery = (lpdis->rcItem.bottom - lpdis->rcItem.top) / totalfovy;

					// Convert to screen coordinates
					CPoint topleft(round2i16((totalfovx / 2 + zoomedx - zoomedfovx / 2) * scalerx), round2i16((totalfovy / 2 + zoomedy - zoomedfovy / 2) * scalery));
					CPoint bottomright(round2i16((totalfovx / 2 + zoomedx + zoomedfovx / 2) * scalerx), round2i16((totalfovy / 2 + zoomedy + zoomedfovy / 2) * scalery));
					CRect rect(topleft, bottomright);

					// Paint this area's FOV white, the other areas' gray
					if ((areatype == at) && (area == a))
						dc.FillSolidRect(&rect, RGB(255, 255, 255));
					else
						dc.FillSolidRect(&rect, RGB(128, 128, 128));
				}
			}
			dc.RestoreDC(-1);
		}

	}
}