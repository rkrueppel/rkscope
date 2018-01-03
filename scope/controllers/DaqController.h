#pragma once

#include "BaseController.h"
#include "config\config_choices.h"
#include "helpers/SyncQueues.h"
#include "devices/Shutter.h"
#include "devices/SwitchResonance.h"
#include "parameters/Daq.h"
#include "helpers/ScopeException.h"
#include "devices/StimulationVector.h"
#include "scanmodes/ScannerVectorFrameBasic.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/DaqChunks.h"
#include "devices/OutputsDAQmx.h"
#include "devices/OutputsDAQmxLineClock.h"
#include "devices/OutputsDAQmxResonance.h"
#include "devices/OutputsDAQmxSlave.h"
#include "devices/InputsDAQmx.h"
#include "devices/InputsFPGA.h"
#include "devices/StimulationsDAQmx.h"
#include "devices/GaterDAQmx.h"

namespace scope {

	/** @ingroup ScopeControl
	* The DaqController controls the data acquisition hardware, both outputs for scanners as well as input from PMTs.
	* When working with National Instruments DAQmx library:\n
	* See http://www.ni.com/white-paper/6411/en#toc5 for DAQmx thread-safety\n
	* => "You can execute two different analog inputs from two different boards from separate threads within the same program,
	* and they both run simultaneously without blocking" */
	class DaqController
		: public BaseController {

	protected:
		/** array holding the output queues to the PipelineControllers */
		std::array<SynchronizedQueue<ScopeMessage<config::DaqChunkPtrType>>, config::threads_daq>* const output_queues;

		/** The outputs for the master areas. */
		std::array<std::unique_ptr<config::OutputType>, config::nmasters> masteroutputs;

		/** The outputs for the slave areas. */
		std::array<std::unique_ptr<config::SlaveOutputType>, config::nslaves> slaveoutputs;

		/** The inputs for all master areas (their slaves are read with them together). */
		std::array<std::unique_ptr<config::InputType>, config::nmasters> inputs;

		/** The stimulation output */
		std::unique_ptr<config::StimulationsType> stimulation;

		/** array holding shutter class for master area */
		std::array<Shutter, config::nmasters> mastershutters;

		std::array<Shutter, config::nslaves> slaveshutters;

		/** array holding SwitchResonance class for every master area */
		std::array<SwitchResonance, config::nmasters> switches;

		/** The scanner vector for frame scanning. Pointer to base class, since current scan mode can be changed via GUI. */
		std::array<ScannerVectorFrameBasicPtr, config::totalareas> scannervecs;

		/** stimulation */
		StimulationVector stimvec;

		/** size of a read chunk in samples per channel */
		std::array<uint32_t, config::nmasters> chunksizes;

		/** condition variables to wait for until online updates is done (new frame is completely written to buffer or aborted) */
		std::array<std::condition_variable, config::nmasters> online_update_done;

		/** bool flag to set after online update is done */
		std::array<std::atomic<bool>, config::nmasters> online_update_done_flag;

		/** mutexe for the condition variables */
		std::array<std::mutex, config::nmasters> online_update_done_mutexe;
	
		parameters::Scope ctrlparams;

	public:
		/** Sets the output queues, generates initial ScannerVectors and initializes the shutters and the resonance scanner switches
		* @param[in] _oqueues output queues
		* @param[in] _parameters initial ScopeParameters set */
		DaqController(const uint32_t& _nactives, const parameters::Scope& _parameters, std::array<SynchronizedQueue<ScopeMessage<config::DaqChunkPtrType>>, config::threads_daq>* const _oqueues);

		/** disable copy */
		DaqController(DaqController& other) = delete;

		/** disable assignment */
		DaqController& operator=(DaqController& other) = delete;

		/** Stops and interrupts everything and zeros galvo outputs */
		~DaqController();

		/** Main function for running data acquisition. It is executed asynchronously.
		* For every area one Run function is executed (since DaqController is derived from BaseController). */
		ControllerReturnStatus Run(StopCondition* const sc, const uint32_t& _area) override;

		/** Starts the DaqController. Calculates ScannerVector and StimulationVector. Calculates various acquisition related rates. Creates tasks
		* sets timing and triggering. Writes ScannerVector and StimulationVector, opens the shutters, turns on the resonance scanner switches, starts all tasks,
		* starts all Run methods asynchronously and gets their futures.
		* @param[in] _params ScopeParameter set to work with */
		void Start(const parameters::Scope& _params);

		/** Handles update of parameters during scanning
		* @post online update is done or aborted */
		void OnlineParameterUpdate(const uint32_t& _masterarea, const parameters::MasterArea& _areaparameters);

		/** Does the actual writing to device for an online update */
		void WorkerOnlineParameterUpdate(const uint32_t _area);

		/** Aborts a running online parameters update (aborts the block-wise Outputs::Write operation)
		* @post the waiting OnlineParameterUpdate should return rather quickly now */
		void AbortOnlineParameterUpdate(const uint32_t& _area);

		/** Sets all galvos to zero position, needed for  microscope alignment. */
		void ZeroGalvoOutputs();

		/** Sets a scanner vector. Only called on startup. */
		void SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv);

		/** Opens/closes the shutter. */
		void OpenCloseMasterShutter(const uint32_t& _masterarea, const bool& _open);
		void OpenCloseSlaveShutter(const uint32_t& _slavearea, const bool& _open);

		/** @return current shutter state */
		bool GetMasterShutterState(const uint32_t& _masterarea) const;
		bool GetSlaveShutterState(const uint32_t& _slavearea) const;

		/** Turns the resonance scanner relay on and off. */
		void TurnOnOffSwitchResonance(const uint32_t& _masterarea, const bool& _on);

		/** @return current resonance scanner relay state */
		bool GetSwitchResonanceState(const uint32_t& _masterarea) const;
	};

}