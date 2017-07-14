#pragma once

#include "ScopeDefines.h"
#include "ScopeController.h"
#include "ScopeDatatypes.h"
#include "devices/xyz/XYControlStanda.h"
#include "devices/xyz/XYControl.h"

namespace scope {

/** The FPUController. Basically encapsulates the FPU's XYStages. ETLs are controlled as fast z axis' from DaqControllerImpl via OutputsDAQmx.
* We need FPUController to be copyable, but since the devices like XYControl are noncopyable (due to mutex protected hardware access), we have
* them in here as shared_ptrs.
* @ingroup ScopeControl */
class FPUController {

protected:
	/** step size for a relative movement */
	std::array<double, SCOPE_NAREAS> stepsizes;

	/** our ScopeController */
	ScopeController scope_controller;

public:
	/** for xy movement of FPU stages */
	std::array<std::unique_ptr<SCOPE_FPUXYCONTROL_T>, SCOPE_NAREAS> theXYStages;

public:
	/** Create XYControls and ETLs and connect buttons. */
	FPUController();

	/** Initialize the FPU's hardware. */
	void Initialize(const parameters::Scope& _params);

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