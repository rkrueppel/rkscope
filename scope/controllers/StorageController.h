#pragma once

#include "BaseController.h"
#include "ScopeDefines.h"
#include "helpers/helpers.h"
#include "helpers/SyncQueues.h"
#include "parameters/Scope.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "helpers/hresult_exception.h"
#include "helpers/ScopeMultiImageEncoder.h"
#include "helpers/ScopeException.h"
#include "ScopeLogger.h"


namespace scope {

/** @ingroup ScopeControl
* The StorageController controls the conversion of multi images to TIFF and the storage of those.
* Uses the pimpl pattern to hide the implementation. */
class StorageController
	: public BaseController {

protected:
	/** Input queue with multi images from the PipelineController */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const input_queue;
	
	/** for continuous file numbering */
	uint32_t runcounter;

	/** Keep track of filenames */
	std::vector<std::vector<std::wstring>> filenames;

	/** the encoders (encapsulating the WIC stuff) */
	std::vector<std::unique_ptr<ScopeMultiImageEncoder>> encoders;

	parameters::Scope& ctrlparams;
	
protected:
	/** disable copy */
	StorageController(StorageController& other) = delete;

	/** disable assignment */
	StorageController& operator=(StorageController& other) = delete;
	
	/** Main function for running data conversion to TIFF and storage. It is executed asynchronously. */
	ControllerReturnStatus Run(StopCondition* const sc, const uint32_t& _area) override;
	
	/** Create folder.Fformat is: "folder/date/time_runmode/" */
	std::wstring StorageController::CreateFolder();
	
	/** Creates and initializes the encoders */
	void InitializeEncoders(const bool& _dosave, const std::wstring& _foldername);
	
	/** Write correct Tiff flags into files (fix the ones that are by default written by the WIC.
	* Use a format (writing into the ImageDescription tag) that ImageJ will recognise.
	* Run exiftool.exe in a separate process (tried libexiv2, but that could not overwrite some tags). */
	void FixTIFFTags();

public:
	/** Connect input queue and take parameters */
	StorageController(const uint32_t& _nactives, parameters::Scope& _parameters, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue);
	
	/** Stop the controller and interrupt thread if necessary */
	~StorageController();
	
	/** We need to override here. The dequeue in Run could block/wait since nothing is in the queue.
	* To stop we put a message with abort tag in, it gets dequeued and we break from the while loop. (In addition we call BaseController::Impl::StopOne which sets the
	* StopCondition to true. */
	void StopOne(const uint32_t& _a) override;
	
};

}