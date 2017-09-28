#pragma once

#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"
#include "Plane.h"
#include "Daq.h"
#include "devices/InputsDAQmx.h"
#include "devices/OutputsDAQmx.h"

namespace scope {
	
namespace parameters {

using boost::property_tree::wptree;

/** Parameters for the ScannerVector
* @ingroup ScopeParameters */
class ScannerVector
	: public Base {

protected:
	/** typedef for change signal slot functions */
	typedef boost::signals2::signal<void ()> signalchange_t;

public:
	/** @return total number of pixels that are generated/acquired for one daq frame (including cutoff etc) */
	virtual uint32_t TotalPixels() const { return 0; }

	/** @return total number of pixels for one image (can be different from the number of pixels for one daq cycle, see BiDi) */
	virtual uint32_t TotalPixelsOneFrame() const { return 0; }
};

/** Parameters for a ScannerVectorLine
* @ingroup ScopeParameters */
class ScannerVectorLine
	: public ScannerVector {
};

/** Parameters for a ScannerVectorFrameBasic
* @ingroup ScopeParameters */
class ScannerVectorFrameBasic
	: public ScannerVector {

public:
	/** The class for presets */
	class Preset {
	public:
		/** name of the preset */
		ScopeString name;																								
	
		ScopeNumber<double> pixeltime;

		ScopeNumber<double> scannerdelay;

		ScopeNumber<uint32_t> averages;

		/** x resolution of the image (linewidth) */
		ScopeNumber<uint32_t> xres;												
	
		/** y resolution of the image (lines) */
		ScopeNumber<uint32_t> yres;

		/** aspect ratio in x direction */
		ScopeNumber<double> xaspectratio;
	
		/** aspect ratio in y direction */
		ScopeNumber<double> yaspectratio;

		/** pixels are square (thus FOV rectangular) when true, when false pixels are rectangular (thus FOV square) depending on xres vs yres */
		ScopeNumber<bool> squarepixels;

		Preset();

		/** Load from boost property tree */
		virtual void Load(const wptree& pt);

		/** Save to boost property tree */
		virtual void Save(wptree& pt) const;
	};

	/** A vector with presets */
	std::vector<std::shared_ptr<Preset>> presets;

	ScannerVectorFrameBasic();

	/** Create function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create() { return std::make_unique<ScannerVectorFrameBasic>(); }

	/** Create copy function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create(const ScannerVectorFrameBasic& _o) { return std::make_unique<ScannerVectorFrameBasic>(_o); }

	/** Factory method to generate parameter sets for different scan types and put them into a ScannerVectorFrameBasic pointer. */
	static std::unique_ptr<ScannerVectorFrameBasic> Factory(const ScannerVectorType& _type, const ScannerVectorFrameBasic* const _o = nullptr);

	/** Connect a slot that is called if some member parameter changes, this can be used to copy certain changes
	* in a master area to its slave areas. Except changes in parameters that are area specific (e.g. pockels).
	* @return a vector with all connections made, for storage and destructor call
	* @param[in] _slot The slot that is connected to all ScopeValues in the ScannerVector */
	virtual std::vector<boost::signals2::connection> ConnectCopyTrigger(signalchange_t::slot_type _slot);

	/** Connect a slot that is called if something in the ScannerVector changes that renders a recalculation
	* of dependent rates (e.g. framerate in parameters::Area) necessary
	* @param[in] _slot The slot that is connected to certain ScopeValues in the ScannerVector */
	virtual void ConnectRateUpdate(signalchange_t::slot_type _slot);

	/** Connect a slot that is called if something in the ScannerVector changes that renders a recalculation
	* of the resolution (e.g. through in parameters::Area::CalculateResolution) necessary
	* @param[in] _slot The slot that is connected to certain ScopeValues in the ScannerVector */
	virtual void ConnectResolutionUpdate(signalchange_t::slot_type _slot);

	/** Connect a slot that is called if something in the ScannerVector changes that renders a recalculation
	* of the micron per pixel (e.g. through in parameters::Area::CalculateMicronPerPixel) necessary
	* @param[in] _slot The slot that is connected to certain ScopeValues in the ScannerVector */
	virtual void ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot);

