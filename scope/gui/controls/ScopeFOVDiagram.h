#pragma once
#include "parameters\Scope.h"

namespace scope {
	namespace gui {

/** Owner-drawn control that displays the total FOV and the smaller FOV of the areas. */
class CScopeFOVDiagram
	: public CWindowImpl<CScopeFOVDiagram>
	, public COwnerDraw<CScopeFOVDiagram> {

protected:
	/** the area of this FPU */
	const uint32_t area;

	const AreaTypeHelper::Mode areatype;

	/** Pointer to the scope parameters to use for calculations */
	std::vector<parameters::MasterArea>& masterareas;
	std::vector<parameters::SlaveArea>& slaveareas;
	
	/** total fov x size for zoom 1 */
	const double totalfovx;
	
	/** total fov y size for zoom 1 */
	const double totalfovy;

	/** Stores connections to ScopeValues that are then disconnected on destruction */
	std::vector<boost::signals2::connection> connections;

public:
	/** @param[in] _area for which area's FOV is this (this area's FOV will be drawn white, the others gray)
	* @param[in] _params pointer to the parameters to use for FOV calculations */
	CScopeFOVDiagram(const uint32_t& _area, const AreaTypeHelper::Mode& _areatype, std::vector<parameters::MasterArea>& _masterareas, std::vector<parameters::SlaveArea>& _slaveareas, const double& _masterfovsizex, const double& _masterfovsizey);
	
	~CScopeFOVDiagram();

	BEGIN_MSG_MAP_EX(CScopeFOVDiagram)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CScopeFOVDiagram>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	/** Attach the control to a dialog item */
	bool AttachToDlgItem(HWND hWnd);

	/** Connected to scope parameters, called upon their change */
	void UpdateDiagram();
 
	/** Called from COwnerDraw */
	void DrawItem(LPDRAWITEMSTRUCT lpdis);

};


}
}