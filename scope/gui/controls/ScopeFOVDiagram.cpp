#include "stdafx.h"
#include "ScopeFOVDiagram.h"
#include "parameters\Scope.h"
#include "helpers\helpers.h"

namespace scope {
	namespace gui {

CScopeFOVDiagram::CScopeFOVDiagram(const uint32_t& _area, const parameters::Scope* _params)
	: area(_area)
	, params(_params) {
	connections.push_back(params->areas[area]->fpuxystage.xpos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
	connections.push_back(params->areas[area]->fpuxystage.ypos.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
	connections.push_back(params->areas[area]->micronperpixelx.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
	connections.push_back(params->areas[area]->micronperpixely.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
	connections.push_back(params->areas[area]->Currentframe().xoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
	connections.push_back(params->areas[area]->Currentframe().yoffset.ConnectGUI(std::bind(&CScopeFOVDiagram::UpdateDiagram, this)));
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

	// total fov for zoom 1
	const double totalfovx = params->masterfovsizex();
	const double totalfovy = params->masterfovsizey();

	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		// current fov
		const double zoomedfovx = params->areas[a]->micronperpixelx() * params->areas[a]->Currentframe().xres();
		const double zoomedfovy = params->areas[a]->micronperpixely() * params->areas[a]->Currentframe().yres();
	
		// center of zoomed fov in totalfov, add FPU movement offset and scanner offset
		const double zoomedx = params->areas[a]->fpuxystage.xpos() + params->areas[a]->XOffsetInMicron();
		const double zoomedy = params->areas[a]->fpuxystage.ypos() + params->areas[a]->YOffsetInMicron();
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