	/** Connect a slot that is called if xres or yres in the ScannerVector are changed
	* This usually is ScopeControllerImpl::ResolutionChange
	* @param[in] _slot The slot that is connected to xres and yres ScopeValues in the ScannerVector */
	virtual void ConnectResolutionChange(signalchange_t::slot_type _slot);

	/** Connect a slot that is called if something in the ScannerVector (which is displayed on the GUI)
	* changes that makes an online update during scanning necessary
	* @param[in] _slot The slot that is connected to certain/all ScopeValues in the ScannerVector, mainly
	* this would be ScopeControllerImpl::UpdateAreaParametersFromGui */
	virtual void ConnectOnlineUpdate(signalchange_t::slot_type _slot);

	/** x resolution of the image (linewidth) */
	ScopeNumber<uint32_t> xres;

	/** y resolution of the image (lines) */
	ScopeNumber<uint32_t> yres;

	/** aspect ratio in x direction */
	ScopeNumber<double> xaspectratio;
	
	/** aspect ratio in y direction */
	ScopeNumber<double> yaspectratio;
	
	/** pixels are square (thus FOV rectangular) when true, when false pixels are rectangular (thus FOV square) depending on xres vs yres */
	ScopeNumber<bool> squarepixels;

	/** current zoom factor (from 1 to 20). With the PXI-6259 analog output range of +-5V and 16bit resolution
	* the smallest voltage step is 0.15mV. Having zoom 20 and 512 pixels corresponds to a step size of 0.98 mV. */
	ScopeNumber<double> zoom;

	/** x offset */
	ScopeNumber<double> xoffset;

	/** y offset */
	ScopeNumber<double> yoffset;

	/** current fast z position */
	ScopeNumber<double> fastz;

	/** pockels cell value */
	ScopeNumber<double> pockels;

	/** Rotation angle around the X axis in degrees */
	ScopeNumber<double> xrotation;

	/** Rotation angle around the Y axis in degrees */
	ScopeNumber<double> yrotation;

	/** @return number of x pixels (columns) in the final image */
	virtual uint32_t XImagePixels() const { return xres(); }

	/** @return number of y pixels (lines) in the final image */
	virtual uint32_t YImageLines() const { return yres(); }
	
	/** @return number of totally acquired pixels in one line */
	virtual uint32_t XTotalPixels() const { return xres(); }

	/** @return number of totally acquired y pixels/lines  */
	virtual uint32_t YTotalLines() const { return yres(); }
	
	uint32_t TotalPixels() const override { return TotalPixelsOneFrame(); }

	uint32_t TotalPixelsOneFrame() const override { return xres() * yres(); }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

	/** Since preset loading is done in base class (here), we need to generate a derived preset, this can be done by calling this
	* function and overriding it in derived classes. (Be careful which classes Preset class is meant!) */
	virtual std::unique_ptr<ScannerVectorFrameBasic::Preset> MakePreset() const;

	/** Save parameters into a preset */
	virtual void SaveToPreset(const std::wstring& _name, const Daq& _daq) {}

	/** Load parameters from a preset */
	virtual void LoadFromPreset(const std::wstring& _name, Daq& _daq) {}

	/** Deletes a preset */
	virtual void DeletePreset(const std::wstring& _name);

