#pragma once

#include "controllers/ScopeController.h"

// Forward declarations
namespace scope {
template <class T> class ScopeNumber;
}

namespace scope {

/** Base class to control an xyz-stage
* @ingroup ScopeComponentsHardware */
class XYZControl {

protected:
	/** true if initialized called successfully */
	bool initialized;

	/** to signal the async polling thread to stop */
	StopCondition stop;

	/** future for the async polling thread */
	std::future<bool> fut;

	/** pointers to a ScopeNumber that is updated with the polled x, y, and z positions */
	std::array<ScopeNumber<double>*, 3> xyzpos;

	/** interval (in milliseconds) to poll device */
	uint32_t pollinterval;

protected:
	/** disable copy */
	XYZControl(XYZControl&);

	/** disable assignment */
	XYZControl operator=(XYZControl);

public:
	XYZControl();

	/** If pollthread was started, request stop and wait for finish */
	~XYZControl();

	/** Initialize hardware. Get the ScopeNumbers by reference so we can automatically update GUI by writing to xpos and ypos.
	* Call StartPolling to start the polling thread, since StartPolling is virtual we always call the
	* most derived version of it (which should be overwritten to start the derived RunPolling).
	* If pollinterval is zero we do not start the polling thread.
	* Call from your derived class Initialize after all specific initializations are done! */
	virtual void Initialize(parameters::XYZControl& _params);

	/** Start the worker function in the pollthread */
	virtual void StartPolling();

	/** Stop the worker function */
	virtual void StopPolling();

	/** Updates xpos, ypos, and zpos with the current device position. Here only a dummy to check if polling works! */
	virtual void UpdatePositionValues();

	/** @return the current x position, questions device immediately, does not wait for polling */
	virtual double CurrentXPosition() { return 0.0; }

	/** @return the current y position, questions device immediately, does not wait for polling */
	virtual double CurrentYPosition() { return 0.0; }

	/** @return the current z position, questions device immediately, does not wait for polling */
	virtual double CurrentZPosition() { return 0.0; }

	/** Sets current xyz position as zero */
	virtual void SetZero();

	/** Sets current X position as zero */
	virtual void SetZeroXAxis() {}

	/** Sets current Y position as zero */
	virtual void SetZeroYAxis() {}

	/** Sets current Z position as zero */
	virtual void SetZeroZAxis() {}

	/** Relative movement in um */
	virtual void MoveRelative(const double& _xrel, const double& _yrel, const double& _zrel) {}

	/** Absolute movement in um */
	virtual void MoveAbsolute(const double& _xabs, const double& _yabs, const double& _zabs) {}
};

}