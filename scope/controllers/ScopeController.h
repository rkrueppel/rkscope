#pragma once
#include "ScopeControllerModes.h"
#include "ScopeDefines.h"
#include "version.h"
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
#include "ScopeDatatypes.h"
#include "ScopeControllerButtons.h"
#include "BaseController.h"

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
	/** queues from the daqs to the pipelines */
	std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS> daq_to_pipeline;

	/** queue from the pipelines to the storage */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>> pipeline_to_storage;

	/** queue from the pipelines to the display */
	SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>> pipeline_to_display;

	/** Scanner vectors
	* @{ */
	/** The scanner vector for frame scanning */
	std::array<ScannerVectorFrameBasicPtr, SCOPE_NAREAS> framescannervecs;
	/** @} */

	/** @name Dataflow controllers
	* @{ */
	DaqController theDaq;
	PipelineController thePipeline;
	StorageController theStorage;
	DisplayController theDisplay;
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

	/** true after parameter set on program start is loaded */
	ScopeNumber<bool> initialparametersloaded;

	/** currently loaded config */
	std::wstring currentconfigfile;

	/** list of callbacks for scan mode switching */
	std::vector<std::function<void(const uint32_t&, const ScannerVectorType&)>> scanmodecallbacks;

	/*ScopeController(ScopeController& other);				// we need this copyable for std::bind to work...
	ScopeController& operator=(ScopeController& other); */

public:
	/** The complete pseudo-global parameter set of the microscope. GUI classes can connect e.g. CScopeEditCtrl to specific parameters and thus pass values
	* to the ScopeController. At the same time, the status of the controls can be controlled by the ScopeController (e.g. disable while scanning). */
	static parameters::Scope GuiParameters;

	/** Set to true while scanning, GUI elements can connect to this to disable buttons and controls (that are not matched
	* by static ScopeButtons etc here) while scanning. */
	static ScopeNumber<bool> ReadOnlyWhileScanning;

	/** @name Hardware controllers
	* @{ */
	FPUController theFPUs;
	SCOPE_XYZCONTROL_T theStage;
	/** @} */

	/** @name ScopeButtons
	* GUI classes can connect CScopeButtonCtrl to these ScopeButtons via their scope_controller member and thus pass commands to the ScopeController.
	* At the same time, the status of the WTL buttons can be controlled by the ScopeController (e.g. disable while scanning).
	* I.e. ScopeButtons are mediators between the controls (CScopeButtonCtrl) and ScopeController functions to start certain stuff;
	* if it weren't for the button state, CScopeButtonCtrl could directly connect to these functions...
	* @{ */
	static ScopeButton StartSingleButton;
	static ScopeButton StartLiveButton;
	static ScopeButton StartStackButton;
	static ScopeButton StartTimeseriesButton;
	static ScopeButton StartBehaviorButton;
	static ScopeButton StopButton;
	static ScopeButton QuitButton;
	static ScopeButton StackStartHereButton;
	static ScopeButton StackStopHereButton;

	/** Updated from PipelineControllerImpl::Run */
	static std::array<ScopeNumber<double>, SCOPE_NAREAS> SingleFrameProgress;

	/** Updated from ScopeControllerImpl::RunStack, connected to progress indicator in CStackSettingsPage */
	static ScopeNumber<double> PlaneCounter;

	/** Updated from PipelineControllerImpl::Run, connected to progress indicator in CTimeSeriesSettingsPage or edit control in CBehaviorSettingsPage */
	static std::array<ScopeNumber<double>, SCOPE_NAREAS> FrameCounter;

	/** Updated from ScopeControllerImpl::RunTimeseries, connected to progress indicator in CTimeSeriesSettingsPage */
	static ScopeNumber<double> RepeatCounter;

	/** Updated from ScopeControllerImpl::RunBehavior, connected to edit control in CBehaviorSettingsPage */
	static ScopeNumber<double> TrialCounter;

	/** Updated from e.g. ScopeControllerImpl::RunBehavior, connected to time indicator in CBehaviorSettingsPage */
	static ScopeNumber<double> TotalTime;

	/** Button to zero main stage coordinates */
	static ScopeButton StageZeroButton;

	/** Buttons for FPU nudge */
	static std::array<FPUButtons, SCOPE_NAREAS> FPU;

	/** Buttons for switching the scan mode */
	static std::array<ScanModeButtons, SCOPE_NAREAS> ScanMode;
	/** @} */