	/** Changes the limits of xoffset and yoffset to account for the maximum allowed offset depending on zoom */
	virtual void CoerceOffset();
};

/** Parameters for a ScannerVectorFrameSaw
* @ingroup ScopeParameters */
class ScannerVectorFrameSaw
	: public ScannerVectorFrameBasic {

public:
	/** The class for presets */
	class Preset
		: public ScannerVectorFrameBasic::Preset {
	public:
		/** Cutoff fraction at beginning of line, total pixels per line = xres * (1 + xcutoff + xretrace) */
		ScopeNumber<double> xcutoff;											
	
		/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
		ScopeNumber<double> ycutoff;
	
		/** Cutoff fraction for retrace at end of line */
		ScopeNumber<double> xretrace;
	
		/** Cutoff fraction for retrace at end of frame */
		ScopeNumber<double> yretrace;

		Preset();

		void Load(const wptree& pt) override;
		void Save(wptree& pt) const override;
	};

	ScannerVectorFrameSaw();

	/** Create function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create() { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameSaw()); }
	
	/** Create copy function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create(const ScannerVectorFrameSaw& _o) { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameSaw(_o)); }
	
	std::vector<boost::signals2::connection> ConnectCopyTrigger(signalchange_t::slot_type _slot) override;

	void ConnectRateUpdate(signalchange_t::slot_type _slot) override;

	void ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) override;

	void ConnectOnlineUpdate(signalchange_t::slot_type _slot) override;

	/** Cutoff fraction at beginning of line, total pixels per line = xres * (1 + xcutoff + xretrace) */
	ScopeNumber<double> xcutoff;										
	
	/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
	ScopeNumber<double> ycutoff;											
	
	/** Cutoff fraction for retrace at end of line */
	ScopeNumber<double> xretrace;											
	
	/** Cutoff fraction for retrace at end of frame */
	ScopeNumber<double> yretrace;											
	
	/** @return number of additional cutoff pixels at linestart */
	uint32_t XCutoffPixels() const { return round2ui32(xcutoff()*static_cast<double>(XImagePixels())); }		
	
	/** @return number of pixels during scan ramp (image pixels + cutoff pixels) */
	uint32_t XScanPixels() const { return XImagePixels() + XCutoffPixels(); }						
	
	/** @return number of pixels during retrace */
	uint32_t XRetracePixels() const { return round2ui32(xretrace()*static_cast<double>(XImagePixels())); }		
	
	/** @return total number of pixels per line (image pixels + cutoff pixels + retrace pixels) */
	uint32_t XTotalPixels() const override { return XCutoffPixels()+XImagePixels()+XRetracePixels(); }		
	
	/** @return number of additional cutoff lines at framestart */
	uint32_t YCutoffLines() const { return round2ui32(ycutoff()*static_cast<double>(YImageLines())); }
	
	/** @return number of lines during y scan ramp (image lines + cutoff lines) */
	uint32_t YScanLines() const { return YImageLines() + YCutoffLines(); }
	
	/** @return number of lines during retrace */
	uint32_t YRetraceLines() const { return round2ui32(yretrace()*static_cast<double>(YImageLines())); }
	
	/** @return total number of y pixels/lines (image lines + cutoff lines + retrace lines) */
	uint32_t YTotalLines() const override { return YCutoffLines()+YImageLines()+YRetraceLines(); }
	
	/** @return total number of pixels (final image + cutoff portions + retrace portions) */
	uint32_t TotalPixels() const override { return TotalPixelsOneFrame(); }

	/** @return total number of pixels for one image */
	uint32_t TotalPixelsOneFrame() const override { return XTotalPixels()*YTotalLines(); }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

	std::unique_ptr<ScannerVectorFrameBasic::Preset> MakePreset() const override;

	void SaveToPreset(const std::wstring& _name, const Daq& _daq) override;
	void LoadFromPreset(const std::wstring& _name, Daq& _daq) override;
};

/** Parameters for a ScannerVectorFrameBiDi
* @ingroup ScopeParameters */
class ScannerVectorFrameBiDi
	: public ScannerVectorFrameBasic {

public:
	/** The class for presets */
	class Preset
		: public ScannerVectorFrameBasic::Preset {
	public:
		/** fraction spent for turning the x galvo around */
		ScopeNumber<double> xturnfraction;

		/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
		ScopeNumber<double> ycutoff;

		/** Cutoff fraction for retrace at end of frame */
		ScopeNumber<double> yretrace;
	
		Preset();

		void Load(const wptree& pt) override;
		void Save(wptree& pt) const override;
	};

	ScannerVectorFrameBiDi();

	/** Create function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create() { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameBiDi()); }

	/** Create copy function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create(const ScannerVectorFrameBiDi& _o) { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameBiDi(_o)); }

	std::vector<boost::signals2::connection> ConnectCopyTrigger(signalchange_t::slot_type _slot) override;

	void ConnectRateUpdate(signalchange_t::slot_type _slot) override;

	void ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) override;

	void ConnectOnlineUpdate(signalchange_t::slot_type _slot) override;

	/** fraction spent for turning the x galvo around */
	ScopeNumber<double> xturnfraction;
	
	/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
	ScopeNumber<double> ycutoff;

	/** Cutoff fraction for retrace at end of frame */
	ScopeNumber<double> yretrace;

	/** @return number of pixels per line spent for turning. */
	uint32_t XTurnPixels() const { return round2ui32(static_cast<double>(XImagePixels()) * xturnfraction()); }

	/** @return total number of pixels per line (image pixels + turn pixels) */
	uint32_t XTotalPixels() const override { return XImagePixels() + XTurnPixels(); }

	/** @return number of additional cutoff lines at framestart */
	uint32_t YCutoffLines() const { return round2ui32(ycutoff()*static_cast<double>(YImageLines())); }
	
	/** @return number of lines during y scan ramp (image lines + cutoff lines) */
	uint32_t YScanLines() const { return YImageLines() + YCutoffLines(); }
	
	/** @return number of lines during retrace */
	uint32_t YRetraceLines() const { return round2ui32(yretrace()*static_cast<double>(YImageLines())); }
	
	/** @return total number of y pixels/lines (image lines + cutoff lines + retrace lines) */
	uint32_t YTotalLines() const override { return YCutoffLines()+YImageLines()+YRetraceLines(); }
	
	
	/** @return total number of pixels */
	uint32_t TotalPixels() const override { return XTotalPixels()*YTotalLines(); }

	/** @return total number of pixels for one image */
	uint32_t TotalPixelsOneFrame() const override { return XTotalPixels()*YTotalLines(); }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

	std::unique_ptr<ScannerVectorFrameBasic::Preset> MakePreset() const override;

	void SaveToPreset(const std::wstring& _name, const Daq& _daq) override;
	void LoadFromPreset(const std::wstring& _name, Daq& _daq) override;
};

/** Parameters for a ScannerVectorFramePlaneHopper
* @ingroup ScopeParameters */
class ScannerVectorFramePlaneHopper
	: public ScannerVectorFrameBasic {

public:
	/** The class for presets */
	class Preset
		: public ScannerVectorFrameBasic::Preset {
	public:
		/** vector with properties for all planes.  */
		std::vector<PlaneProperties> planes;
	
		Preset();

		void Load(const wptree& pt) override;
		void Save(wptree& pt) const override;
	};

	ScannerVectorFramePlaneHopper();

	/** Create function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create() { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFramePlaneHopper()); }

	/** Create copy function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create(const ScannerVectorFramePlaneHopper& _o) { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFramePlaneHopper(_o)); }

	std::vector<boost::signals2::connection> ConnectCopyTrigger(signalchange_t::slot_type _slot) override;

	void ConnectRateUpdate(signalchange_t::slot_type _slot) override;

	void ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) override;

	void ConnectOnlineUpdate(signalchange_t::slot_type _slot) override;

	/** vector with properties for all planes */
	std::vector<PlaneProperties> planes;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

	std::unique_ptr<ScannerVectorFrameBasic::Preset> MakePreset() const override;

	void SaveToPreset(const std::wstring& _name, const Daq& _daq) override;
	void LoadFromPreset(const std::wstring& _name, Daq& _daq) override;
};

/** Parameters for a ScannerVectorFrameResonance
* @ingroup ScopeParameters */
class ScannerVectorFrameResonance
	: public ScannerVectorFrameBasic {

public:
	/** The class for presets */
	class Preset
		: public ScannerVectorFrameBasic::Preset {
	public:
		/** fraction spent for turning the x resonance scanner around */
		ScopeNumber<double> xturnfraction;

		/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
		ScopeNumber<double> ycutoff;

		/** Cutoff fraction for retrace at end of frame */
		ScopeNumber<double> yretrace;
	
		/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the storage queue (fixes a problem of wrong chunks in the wrong frames) */
		ScopeNumber<uint32_t> waitafterenqueuestorage;

		/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the display queue (fixes a problem of wrong chunks in the wrong frames) */
		ScopeNumber<uint32_t> waitafterenqueuedisplay;

		Preset();

		void Load(const wptree& pt) override;
		void Save(wptree& pt) const override;
	};

	ScannerVectorFrameResonance();

	/** Create function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create() { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameResonance()); }

	/** vector with properties for all planes */
	std::vector<PlaneProperties> planes;

	/** Create copy function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create(const ScannerVectorFrameResonance& _o) { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameResonance(_o)); }

	std::vector<boost::signals2::connection> ConnectCopyTrigger(signalchange_t::slot_type _slot) override;

	void ConnectRateUpdate(signalchange_t::slot_type _slot) override;

	void ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) override;

	void ConnectOnlineUpdate(signalchange_t::slot_type _slot) override;

	/** fraction spent for turning the x resonance scanner around */
	ScopeNumber<double> xturnfraction;
	
	/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
	ScopeNumber<double> ycutoff;

	/** Cutoff fraction for retrace at end of frame */
	ScopeNumber<double> yretrace;

	/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the storage queue (fixes a problem of wrong chunks in the wrong frames) */
	ScopeNumber<uint32_t> waitafterenqueuestorage;

	/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the display queue (fixes a problem of wrong chunks in the wrong frames) */
	ScopeNumber<uint32_t> waitafterenqueuedisplay;

	/** @return number of pixels per line spent for turning. */
	uint32_t XTurnPixels() const { return round2ui32(static_cast<double>(XImagePixels()) * xturnfraction()); }

	/** @return number of pixels per line spent for turning on the left side of the image. */
	uint32_t XTurnPixelsLeft() const { return static_cast<uint32_t>(floor(static_cast<double>(XTurnPixels()) / 2)); }
	
	/** @return number of pixels per line spent for turning on the right side of the image. */
	uint32_t XTurnPixelsRight() const { return static_cast<uint32_t>(ceil(static_cast<double>(XTurnPixels()) / 2)); }

	/** @return total number of pixels per line (image pixels + turn pixels) */
	uint32_t XTotalPixels() const override { return XImagePixels() + XTurnPixels(); }

	/** @return number of additional cutoff lines at framestart */
	uint32_t YCutoffLines() const { return 2 * round2ui32(ycutoff()*static_cast<double>(YImageLines())/2); }
	
	/** @return number of lines during y scan ramp (image lines + cutoff lines) */
	uint32_t YScanLines() const { return YImageLines() + YCutoffLines(); }
	
	/** @return number of lines during retrace */
	uint32_t YRetraceLines() const { return 2 * round2ui32(yretrace()*static_cast<double>(YImageLines())/2); }
	
	/** @return total number of y pixels/lines (image lines + cutoff lines + retrace lines) */
	uint32_t YTotalLines() const override { return YCutoffLines()+YImageLines()+YRetraceLines(); }

	/** @return total number of pixels */
	uint32_t TotalPixels() const override { return XTotalPixels()*YTotalLines(); }

	/** @return total number of pixels for one image */
	uint32_t TotalPixelsOneFrame() const override { return XTotalPixels()*YTotalLines(); }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

	std::unique_ptr<ScannerVectorFrameBasic::Preset> MakePreset() const override;

	void SaveToPreset(const std::wstring& _name, const Daq& _daq) override;
	void LoadFromPreset(const std::wstring& _name, Daq& _daq) override;

	/** Changes the y resolution to the allowed values for the resonance scanner  */
	virtual void CoerceYResolution();

	/** Updates the x turn fraction for the resonance scanner  */
	virtual void UpdateXTurnFraction();
};


/** Parameters for a ScannerVectorFrameResonanceHopper
* @ingroup ScopeParameters */
class ScannerVectorFrameResonanceHopper
	: public ScannerVectorFrameBasic {

public:
	/** The class for presets */
	class Preset
		: public ScannerVectorFrameBasic::Preset {
	public:
		/** vector with properties for all planes.  */
		std::vector<PlaneProperties> planes;

		/** fraction spent for turning the x resonance scanner around */
		ScopeNumber<double> xturnfraction;

		/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
		ScopeNumber<double> ycutoff;

		/** Cutoff fraction for retrace at end of frame */
		ScopeNumber<double> yretrace;
	
		/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the storage queue (fixes a problem of wrong chunks in the wrong frames) */
		ScopeNumber<uint32_t> waitafterenqueuestorage;

		/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the display queue (fixes a problem of wrong chunks in the wrong frames) */
		ScopeNumber<uint32_t> waitafterenqueuedisplay;

		Preset();

		void Load(const wptree& pt) override;
		void Save(wptree& pt) const override;
	};

	ScannerVectorFrameResonanceHopper();

	/** Create function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create() { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameResonanceHopper()); }

	/** vector with properties for all planes */
	std::vector<PlaneProperties> planes;

	/** Create copy function for factory */
	static std::unique_ptr<ScannerVectorFrameBasic> Create(const ScannerVectorFrameResonanceHopper& _o) { return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameResonanceHopper(_o)); }

