#include "StdAfx.h"
#include "BaseController.h"

namespace scope {
	BaseController::BaseController(const uint32_t& _nactives)
		: nactives(_nactives)
		, futures(_nactives)
		, stops(_nactives) {
		ATLASSERT(_nactives > 0);
	}

	void BaseController::Start(const parameters::Scope& _params) {
		DBOUT(L"BaseController::Start\n");
		parameters = _params;
		for (uint32_t a = 0; a < nactives; a++) {
			stops[a].Set(false);
			// Let "Run" run asynchronously and get its future
			futures[a] = std::async(std::bind(&Run, this, &stops[a], a));
		}
	}

	void BaseController::StopOne(const uint32_t& _a) {
		ATLASSERT(_a < nactives);
		stops[_a].Set(true);
	}

	void BaseController::StopAll() {
		DBOUT(L"BaseController::Stop\n");
		for (uint32_t a = 0; a < nactives; a++)
			StopOne(a);
	}

	ControllerReturnStatus BaseController::WaitForOne(const uint32_t& _a, const int32_t& _wait_time) {
		ATLASSERT(_a < nactives);
		if (futures[_a].valid()) {
			std::future_status state(std::future_status::ready);

			// Wait (either indefinitely or _wait_time
			if (_wait_time == -1)
				futures[_a].wait();
			else
				state = futures[_a].wait_for(std::chrono::milliseconds(_wait_time));

			// If async returned, get the future's value here
			if (state == std::future_status::ready)
				return futures[_a].get();
			// Landing here means, wait_for ran into a timeout
			else
				throw ScopeException("WaitForOne did not succeed, async thread did not return");
		}
		else
			DBOUT(L"WaitForOne future is not valid (async worker function is not running)");

		return ControllerReturnStatus::error;
	}

	ControllerReturnStatus BaseController::WaitForAll(const int32_t& _wait_time) {
		ControllerReturnStatus totalresult(ControllerReturnStatus::none);
		for (uint32_t a = 0; a < nactives; a++) {
			try {
				totalresult = (ControllerReturnStatus)(totalresult | WaitForOne(a, _wait_time));
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
		}
		DBOUT(L"BaseController::WaitForAll returns\n");
		return totalresult;
	}

}