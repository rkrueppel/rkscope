#pragma once

#include "parameters/Scope.h"
#include "ScopeDatatypes.h"
#include "helpers/ScopeException.h"

namespace scope {

/** Implementation class of the BaseController, uses std::async to execute worker functions in a separate thread. */
template<uint32_t N_ACTIVES, bool STATIC_PIMPL>
class BaseController<N_ACTIVES, STATIC_PIMPL>::Impl {

protected:
	/** futures from the asynchronous Run worker functions */
	std::array<std::shared_future<ControllerReturnStatus>, N_ACTIVES> futures;

	/** handed to the asynchronous Run worker functions */
	std::array<StopCondition, N_ACTIVES> stops;

	/** the Controller's own set of ScopeParameters */
	parameters::Scope parameters;

private:
	/** disable copy */
	Impl(const Impl& i);

	/** disable assignment */
	Impl operator=(const Impl& i);

protected:
	/** The worker function that will run asynchronously.
	* @warning Every work function should regularly check the stop condition and should return on a boost::thread_interrupted exception.
	* @return since this is only a dummy, always returns ControllerReturnStatus(error). */
	virtual ControllerReturnStatus Run(StopCondition* const ac, const uint32_t& _n) {
		DBOUT(L"BaseControllerImpl::Run - this should not happen, the derived class' Run method should have been called\n");
		while ( !ac->IsSet() )
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		ac->Set(false);
		return ControllerReturnStatus(ControllerReturnStatus::error);			// If this ever gets executed, something went wrong...
	}

public:
	/** @param[in] _parameters initial set of ScopeParameters */
	Impl(const parameters::Scope& _parameters)
		: parameters(_parameters) {
	}

	/** It is important that this is virtual, only by this the correct destructor for the pimpl is called in derived classes (since we are using
	* the one BaseControllerImpl* pointer to store also derived Impl class objects. */
	virtual ~Impl() {
		DBOUT(L"BaseController::Impl::~Impl\n");
	}

	/** Execute the Run worker functions asynchronously, give them a reference to a StopCondition and get their ControllerReturnStatus futures.
	* @param[in] _params new set of ScopeParameters to use in the Controller (and its worker functions) */
	virtual void Start(const parameters::Scope& _params) {
		DBOUT(L"BaseController::Impl::Start\n");
		parameters = _params;
		for( uint32_t a = 0; a < N_ACTIVES ; a++ ) {
			stops[a].Set(false);
			// Let "Run" run asynchronously and get its future
			futures[a] = std::async(std::bind(&Impl::Run, this, &stops[a], a));
		}
	}

	/** Request one async worker function to stop by settings its StopCondition to true.
	* @param[in] _a which async to stop */
	virtual void StopOne(const uint32_t& _a) {
		ATLASSERT(_a < N_ACTIVES);
		stops[_a].Set(true);
	}

	/** Request all async worker function to stop. */
	virtual void StopAll() {
		DBOUT(L"BaseController::Impl::Stop\n");
		for ( uint32_t a = 0 ; a < N_ACTIVES ; a++ )
			StopOne(a);
	}

	/** Waits for the future of one async worker function.
	* @param[in] _a which async to wait for
	* @param[in] _wait_time in milliseconds to wait for the worker function to return, if -1 wait indefinitely
	* @return the future value */
	virtual ControllerReturnStatus WaitForOne(const uint32_t& _a, const int32_t& _wait_time) {
		if ( futures[_a].valid() ) {
			std::future_status::future_status state(std::future_status::ready);

			// Wait (either indefinitely or _wait_time
			if ( _wait_time == -1 )
				futures[_a].wait();
			else
				state = futures[_a].wait_for(std::chrono::milliseconds(_wait_time));

			// If async returned, get the future's value here
			if ( state == std::future_status::ready )
				return futures[_a].get();
			// Landing here means, wait_for ran into a timeout
			else
				throw ScopeException("WaitForOne did not succeed, async thread did not return");
		}
		else
			DBOUT(L"WaitForOne future is not valid (async worker function is not running)");

		return ControllerReturnStatus::error;
	}

	/** Wait for the futures of all async worker functions.
	* @param[in] _wait_time time in milliseconds to wait for the async worker function to return, if -1 wait indefinitely
	* @return the cumulated (or'ed) ControllerReturnStatus' from all async worker functions' futures */
	virtual ControllerReturnStatus WaitForAll(const int32_t& _wait_time) {
		ControllerReturnStatus totalresult(ControllerReturnStatus::none);
		for ( uint32_t a = 0 ; a < N_ACTIVES ; a++ ) {
			try {
				totalresult = (ControllerReturnStatus)( totalresult | WaitForOne(a, _wait_time) );
			}
			catch (... ) { ScopeExceptionHandler(__FUNCTION__, true, true); }
		}
		DBOUT(L"BaseController::Impl::WaitForAll returns\n");
		return totalresult;
	}
};

}