#pragma once

#include "ScopeDatatypes.h"
#include "parameters\Scope.h"

namespace scope {

/** Base class for all controllers.
* @ingroup ScopeControl */
class BaseController {

protected:
	/** number of possible active threads */
	const uint32_t nactives;

	/** futures from the asynchronous Run worker functions */
	std::vector<std::shared_future<ControllerReturnStatus>> futures;

	/** handed to the asynchronous Run worker functions */
	std::vector<StopCondition> stops;

	/** the Controller's own set of ScopeParameters */
	parameters::Scope parameters;

protected:
	/** The worker function that will run asynchronously.
	* @warning Every work function should regularly check the stop condition and should return on a boost::thread_interrupted exception.
	* @return since this is only a dummy, always returns ControllerReturnStatus(error). */
	virtual ControllerReturnStatus Run(StopCondition* const ac, const uint32_t& _n) {
		DBOUT(L"BaseController::Run - this should not happen, the derived class' Run method should have been called\n");
		while (!ac->IsSet())
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		ac->Set(false);
		return ControllerReturnStatus(ControllerReturnStatus::error);			// If this ever gets executed, something went wrong...
	}

public:
	BaseController(const uint32_t& _nactives);

	// Disable copy construction
	BaseController(const BaseController& _other) = delete;

	// Disable assignment
	BaseController operator=(const BaseController& _other) = delete;

	/** Execute the Run worker functions asynchronously, give them a reference to a StopCondition and get their ControllerReturnStatus futures.
	* @param[in] _params new set of ScopeParameters to use in the Controller (and its worker functions) */
	virtual void Start(const parameters::Scope& _params);

	/** Request one async worker function to stop by settings its StopCondition to true.
	* @param[in] _a which async to stop */
	virtual void StopOne(const uint32_t& _a);

	/** Request all async worker function to stop. */
	virtual void StopAll();

	/** Waits for the future of one async worker function.
	* @param[in] _a which async to wait for
	* @param[in] _wait_time in milliseconds to wait for the worker function to return, if -1 wait indefinitely
	* @return the future value */
	virtual ControllerReturnStatus WaitForOne(const uint32_t& _a, const int32_t& _wait_time);

	/** Wait for the futures of all async worker functions.
	* @param[in] _wait_time time in milliseconds to wait for the async worker function to return, if -1 wait indefinitely
	* @return the cumulated (or'ed) ControllerReturnStatus' from all async worker functions' futures */
	virtual ControllerReturnStatus WaitForAll(const int32_t& _wait_time);
};

}