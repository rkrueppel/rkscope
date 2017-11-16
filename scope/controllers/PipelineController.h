#pragma once
#include "ScopeDefines.h"
#include "BaseController.h"
#include "helpers/SyncQueues.h"
#include "parameters/Scope.h"
#include "TheScopeCounters.h"
#include "helpers/DaqChunk.h"
#include "helpers/DaqChunkResonance.h"
#include "helpers/ScopeMultiImage.h"
#include "scanmodes/PixelmapperBasic.h"
#include "scanmodes/ScannerVectorFrameBasic.h"
#include "helpers/ScopeDatatypes.h"

namespace scope {

/** The PipelineController controls everything from pixels to complete images, i.e. pixel-mapping averaging and distributing to DisplayController and/or StorageController
* @ingroup ScopeControl */
class PipelineController
	: public BaseController {

protected:
	/** Reference to TheScope's gui parameters */
	parameters::Scope& guiparameters;
	
	/** Reference to TheScope's counters */
	ScopeCounters& counters;
	
	/** input queue from the DaqController */
	std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>>* const input_queues;

	/** output queue to the StorageController */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const storage_queue;

	/** output queue to the DisplayController */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const display_queue;

	/** array with the scanner vectors */
	std::vector<ScannerVectorFrameBasicPtr> scannervecs;

	/** protection for online updates during live scanning */
	std::vector<std::mutex> online_update_mutexe;

	/** trigger for online updates during live scanning */
	std::vector<bool> online_updates;
	
protected:
	/** disable copy */
	PipelineController(PipelineController& other) = delete;

	/** disable assignment */
	PipelineController& operator=(PipelineController& other) = delete;
	
	/** Main function for running pixel mapping. It is executed asynchronously. */
	ControllerReturnStatus Run(StopCondition* const sc, const uint32_t& _area) override;

public:
	/** Connects queues and gets parameters */
	PipelineController(const uint32_t& _nactives
		, parameters::Scope& _guiparameters
		, ScopeCounters& _counters
		, std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>>* const _iqueues
		, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _squeue
		, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _dqueue
	);
		
	~PipelineController();
	
	void StopOne(const uint32_t& _a) override;
	
	/** Handles update of parameters during scanning */
	void OnlineParameterUpdate(const parameters::Area& _areaparameters);

	/** Sets the pointers to the scanner vector. Only called on startup. */
	void SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv);
};

}