	std::vector<boost::signals2::connection> ConnectCopyTrigger(signalchange_t::slot_type _slot) override;

	void ConnectRateUpdate(signalchange_t::slot_type _slot) override;

	void ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) override;

	void ConnectOnlineUpdate(signalchange_t::slot_type _slot) override;

	/** fraction spent for turning the x resonance scanner around */
	ScopeNumber<double> xturnfraction;
	
	/** Cutoff fraction at beginning of frame, total lines per frame = yres * (1 + ycutoff + yretrace) */
	ScopeNumber<double> ycutoff;

	/** Cutoff fraction for retrace at end of frame */
	ScopeNumber<double> yretrace;

	/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the storage queue (fixes a problem of wrong chunks in the wrong frames) */
	ScopeNumber<uint32_t> waitafterenqueuestorage;

	/** Time (in milliseconds) to wait after the PipelineController has enqueued the image to the display queue (fixes a problem of wrong chunks in the wrong frames) */
	ScopeNumber<uint32_t> waitafterenqueuedisplay;

	/** @return number of pixels per line spent for turning. */
	uint32_t XTurnPixels() const { return round2ui32(static_cast<double>(XImagePixels()) * xturnfraction()); }

	/** @return number of pixels per line spent for turning on the left side of the image. */
	uint32_t XTurnPixelsLeft() const { return static_cast<uint32_t>(floor(static_cast<double>(XTurnPixels()) / 2)); }
	
	/** @return number of pixels per line spent for turning on the right side of the image. */
	uint32_t XTurnPixelsRight() const { return static_cast<uint32_t>(ceil(static_cast<double>(XTurnPixels()) / 2)); }

	/** @return total number of pixels per line (image pixels + turn pixels) */
	uint32_t XTotalPixels() const override { return XImagePixels() + XTurnPixels(); }

	/** @return number of additional cutoff lines at framestart */
	uint32_t YCutoffLines() const { return 2 * round2ui32(ycutoff()*static_cast<double>(YImageLines())/2); }
	
	/** @return number of lines during y scan ramp (image lines + cutoff lines) */
	uint32_t YScanLines() const { return YImageLines() + YCutoffLines(); }
	
	/** @return number of lines during retrace */
	uint32_t YRetraceLines() const { return 2 * round2ui32(yretrace()*static_cast<double>(YImageLines())/2); }
	
	/** @return total number of y pixels/lines (image lines + cutoff lines + retrace lines) */
	uint32_t YTotalLines() const override { return YCutoffLines()+YImageLines()+YRetraceLines(); }

	/** @return total number of pixels */
	uint32_t TotalPixels() const override { return XTotalPixels()*YTotalLines(); }

	/** @return total number of pixels for one image */
	uint32_t TotalPixelsOneFrame() const override { return XTotalPixels()*YTotalLines(); }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

	std::unique_ptr<ScannerVectorFrameBasic::Preset> MakePreset() const override;

	void SaveToPreset(const std::wstring& _name, const Daq& _daq) override;
	void LoadFromPreset(const std::wstring& _name, Daq& _daq) override;

	/** Changes the y resolution to the allowed values for the resonance scanner  */
	virtual void CoerceYResolution();

	/** Updates the x turn fraction for the resonance scanner  */
	virtual void UpdateXTurnFraction();
};

}

}