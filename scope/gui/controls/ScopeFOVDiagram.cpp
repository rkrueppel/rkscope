#include "stdafx.h"
#include "ScopeFOVDiagram.h"
#include "parameters\Scope.h"
#include "helpers\helpers.h"

namespace scope {
	namespace gui {

		CScopeFOVDiagram::CScopeFOVDiagram(const uint32_t& _area, std::vector<parameters::Area>& _areaparamsvec, const double& _masterfovsizex, const double& _masterfovsizey)
			: area(_area)
			, areaparamsvec(_areaparamsvec)
			, totalfovx(_masterfovsizex)
			, totalfovy(_masterfovsizey) {
			connections.push_back(areaparamsvec[area].fpuxystage.xpos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(areaparamsvec[area].fpuxystage.ypos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(areaparamsvec[area].micronperpixelx.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(areaparamsvec[area].micronperpixely.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(areaparamsvec[area].Currentframe().xoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
			connections.push_back(areaparamsvec[area].Currentframe().yoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
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

			for ( uint32_t a = 0 ; a < areaparamsvec.size() ; a++ ) {
				// current fov
				const double zoomedfovx = areaparamsvec[a].micronperpixelx() * areaparamsvec[a].Currentframe().xres();
				const double zoomedfovy = areaparamsvec[a].micronperpixely() * areaparamsvec[a].Currentframe().yres();
			
				// center of zoomed fov in totalfov, add FPU movement offset and scanner offset
				const double zoomedx = areaparamsvec[a].fpuxystage.xpos() + areaparamsvec[a].XOffsetInMicron();
				const double zoomedy = areaparamsvec[a].fpuxystage.ypos() + areaparamsvec[a].YOffsetInMicron();
				const double scalerx = (lpdis->rcItem.right-lpdis->rcItem.left)/totalfovx;
				const double scalery = (lpdis->rcItem.bottom-lpdis->rcItem.top)/totalfovy;
			
				// Convert to screen coordinates
				CPoint topleft(round2i16( (totalfovx/2 + zoomedx - zoomedfovx/2) * scalerx), round2i16( (totalfovy/2 + zoomedy - zoomedfovy/2) * scalery));
				CPoint bottomright(round2i16( (totalfovx/2 + zoomedx + zoomedfovx/2) * scalerx), round2i16( (totalfovy/2 + zoomedy + zoomedfovy/2) * scalery));
				CRect rect(topleft, bottomright);
			
				// Paint this area's FOV white, the other areas' gray
				if ( area == a )
					dc.FillSolidRect(&rect, RGB(255,255,255));
				else
					dc.FillSolidRect(&rect, RGB(128,128,128));
			}

			dc.RestoreDC(-1);
		}

	}
}