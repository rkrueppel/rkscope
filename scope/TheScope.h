#pragma once

#include "gui\MainDlgFrame.h"
#include "helpers\SyncQueues.h"
#include "helpers\helpers.h"
#include "helpers\ScopeMultiImage.h"
#include "controllers/ScopeController.h"
#include "controllers/DaqController.h"
#include "controllers/PipelineController.h"
#include "controllers/DisplayController.h"
#include "controllers/StorageController.h"
#include "controllers\FPUController.h"
#include "devices/xyz/XYZControl.h"
#include "devices/xyz/XYZControlGalil.h"
#include "devices/xyz/XYZControlSutter.h"
#include "TheScopeButtons.h"
#include "TheScopeCounters.h"

namespace scope {
	
	/** @return true if area _a is a slave area */
	template<class C> bool ThisIsSlaveArea(const C& c, const typename C::iterator& i) {
		#ifdef SCOPE_NBEAM_SETUP
			return i != std::begin(c);
		#else
			return false;
		#endif
	}

	/** @return true if area _a is a slave area */
	bool ThisIsSlaveArea(const uint32_t& _a);

	/** @return the master area 0 (if nbeam setup) or the area _a */
	template<class C> typename C::iterator ThisAreaOrMasterArea(const C& c, const typename C::iterator& i) {
		return ThisIsSlaveArea(c, i)?std::begin(c):i;
	}

	/** @return the master area 0 (if nbeam setup) or the area _a */
	uint32_t ThisAreaOrMasterArea(const uint32_t& _a);
	
	class TheScope {
		private:
			/** static to make sure only there is only one instance */
			static std::atomic<bool> instanciated;
	
			const uint32_t nareas;
			
			std::unique_ptr<scope::gui::CMainDlgFrame> wndmain;
			
			/** The complete pseudo-global parameter set of the microscope. GUI classes can connect e.g. CScopeEditCtrl to specific parameters and thus pass values.
			* At the same time, the status of the controls can be controlled by the ScopeController (e.g. disable while scanning). */
			parameters::Scope guiparameters;
			
			/** @name ScopeButtons
			* GUI classes can connect CScopeButtonCtrl to these ScopeButtons via their scope_controller member and thus pass commands to the ScopeController.
			* At the same time, the status of the WTL buttons can be controlled by the ScopeController (e.g. disable while scanning).
			* I.e. ScopeButtons are mediators between the controls (CScopeButtonCtrl) and ScopeController functions to start certain stuff;
			* if it weren't for the button state, CScopeButtonCtrl could directly connect to these functions...
			* @{ */
			
			/** Buttons to start the different run modes (live, timeseries, ...) */
			RunButtons runbuttons;

			/** Buttons to set stack start and end */
			StackButtons stackbuttons;

			/** Buttons for zeroing stage and galvos */
			ZeroButtons zerobuttons;

			/** Buttons for FPU nudge */
			std::array<FPUButtons, SCOPE_NAREAS> fpubuttonsvec;

			/** Buttons for switching the scan mode */
			std::array<ScanModeButtons, SCOPE_NAREAS> scanmodebuttonsvec;
			/** @} */
			
			/** The counters and progresses */
			ScopeCounters<SCOPE_NAREAS> counters;
			
			/** queues from the daqs to the pipelines */
			std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQMULTICHUNKPTR_T>>, SCOPE_NBEAM_DAQS> daq_to_pipeline;

			/** queue from the pipelines to the storage */
			SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>> pipeline_to_storage;

			/** queue from the pipelines to the display */
			SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>> pipeline_to_display;
			
			/** @name Dataflow controllers
			* @{ */
			DaqController theDaq;
			PipelineController thePipeline;
			StorageController theStorage;
			DisplayController theDisplay;
			/** @} */
			
			/** @name Hardware controllers
			* @{ */
			FPUController theFPUs;
			SCOPE_XYZCONTROL_T theStage;
			/** @} */
			
			/** The ScopeController */
			ScopeController theController;
			
		public:
			TheScope(const uint32_t& _nareas, const std::wstring& _initialparameterpath);
			
			// Disable copy construction
			TheScope(const TheScope& _other) = delete;

			// Disable assignment
			TheScope operator=(const TheScope& _other) = delete;
		
			void CreateAndShowMainWindow();

			/** Print the current ScopeController version to debug console.
			* This should be called before creation of the GUI, when only the main thread exists. Then the static member in GetImpl() is initialized and we
			* do not have to worry about thread-safe singleton creation... */
			void Version() const;
			
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
			
			/** Sets the state of the GUI buttons for FPU control (via the corresponding ScopeButtons). True = enabled, false = disabled.  */
			void SetFPUButtonsState(const bool& state);

			/** Sets the state of GUI controls (via the corresponding ScopeNumbers). True = read&write/enabled, false = readonly/disabled. */
			void SetGuiCtrlState();

			/** Called via change of GuiParameters.areas[x].framesaw.scanvec.xres etc. */
			void ResolutionChange(const uint32_t& _area);

			/** Sets the type of scanning. Called ScanModeButtons.
			* Updates/recreates the scanvectors from a different type and sets them in theDaq and thePipeline */
			void SetScanMode(const uint32_t& _area, const ScannerVectorType& _mode);

			std::wstring GetStorageFolder() { return guiparameters.storage.folder(); }
			
			void PrepareQuit();

	};
}