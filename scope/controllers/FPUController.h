#pragma once

#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "parameters/Area.h"
#include "devices/xyz/XYControlStanda.h"
#include "devices/xyz/XYControl.h"
#include "TheScopeButtons.h"

namespace scope {

/** The FPUController. Basically encapsulates the FPU's XYStages. ETLs are controlled as fast z axis' from DaqControllerImpl via OutputsDAQmx.
* @ingroup ScopeControl */
class FPUController {

private:
	const uint32_t nareas;
	
	/** step size for a relative movement */
	std::vector<double> stepsizes;
	
	/** Reference to TheScope's gui parameters */
	std::vector<parameters::Area>& guiareaparamsvec;
	
	/** for xy movement of FPU stages */
	std::vector<std::unique_ptr<SCOPE_FPUXYCONTROL_T>> theXYStages;

public:
	/** Create XYControls and ETLs and connect buttons. */
	FPUController(const uint32_t& _nareas, std::vector<parameters::Area>& _guiareaparamsvec, std::vector<FPUButtons>& _fpubuttonsvec);

	/** Initialize the FPU's hardware. */
	void Initialize();

	/** Move to an absolute position given by the scope_controller's GuiParameters
	* @param[in] _area which FPU to move */
	void MoveAbsolute(const uint32_t& _area);

	/** Move one stepsize relative to the current position
	* @param[in] _area which FPU to move
	* @param[in] _dir direction of movement */
	void MoveRelative(const uint32_t& _area, const FPUMoveDirection& _dir);

	/** Set the current xy position of the FPU as zero */
	void SetXYZero(const uint32_t& _area);
};

}