#pragma once
#include "ScopeDefines.h"
#include "version.h"
#include "BaseController.h"
#include "ScopeControllerModes.h"
#include "ScopeControllerCounters.h"
#include "ScopeControllerButtons.h"
#include "scanmodes/ScannerVectorFrameBasic.h"
#include "scanmodes/ScannerVectorFrameSaw.h"
#include "scanmodes/ScannerVectorFrameBiDi.h"
#include "scanmodes/ScannerVectorFramePlaneHopper.h"
#include "scanmodes/ScannerVectorFrameResonanceBiDi.h"
#include "scanmodes/ScannerVectorFrameResonanceHopper.h"
#include "helpers/ScopeMultiImage.h"
#include "helpers/ScopeMultiImageResonanceSW.h"
#include "DaqController.h"
#include "PipelineController.h"
#include "DisplayController.h"
#include "StorageController.h"
#include "helpers/ScopeException.h"
#include "FPUController.h"
#include "ScopeLogger.h"
#include "gui/ChannelFrame.h"
#include "gui/HistogramFrame.h"
#include "devices/xyz/XYZControl.h"
#include "devices/xyz/XYZControlGalil.h"
#include "devices/xyz/XYZControlSutter.h"
#include "devices/GaterDAQmx.h"
#include "helpers/ScopeButton.h"
#include "helpers/ScopeScanModeButton.h"
#include "helpers/ScopeNumber.h"
#include "parameters/Scope.h"
#include "helpers/ScopeDatatypes.h"


// We need the type of the FPGAStatus struct for FPGAFIFOStatus here
#include "devices\fpga\FPGAPhotonCounter.h"
#include "devices\fpga\FPGADigitalDemultiplexer.h"
#include "devices\fpga\FPGAAnalogIntegrator.h"
#include "devices\fpga\FPGAAnalogDemultiplexer.h"
#include "devices\fpga\FPGAAnalogDemultiplexerResonance.h"
#include "devices\fpga\FPGANoiseOutput.h"
#include "devices\fpga\FPGAResonanceScanner.h"
#include "devices\fpga\FPGAResonanceScanner_NI5771.h"

