#pragma once
#include "ScopeDefines.h"
#include "BaseController.h"
#include "BaseController_p.h"

// Forward declarations
template<class T> class SynchronizedQueue;
template<class T> class ScopeMessage;
namespace scope {
	class DaqChunk;
	class DaqChunkResonance;
	class ScannerVectorFrameBasic;
	typedef std::shared_ptr<ScannerVectorFrameBasic> ScannerVectorFrameBasicPtr;
	typedef std::shared_ptr<DaqChunk> DaqChunkPtr;
	typedef std::shared_ptr<DaqChunkResonance> DaqChunkResonancePtr;
	class ScopeMultiImage;
	typedef std::shared_ptr<ScopeMultiImage> ScopeMultiImagePtr;
	class ScopeMultiImageResonanceSW;
	typedef std::shared_ptr<ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWPtr;
	namespace parameters {
		class Scope;
	}
	enum ControllerReturnStatus;
}

namespace scope {

/** The PipelineController controls everything from pixels to complete images, i.e. pixel-mapping averaging and distributing to DisplayController and/or StorageController
* @ingroup ScopeControl */
class PipelineController
	: public BaseController<SCOPE_NAREAS> {

protected:
	/** The hidden implementation class */
	class Impl;

protected:
	/** disable copy */
	PipelineController(PipelineController& other);

	/** disable assignment */
	PipelineController& operator=(PipelineController& other);

	/** @return pointer to the hidden implementation */
	Impl* const Pimpl() const;

public:
	/** Connects queues and gets parameters */
	PipelineController(std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS>* const _iqueues
		, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _squeue
		, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _dqueue
		, const parameters::Scope& _parameters);
	
	~PipelineController();

	/** Sets a scanner vector. Only called on startup. Calls PipelineControllerImpl::SetScannerVector */
	void SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv);
};

}