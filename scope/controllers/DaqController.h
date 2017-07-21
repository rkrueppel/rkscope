#pragma once

#include "ScopeDefines.h"
#include "BaseController.h"
#include "helpers/SyncQueues.h"
#include "devices/Shutter.h"
#include "devices/SwitchResonance.h"
#include "parameters/Daq.h"
#include "helpers/ScopeException.h"
#include "devices/StimulationVector.h"
#include "scanmodes/ScannerVectorFrameBasic.h"
#include "ScopeDatatypes.h"
#include "helpers/DaqChunk.h"
#include "helpers/DaqChunkResonance.h"
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
* Uses the pimpl pattern to hide the implementation\n
* When working with National Instruments DAQmx library:\n
* See http://www.ni.com/white-paper/6411/en#toc5 for DAQmx thread-safety\n
* => "You can execute two different analog inputs from two different boards from separate threads within the same program,
* and they both run simultaneously without blocking" */
class DaqController
	: public BaseController {

protected:
	/** array holding the output queues to the PipelineControllers */
	std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>>* const output_queues;

	/** The outputs (pointers to base class) */
	std::vector<std::unique_ptr<Outputs>> outputs;

	/** The inputs (pointers to base class) */
	std::vector<std::unique_ptr<Inputs>> inputs;

	/** The stimulation output */
	std::unique_ptr<SCOPE_STIMULATIONS_T> stimulation;

	/** array holding shutter class for every area */
	std::vector<Shutter> shutters;

	/** array holding SwitchResonance class for every area */
	std::vector<SwitchResonance> switches;

	/** The scanner vector for frame scanning */
	std::vector<ScannerVectorFrameBasicPtr> scannervecs;

	/** stimulation */
	StimulationVector stimvec;

	/** size of a read chunk in samples per channel */
	std::vector<uint32_t> chunksizes;

	/** condition variables to wait for until online updates is done (new frame is completely written to buffer or aborted) */
	std::vector<std::condition_variable> online_update_done;

	/** bool flag to set after online update is done */
	std::vector<std::atomic<bool>> online_update_done_flag;

	/** mutexe for the condition variables */
	std::vector<std::mutex> online_update_done_mutexe;


public:
	/** Sets the output queues, generates initial ScannerVectors and initializes the shutters and the resonance scanner switches
	* @param[in] _oqueues output queues
	* @param[in] _parameters initial ScopeParameters set */
	DaqController(const uint32_t& _nactives, const parameters::Scope& _parameters, std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>>* const _oqueues);

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
	void Start(const parameters::Scope& _params) override;

	/** Handles update of parameters during scanning
	* @post online update is done or aborted */
	void OnlineParameterUpdate(const parameters::Area& _areaparameters);

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
	void OpenCloseShutter(const uint32_t& _area, const bool& _open);

	/** @return current shutter state */
	bool GetShutterState(const uint32_t& _area) const;

	/** Turns the resonance scanner relay on and off. */
	void TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on);

	/** @return current resonance scanner relay state */
	bool GetSwitchResonanceState(const uint32_t& _area) const;
};

}