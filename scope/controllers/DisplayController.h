#pragma once

#include "ScopeDefines.h"
#include "BaseController.h"
#include "helpers/SyncQueues.h"
#include "parameters/Scope.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "helpers/ScopeDatatypes.h"
#include "gui/ChannelFrame.h"
#include "gui/HistogramFrame.h"
#include "ScopeLogger.h"


namespace scope {

/** The display controller handles displaying images and histograms.
* It has an Active object (since it derives from BaseController) but the work is actually done in Active objects inside CChannelFrame/View and CHistogramFrame/View.
* I choose this solution because much stuff is dependent on the size of the actual window and the scaling is better with one worker thread per window.
* The DisplayController now merely keeps track of which windows exist and transfers pointer to the current image to these.
* There is only one Run thread (not one for every area since the heavy work is done inside CChannelFrame and CHistogramFrame)
* This passes images arriving from the PipelineController to attached CChannelFrame and CHistogramFrame where the actual calculations
* (overlay, histogram) and renderings take place.
* @ingroup ScopeControl */
class DisplayController
	: public BaseController {

protected:
	/** Input queue with multi images from the PipelineController */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const input_queue;
	
	/** Vector of CChannelFrame observers */
	std::vector<std::vector<gui::CChannelFrame*>> channelframes;

	/** Mutex to protect access to that vector */
	mutable std::vector<std::mutex> channelframes_mutexe;

	/** Vector of CHistogramFrame observers */
	std::vector<std::vector<gui::CHistogramFrame*>> histogramframes;

	/** Mutex to protect access to that vector */
	mutable std::vector<std::mutex> histogramframes_mutexe;

	
protected:
	/** disable copy */
	DisplayController(DisplayController& other) = delete;

	/** disable assignment */
	DisplayController& operator=(DisplayController& other) = delete;
	
	/** Main function for managing display. It is executed asynchronously.
	* The actual overlay creations, histogram calculations, and rendering are done in the Active's threads inside
	* CChannelFrame/View and CHistogramFrame/View.
	* Only one Run thread, so _area parameter here is always 0. 
	* We dequeue images from the input queue and distribute to the areas CChannelFrame and CHistogramFrame these belong to. */
	ControllerReturnStatus Run(StopCondition* const sc, const uint32_t& _area) override;
	
	/** We need to override here. The dequeue in Run could block/wait since nothing is in the queue.
	* To stop we put a message with abort tag in, it gets dequeued and we break from the while loop. (In addition we call BaseController::Impl::StopOne which sets the
	* StopCondition to true. */
	void StopOne(const uint32_t& _a) override;
	
	/** Calls CChannelFrame::UpdateStatus and CHistogramFrame::UpdateStatus in all attached frames*/
	void UpdateStatusInFrames(const RunState& _rs);

public:
	/** Connect queue and get parameters */
	DisplayController(const uint32_t& _nactives, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue, const parameters::Scope& _parameters);
	
	~DisplayController();

	/** Go through all CChannelFrames and request resize to cope with new image size
	* @param[in] _ap contains the new x and y resolution of images*/
	void ResolutionChange(const parameters::Area& _ap);

	/** @name These are called by CChannelFrame and CHistogramFrame
	* @{ */
	/** Go through all CChannelFrames of that area and set the histogram limits there
	* @param[in] _area,_channel area and channel to set limits
	* @param[in] _lower,_upper new lower and upper histogram limits*/
	void SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper);

	/** Attaches a CChannelFrame as observer to the DisplayController
	* @param[in] _cframe pointer to the new CChannelFrame observer*/
	void AttachFrame(gui::CChannelFrame* const _cframe);

	/** Detaches a CChannelFrame as observer from the DisplayController
	* @param[in] _cframe pointer to the CChannelFrame to detach
	* @throws std::exception if _cframe is not found */
	void DetachFrame(gui::CChannelFrame* const _cframe);

	/** Attaches a CHistogramFrame as observer to the DisplayController
	* @param[in] _hframe pointer to the new CHistogramFrame observer */
	void AttachFrame(gui::CHistogramFrame* const _hframe);

	/** @return true if alreay at least one histogram for that area attached, used to avoid several histograms for the same
	* area. */
	bool HistogramAlreadyAttached(const uint32_t& _area) const;

	/** Detaches a CHistogramFrame as observer from the DisplayController
	* @param[in] _hframe pointer to the CHistogramFrame to detach
	* @throws std::exception if _hframe is not found */
	void DetachFrame(gui::CHistogramFrame* const  _hframe);

	/** Adds frames to WindowCollection of ScopeController::GuiParameters.*/
	parameters::WindowCollection GetWindowCollection() const;
	/** @} */
};

}
