#pragma once
#include "ScopeDefines.h"
#include "version.h"
#include "BaseController.h"
#include "ScopeControllerModes.h"
#include "ScopeControllerCounters.h"
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
#include "ScopeLogger.h"
#include "devices/xyz/XYZControl.h"
#include "devices/xyz/XYZControlGalil.h"
#include "devices/xyz/XYZControlSutter.h"
#include "devices/GaterDAQmx.h"
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

		private:
			const uint32_t nareas;
			
			/** Reference to TheScope's gui parameters */
			parameters::Scope &guiparameters;
			
			/** The ScopeControllers own parameter set, gets overwritten on Start by guiparameters while some values are adjusted/calculated for the actual Run */
			parameters::Scope ctrlparameters;
			
			/** @name References to the dataflow controllers
			* @{ */
			DaqController& theDaq;
			PipelineController& thePipeline;
			StorageController& theStorage;
			DisplayController& theDisplay;
			/** @} */
			
			/** @name References to the queues between the dataflow controller 
			* @{ */
			std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>>& daq_to_pipeline;
			SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>& pipeline_to_storage;
			SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>& pipeline_to_display;
			/** @} */
			
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

			/*ScopeController(ScopeController& other);				// we need this copyable for std::bind to work...
			ScopeController& operator=(ScopeController& other); */

			/** The counters */
			ScopeControllerCounters scopecontrollercounters;

		private:
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
			ScopeController(const uint32_t& _nareas
				, parameters::Scope& _guiparameters
				, DaqController& _theDaq
				, PipelineController& _thePipeline
				, StorageController& _theStorage
				, DisplayController& _theDisplay
				, std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>>& _daq_to_pipeline
				, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>& _pipeline_to_storage
				, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>& _pipeline_to_display
			);

			/** Stop whatever is running */
			~ScopeController(void);
			
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
			void OnlineUpdate(const uint32_t& _area);


			/** Updates the number of resonance planes in scope controller **/
			//void UpdateResonancePlanes(const uint32_t& _area);

			/** Saves the current values as a preset with name _name. Calls ScopeControllerImpl::SavePreset. */
			//void SavePreset(const uint32_t& _area, const CString& _name);

			/** Writes the parameters of the selected preset into daq and svframe. Calls ScopeControllerImpl::SelectPreset. */
			//void SelectPreset(const uint32_t& _area, const size_t& _n);
	};

}