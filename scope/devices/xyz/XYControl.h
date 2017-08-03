#pragma once

// Forward declarations
namespace scope {
template <class T> class ScopeNumber;
}

namespace scope {

/** Base class to control an xy-stage
* @ingroup ScopeComponentsHardware */
class XYControl {

protected:
	/** true if initialized called successfully */
	bool initialized;

	/** to signal the async polling thread to stop */
	StopCondition stop;

	/** future for the async polling thread */
	std::future<bool> fut;

	/** pointers to a ScopeNumber that is updated with the polled x and y positions */
	std::array<ScopeNumber<double>*, 2> pos;

	/** interval (in milliseconds) to poll device */
	uint32_t pollinterval;

protected:
	/** disable copy */
	XYControl(XYControl&);

	/** disable assignment */
	XYControl operator=(XYControl);

public:
	XYControl();

	/** If pollthread was started, request stop and wait for finish */
	~XYControl();

	/** Initialize hardware. Get the ScopeNumbers by reference so we can automatically update GUI by writing to xpos and ypos.
	* Call StartPolling to start the polling thread, since StartPolling is virtual we always call the
	* most derived version of it (which should be overwritten to start the derived RunPolling).
	* If pollinterval is zero we do not start the polling thread. */
	virtual void Initialize(parameters::XYControl& _params);

	/** Start the worker function in the pollthread */
	virtual void StartPolling();

	/** Stop the worker function */
	virtual void StopPolling();

	/** Updates xpos and ypos with the current device position. Here only a dummy to check if polling works! */
	virtual void UpdatePositionValues();

	/** Sets current position as zero */
	virtual void SetZero() {}

	/** Sets current X position as zero */
	virtual void ZeroXAxis() {}

	/** Sets current Y position as zero */
	virtual void ZeroYAxis() {}

	/** Relative movement */
	virtual void MoveRelative(const double& xrel, const double& yrel) {}

	/** Absolute movement */
	virtual void MoveAbsolute(const double& xabs, const double& yabs) {}
};

}