#pragma once

#include "gui\MainDlgFrame.h"
#include "controllers/ScopeController.h"
#include "controllers/DaqController.h"
#include "controllers/PipelineController.h"
#include "controllers/DisplayController.h"
#include "controllers/StorageController.h"

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

			/** The ScopeController */
			ScopeController theController;
			
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
			
			/** The complete pseudo-global parameter set of the microscope. GUI classes can connect e.g. CScopeEditCtrl to specific parameters and thus pass values.
			* At the same time, the status of the controls can be controlled by the ScopeController (e.g. disable while scanning). */
			parameters::Scope guiparameters;
			
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
	};
}