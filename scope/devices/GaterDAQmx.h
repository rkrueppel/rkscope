#pragma once

// Forward declaration
class StopCondition;

namespace scope {

/** Starts a DAQmx task that waits until the digital line is either high or low (depending on _waitforhigh)
* or until condition variable is notified  */
class GaterDAQmx {
protected:
	/** Line on which to wait for digital high/low signal. E.g. PXI-6259_0/port0/line0 */
	const std::wstring gateline;

	/** The StopCondition to check if set for aborting the WaitFor */
	const StopCondition* const sc;

public:
	/** Initialize waiting task
	* @param[in] _gateline the digital line on which to wait
	* @param[in] _sc WaitFor is aborted when this StopCondition is set to true */
	GaterDAQmx(const std::wstring& _gateline, const StopCondition* const _sc);

	/** Starts a DAQmx task that waits until the digital line is either high or low (depending on _waitforhigh)
	* or until StopCondition is set (StopCondition is checked every 200ms)
	* @param[in] _waitforhigh true: wait for high on gateline, false: wait for low
	* @return true: wait succeeded, false: wait was aborted */
	bool WaitFor(const bool& _waitforhigh);
};

}