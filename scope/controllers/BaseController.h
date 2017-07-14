#pragma once

#include "helpers/helpers.h"

// Forward declarations
namespace scope {
	namespace parameters {
		class Scope;
	}
	enum ControllerReturnStatus;
}

namespace scope {

/** Base class for all controllers.
* @tparam N_ACTIVES the number of active threads
* @tparam STATIC_PIMPL is the pimpl pointer static (if not we have to delete it in the destructor!), implementin the static pimpl (as a static local variable has to be done in derived classes, see e.g. ScopeController!)
* @ingroup ScopeControl */
template<uint32_t N_ACTIVES = 1, bool STATIC_PIMPL = false>
class BaseController {

public:
	BaseController()
		: pimpl(new Impl) {
	}
	
	/** Deletes the pimpl unless it points to a static local variable */
	virtual ~BaseController() {
		if (!STATIC_PIMPL)
			delete pimpl;
	}

private:
	/** disable copy */
	BaseController(const BaseController& i);

	/** disable assignment */
	BaseController operator=(const BaseController& i);

protected:
	/** Hidden implementation class */
	class Impl;

	/** the pointer to the implementation */
	Impl* const pimpl;

	/** Constructor which can be used from derived classes to store their implementation class in BaseControllers pimpl.
	* Stores a pimpl pointer in BaseController::Impl* pimpl (we use this to store pointers to derived Impl classes in the BaseControllers pimpl).
	* @param[in] _pimpl a pimpl pointer */
	explicit BaseController(Impl* const _pimpl)
		: pimpl(_pimpl) {
	}													

public:
	/** @return a pointer to the hidden implementation */
	Impl* Pimpl() const {
		return pimpl;
	}

	/** Start the controller, calls BaseController::Impl::Start */
	void Start(const parameters::Scope& _parameters) {
		pimpl->Start(_parameters);
	}

	/** Stop the controller, calls BaseController::Impl::StopAll */
	void StopAll() {
		pimpl->StopAll();
	}

	/** Wait until the controller threads finished, calls BaseController::Impl::WaitForAll
	* @param[in] _wait_time time to wait for finish in milliseconds */
	ControllerReturnStatus WaitForAll(const int32_t& _wait_time = -1) {
		return pimpl->WaitForAll(_wait_time);
	}

};

}