protected:
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

	/** Set parameters for scanner vectors, since these are shared_ptr in DaqControllerDAQmx and PipelineController they get updated there too */
	void SetScannerVectorParameters();

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
	/** Injects pimpl of ScopeController into BaseControllers pimpl (saves memory, since pointer is reused for the current pimpl) */
	ScopeController(void);

	~ScopeController(void);

	/** Print the current ScopeController version to debug console.
	* This should be called before creation of the GUI, when only the main thread exists. Then the static member in GetImpl() is initialized and we
	* do not have to worry about thread-safe singleton creation... */
	void Version();

	/** @return the currently loaded config file */
	std::wstring CurrentConfigFile() const;

	/** Called by CMainDialogFrame::QuitApplication.
	* Stops running threads etc, avoids destruction of these when the static pimpl gets destructed (probably very late or undefined).*/
	void PrepareQuit();

	/** Load a complete ScopeParameters set from disk. Calls ScopeControllerImpl::LoadParameters.*/
	bool LoadParameters(const std::wstring& filepath);

	/** Store the current complete ScopeParameters set to disk. Calls ScopeControllerImpl::SaveParameters (non-const since we update parameters from GUI first).*/
	bool SaveParameters(const std::wstring& filepath);

	/** Zeros galvo outputs. Calls ScopeControllerImpl::ZeroGalvoOutputs. */
	void ZeroGalvoOutputs();

	/** Opens/closes the shutter. Calls ScopeControllerImpl::OpenCloseShutter */
	void OpenCloseShutter(const uint32_t& _area, const bool& _open);

	/** Current state of the shutter. Calls ScopeControllerImpl::GetShutterState */
	bool GetShutterState(const uint32_t& _area) const;

	/** Turns the resonance scanner relay on and off. Calls ScopeControllerImpl::TurnOnOffSwitchResonance */
	void TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on);

	/** Current state of the resonance scanner relay. Calls ScopeControllerImpl::GetShutterState */
	bool GetSwitchResonanceState(const uint32_t& _area) const;

	/** @name Attaching/Detaching channels and histograms
	* @{ */
	void AttachFrame(gui::CChannelFrame* const cframe);
	void DetachFrame(gui::CChannelFrame* const cframe);
	void AttachFrame(gui::CHistogramFrame* const hframe);
	bool HistogramAlreadyAttached(const uint32_t& _area);
	void DetachFrame(gui::CHistogramFrame* const hframe);
	/** @} */

	/** Saves current positions of windows by adding frames to WindowCollection of ScopeController::GuiParameters.
	* Calls ScopeControllerImpl::SaveCurrentWindowPositions. */
	void SaveCurrentWindowPositions();

	/** Sets display limits for a certain area and channel. Calls ScopeControllerImpl::SetHistogramLimits.*/
	void SetHistogramLimits(const uint32_t& _area, const uint32_t& _channel, const uint16_t& _lower, const uint16_t& _upper);

	/** Updates the number of resonance planes in scope controller **/
	void UpdateResonancePlanes(const uint32_t& _area);

	/** Saves the current values as a preset with name _name. Calls ScopeControllerImpl::SavePreset. */
	void SavePreset(const uint32_t& _area, const CString& _name);

	/** Writes the parameters of the selected preset into daq and svframe. Calls ScopeControllerImpl::SelectPreset. */
	void SelectPreset(const uint32_t& _area, const size_t& _n);

	/** Registers a function to call when scanmode is changed.
	* Usually CScanSettingsSheet registers for switching between FrameScan property pages.
	* Calls ScopeControllerImpl::RegisterScanModeCallback */
	void RegisterScanmodeCallback(std::function<void(const uint32_t&, const ScannerVectorType&)> _callback);
};

}