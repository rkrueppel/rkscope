#pragma once

// Forward declaration
namespace scope {
	namespace parameters {
		class Scope;
	}
}

namespace scope {
	namespace gui {

/** Owner-drawn control that displays the total FOV and the smaller FOV of the areas. */
class CScopeFOVDiagram
	: public CWindowImpl<CScopeFOVDiagram>
	, public COwnerDraw<CScopeFOVDiagram> {

protected:
	/** the area of this FPU */
	const uint32_t area;

	/** Pointer to the scope parameters to use for calculations */
	parameters::Area& areaparams;
	
	/** total fov x size for zoom 1 */
	const double totalfovx;
	
	/** total fov y size for zoom 1 */
	const double totalfovy;

	/** Stores connections to ScopeValues that are then disconnected on destruction */
	std::vector<boost::signals2::connection> connections;

public:
	/** @param[in] _area for which area's FOV is this (this area's FOV will be drawn white, the others gray)
	* @param[in] _params pointer to the parameters to use for FOV calculations */
	CScopeFOVDiagram(parameters::Area& _areaparams, const double& _masterfovsizex, const double& _masterfovsizey);
	
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