#pragma once

#include "ScopeDefines.h"
#include "BaseController.h"
#include "BaseController_p.h"

// Forward declarations
template<class T> class SynchronizedQueue;
template<class T> class ScopeMessage;
namespace scope {
	namespace parameters {
		class Scope;
		class Area;
	}
	class ScopeMultiImage;
	typedef std::shared_ptr<ScopeMultiImage> ScopeMultiImagePtr;
	class ScopeMultiImageResonanceSW;
	typedef std::shared_ptr<ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWPtr;
	namespace gui {
		class CChannelFrame;
	class CHistogramFrame;
	}
}

namespace scope {

/** The display controller handles displaying images and histograms.
* It has an Active object (since it derives from BaseController) but the work is actually done in Active objects inside CChannelFrame/View and CHistogramFrame/View.
* I choose this solution because much stuff is dependent on the size of the actual window and the scaling is better with one worker thread per window.
* The DisplayController now merely keeps track of which windows exist and transfers pointer to the current image to these.
* @ingroup ScopeControl */
class DisplayController
	: public BaseController<1> {

protected:
	/** the hidden implementation class */
	class Impl;

protected:
	/** disable copy */
	DisplayController(DisplayController& other);

	/** disable assignment */
	DisplayController& operator=(DisplayController& other);

	/** Return a pointer to the hidden implementation */
	Impl* const Pimpl() const;

public:
	/** Connect queue and get parameters */
	DisplayController(SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue, const parameters::Scope& _parameters);

	/** Request resize of CChannelFrame observers to cope with new image size. Calls DisplayController::Impl::ResolutionChange */
	void ResolutionChange(const parameters::Area& _parameters);

	/** @name These are called by CChannelFrame and CHistogramFrame
	* @{ */
	/** calls DisplayController::Impl::SetHistogramLimits */
	void SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper);

	/** Attaches a CChannelFrame as an observer. Calls DisplayController::Impl::AttachFrame. */
	void AttachFrame(gui::CChannelFrame* const cframe);

	/** Detaches a CChannelFrame as an observer. Calls DisplayController::Impl::DetachFrame. */
	void DetachFrame(gui::CChannelFrame* const cframe);

	/** Attaches a CHistogramFrame as an observer. Calls DisplayController::Impl::AttachFrame. */
	void AttachFrame(gui::CHistogramFrame* const hframe);

	/** @return true if alread at least one histogram attached for that area. Calls DisplayController::Impl::HistogramAlreadyAttached. */
	bool HistogramAlreadyAttached(const uint32_t& _area) const;

	/** Detaches a CHistogramFrame as an observer. Calls DisplayController::Impl::DetachFrame. */
	void DetachFrame(gui::CHistogramFrame* const hframe);

	/** Adds frames to WindowCollection of ScopeController::GuiParameters. Calls DisplayController::Impl::AddFramesToWindowCollection. */
	parameters::WindowCollection GetWindowCollection() const;
	/** @} */
};

}
