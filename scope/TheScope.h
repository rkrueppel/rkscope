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
#include "TheScopeButtons.h"

namespace scope {
	
	class TheScope {
		private:
			/** static to make sure only there is only one instance */
			static std::atomic<bool> instanciated;
	
			const uint32_t nareas;
			
			std::unique_ptr<scope::gui::CMainDlgFrame> wndmain;
			
			/** queues from the daqs to the pipelines */
			std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>> daq_to_pipeline;

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
			
			/** The complete pseudo-global parameter set of the microscope. GUI classes can connect e.g. CScopeEditCtrl to specific parameters and thus pass values.
			* At the same time, the status of the controls can be controlled by the ScopeController (e.g. disable while scanning). */
			parameters::Scope guiparameters;
			
			/** @name ScopeButtons
			* GUI classes can connect CScopeButtonCtrl to these ScopeButtons via their scope_controller member and thus pass commands to the ScopeController.
			* At the same time, the status of the WTL buttons can be controlled by the ScopeController (e.g. disable while scanning).
			* I.e. ScopeButtons are mediators between the controls (CScopeButtonCtrl) and ScopeController functions to start certain stuff;
			* if it weren't for the button state, CScopeButtonCtrl could directly connect to these functions...
			* @{ */
			
			RunButtons runbuttons;

			StackButtons stackbuttons;

			MiscButtons miscbuttons;

			/** Buttons for FPU nudge */
			std::vector<FPUButtons> fpubuttonsvec;

			/** Buttons for switching the scan mode */
			std::vector<ScanModeButtons> scanmodebuttonsvec;
			/** @} */
			
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
			
			void PrepareQuit();

	};
}