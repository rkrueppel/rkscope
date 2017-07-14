#pragma once

#include "BaseController.h"
#include "ScopeDefines.h"

// Forward declarations
template<class T> class SynchronizedQueue;
template<class T> class ScopeMessage;
namespace scope {
	namespace parameters {
		class Scope;
	}
	class ScopeMultiImage;
	typedef std::shared_ptr<ScopeMultiImage> ScopeMultiImagePtr;
	class ScopeMultiImageResonanceSW;
	typedef std::shared_ptr<ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWPtr;
	enum ControllerReturnStatus;
}

namespace scope {

/** @ingroup ScopeControl
* The StorageController controls the conversion of multi images to TIFF and the storage of those.
* Uses the pimpl pattern to hide the implementation. */
class StorageController
	: public BaseController<1> {

protected:
	/** Implementation class */
	class Impl;

protected:
	/** disable copy */
	StorageController(StorageController& other);

	/** disable assignment */
	StorageController& operator=(StorageController& other);

public:
	/** Connect input queue and take parameters */
	StorageController(SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue, const parameters::Scope& _parameters);
	~StorageController();
	
	/** @return a pointer to the hidden implementation */
	StorageController::Impl* const Pimpl() const;
};

}