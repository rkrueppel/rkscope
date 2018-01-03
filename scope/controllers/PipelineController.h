#pragma once

#include "BaseController.h"
#include "config\config_choices.h"
#include "helpers/SyncQueues.h"
#include "parameters/Scope.h"
#include "TheScopeCounters.h"
#include "helpers/DaqChunks.h"
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
		ScopeCounters<config::nmasters>& counters;
		
		/** input queue from the DaqController */
		std::array<SynchronizedQueue<ScopeMessage<config::DaqChunkPtrType>>, config::threads_daq>* const input_queues;

		/** output queue to the StorageController */
		SynchronizedQueue<ScopeMessage<config::MultiImagePtrType>>* const storage_queue;

		/** output queue to the DisplayController */
		SynchronizedQueue<ScopeMessage<config::MultiImagePtrType>>* const display_queue;

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
			, ScopeCounters<config::nmasters>& _counters
			, std::array<SynchronizedQueue<ScopeMessage<config::DaqChunkPtrType>>, config::threads_daq>* const _iqueues
			, SynchronizedQueue<ScopeMessage<config::MultiImagePtrType>>* const _squeue
			, SynchronizedQueue<ScopeMessage<config::MultiImagePtrType>>* const _dqueue
		);
			
		~PipelineController();
		
		void StopOne(const uint32_t& _a) override;
		
		/** Handles update of parameters during scanning */
		void OnlineParameterUpdate(const parameters::MasterArea& _areaparameters);

		/** Sets the pointers to the scanner vector. Only called on startup. */
		void SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv);
	};

}