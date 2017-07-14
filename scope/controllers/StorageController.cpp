#include "stdafx.h"
#include "StorageController_p.h"

namespace scope {

StorageController::StorageController(SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue, const parameters::Scope& _parameters)
	: BaseController( new StorageController::Impl(_iqueue, _parameters) ) {
}

StorageController::~StorageController() {
}

StorageController::Impl* const StorageController::Pimpl() const {
	return static_cast<StorageController::Impl*>(BaseController<1>::Pimpl());
}


}