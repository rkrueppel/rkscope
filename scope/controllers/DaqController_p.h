#pragma once

#include "DaqController.h"

namespace scope {

/** The implementation class of the DaqController. Every area runs in one thread. */
class DaqController::Impl 
	: public BaseController<SCOPE_NAREAS>::Impl {


	/** a ScopeLogger kept handy here */
	ScopeLogger scope_logger;

protected:
	/** disable copy */
	Impl(const Impl& i);
	
	/** disable assignment */
	Impl operator=(const Impl& i);
};



}