namespace scope {

/** @defgroup ScopeControl Controller classes
* Classes controlling various aspects of data acquisition, generation, handling, displaying, storing, ...
* All classes derive from BaseController, i.e. they have one or more threads inside and run their stuff inside these. */

/** @defgroup ScopeComponentsHardware Scope hardware component classes
* Classes that model various hardware devices and parts of the microscope. */

/** @ingroup ScopeControl
* Main controller of microscope hardware and acquisition, also interface to the GUI.
* Implements: Singleton, ActiveObject, Pimpl
* @warning The singleton is not completely threadsafe but we do not really need this,
* since call of ScopeController.instance().version() in main (there only one thread exists yet)
* ensures safe initialization. */
class ScopeController
	: public BaseController {

protected:
	
	const uint32_t nareas;
	
	/** Scanner vectors
	* @{ */
	/** The scanner vector for frame scanning */
	std::vector<ScannerVectorFrameBasicPtr> framescannervecs;
	/** @} */

	/** thread-safe bool to signal a requested abort of a stack scan */
	StopCondition repeat_abort;

	/** for abortable waiting between timeseries repeats */
	std::condition_variable waitbetweenrepeats_cond;

	/** for abortable waiting between timeseries repeats */
	std::mutex waitbetweenrepeats_mutex;

	/** future for the online update thread */
	std::future<void> onlineupdate_future;

	/** set if an online update is currently running, to prevent to simultaneous online update threads */
	std::atomic<bool> onlineupdate_running;

	/** stores the time to run a scan */
	DWORD time;

	/** list of callbacks for scan mode switching */
	std::vector<std::function<void(const uint32_t&, const ScannerVectorType&)>> scanmodecallbacks;

	/*ScopeController(ScopeController& other);				// we need this copyable for std::bind to work...
	ScopeController& operator=(ScopeController& other); */

public:
	/** Set to true while scanning, GUI elements can connect to this to disable buttons and controls (that are not matched
	* by static ScopeButtons etc here) while scanning. */
	ScopeNumber<bool> readonlywhilescanning;

	/** @name ScopeButtons
	* GUI classes can connect CScopeButtonCtrl to these ScopeButtons via their scope_controller member and thus pass commands to the ScopeController.
	* At the same time, the status of the WTL buttons can be controlled by the ScopeController (e.g. disable while scanning).
	* I.e. ScopeButtons are mediators between the controls (CScopeButtonCtrl) and ScopeController functions to start certain stuff;
	* if it weren't for the button state, CScopeButtonCtrl could directly connect to these functions...
	* @{ */
	
	RunButtons runbuttons;

	StackButtons stackbuttons;

	/** Button to zero main stage coordinates */
	ScopeButton stagezerobutton;

	/** Buttons for FPU nudge */
	std::vector<FPUButtons> fpubuttonsvec;

	/** Buttons for switching the scan mode */
	std::vector<ScanModeButtons> scanmodevec;
	/** @} */

	/** The counters */
	ScopeControllerCounters scopecontrollercounters;


protected:
	/** Sets the state of the GUI buttons for FPU control (via the corresponding ScopeButtons). True = enabled, false = disabled.  */
	void SetFPUButtonsState(const bool& state);

	/** Sets the state of GUI controls (via the corresponding ScopeNumbers). True = read&write/enabled, false = readonly/disabled.
	* Other GUI elements can connect to ReadOnlyWhileScanning. */
	void SetGuiCtrlState();

	/** Called via change of GuiParameters.areas[x].framesaw.scanvec.xres etc. */
	void ResolutionChange(const uint32_t& _area);

	/** Sets the type of scanning. Called via ScopeController ScanModeButtons.
	* Calls the scanmode callbacks.
	* Updates/recreates the scanvectors from a different type and sets them in theDaq and thePipeline */
	void SetScanMode(const uint32_t& _area, const ScannerVectorType& _mode);

	/** Set parameters for scanner vectors, since these are shared_ptr in DaqControllerDAQmx and PipelineController they get updated there too */
	void SetScannerVectorParameters();

	/** Logs the current scan (not live scans) */
	void LogRun();

	/** Starts all controllers with the current parameters */
	void StartAllControllers();

	/** Stops all controllers in the correct order and waits for them to finish. */
	void StopAllControllers();

	/** Waits for all controllers to intrinsically finish. Only good for nframes runmode */
	void WaitForAllControllers();

	/** Clear all queues between controllers */
	void ClearAllQueues();

	/** Worker function to control live scanning (basically only starting everything up) */
	ControllerReturnStatus RunLive(StopCondition* const sc);

	/** Worker function to control a single scan */
	ControllerReturnStatus RunSingle(StopCondition* const sc);

	/** Worker function to control a stack scan */
	ControllerReturnStatus RunStack(StopCondition* const sc);

	/** Worker function to control a timeseries scan */
	ControllerReturnStatus RunTimeseries(StopCondition* const sc);

	/** Worker function to control a behavior triggered scan */
	ControllerReturnStatus RunBehavior(StopCondition* const sc);

	/** Clears queues, resets run state, and reenables controls */
	void ClearAfterStop();

public:
	/** Initializes and connects stuff */
	ScopeController(const uint32_t& _nareas);

	/** Stop whatever is running */
	~ScopeController(void);
	
	/** @return the currently loaded config file */
	std::wstring CurrentConfigFile() const;

	/** Called by CMainDialogFrame::QuitApplication.
	* Stops running threads etc, avoids destruction of these when the static pimpl gets destructed (probably very late or undefined).*/
	void PrepareQuit();

	/** Starts live scanning by running RunLive asynchronously */
	void StartLive();

	/** Start a stack scan by running RunStack asynchronously */
	void StartStack();

	/** Starts a single frame scan by running RunSingle asynchronously */
	void StartSingle();

	/** Starts a timeseries by running RunTimeseries asynchronously */
	void StartTimeseries();

	/** Starts a behavior triggered acquisition by running RunBehavior asynchronously */
	void StartBehavior();

	/** Stops whatever scanning is going on and clears the queues */
	void Stop();

	/** Updates area parameters and scanner vectors in the controllers from the GuiParameters set without stopping scanning */
	void UpdateAreaParametersFromGui(const uint32_t& _area);

	/** Sets the current xyz stage position as zero */
	void SetStageZero();

	/** Sets the start stack position to the current z position */
	void StackStartHere();

	/** Sets the stop stack position to the current z position */
	void StackStopHere();

	/** Zeros galvo outputs.*/
	void ZeroGalvoOutputs();

	/** Opens/closes the shutter.*/
	void OpenCloseShutter(const uint32_t& _area, const bool& _open);

	/** Current state of the shutter.*/
	bool GetShutterState(const uint32_t& _area) const;

	/** Turns the resonance scanner relay on and off.*/
	void TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on);

	/** Current state of the resonance scanner relay.*/
	bool GetSwitchResonanceState(const uint32_t& _area) const;

	/** @name Attaching/Detaching channels and histograms
	* @{ */
	/** Attach a CChannelFrame as observer */
	void AttachFrame(gui::CChannelFrame* const cframe);
	/** Detach an observing CChannelFrame */
	void DetachFrame(gui::CChannelFrame* const cframe);
	/** Attach a CHistogramFrame as observer */
	void AttachFrame(gui::CHistogramFrame* const hframe);
	/** @return true if at least one histogram already attached to theDisplay for that area. */
	bool HistogramAlreadyAttached(const uint32_t& _area);
	/** Detach an observing CHistogramFrame */
	void DetachFrame(gui::CHistogramFrame* const hframe);
	/** @} */

	/** Saves current positions of windows by adding frames to WindowCollection of ScopeController::GuiParameters.*/
	void SaveCurrentWindowPositions();

	/** Sets the color limits for displaying imgages, calls DisplayController::SetHistogramLimits */
	void SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper);

	/** Updates the number of resonance planes in scope controller **/
	//void UpdateResonancePlanes(const uint32_t& _area);

	/** Saves the current values as a preset with name _name. Calls ScopeControllerImpl::SavePreset. */
	//void SavePreset(const uint32_t& _area, const CString& _name);

	/** Writes the parameters of the selected preset into daq and svframe. Calls ScopeControllerImpl::SelectPreset. */
	//void SelectPreset(const uint32_t& _area, const size_t& _n);

	/** Registers a function to call when scanmode is changed.
	* Usually CScanSettingsSheet registers for switching between FrameScan property pages.*/
	void RegisterScanmodeCallback(std::function<void(const uint32_t&, const ScannerVectorType&)> _callback);
};

}