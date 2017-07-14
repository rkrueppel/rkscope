#pragma once

#include "DaqController.h"
#include "BaseController_p.h"
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
#include "ScopeLogger.h"
#include "devices/OutputsDAQmx.h"
#include "devices/OutputsDAQmxLineClock.h"
#include "devices/OutputsDAQmxResonance.h"
#include "devices/OutputsDAQmxSlave.h"
#include "devices/InputsDAQmx.h"
#include "devices/InputsFPGA.h"
#include "devices/StimulationsDAQmx.h"
#include "devices/GaterDAQmx.h"

namespace scope {

/** The implementation class of the DaqController. Every area runs in one thread. */
class DaqController::Impl 
	: public BaseController<SCOPE_NAREAS>::Impl {

protected:
	/** array holding the output queues to the PipelineControllers */
	std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS>* const output_queues;

	/** The outputs (pointers to base class) */
	std::array<std::unique_ptr<Outputs>, SCOPE_NAREAS> outputs;

	/** The inputs (pointers to base class) */
	std::array<std::unique_ptr<Inputs>, SCOPE_NAREAS> inputs;

	/** The stimulation output */
	std::unique_ptr<SCOPE_STIMULATIONS_T> stimulation;

	/** array holding shutter class for every area */
	std::array<Shutter, SCOPE_NAREAS> shutters;

	/** array holding SwitchResonance class for every area */
	std::array<SwitchResonance, SCOPE_NAREAS> switches;

	/** The scanner vector for frame scanning */
	std::array<ScannerVectorFrameBasicPtr, SCOPE_NAREAS> scannervecs;

	/** stimulation */
	StimulationVector stimvec;

	/** size of a read chunk in samples per channel */
	std::array<uint32_t, SCOPE_NAREAS> chunksizes;

	/** condition variables to wait for until online updates is done (new frame is completely written to buffer or aborted) */
	std::array<std::condition_variable, SCOPE_NAREAS> online_update_done;

	/** bool flag to set after online update is done */
	std::array<std::atomic<bool>, SCOPE_NAREAS> online_update_done_flag;
	
	/** mutexe for the condition variables */
	std::array<std::mutex, SCOPE_NAREAS> online_update_done_mutexe;

	/** a ScopeLogger kept handy here */
	ScopeLogger scope_logger;

protected:
	/** disable copy */
	Impl(const Impl& i);
	
	/** disable assignment */
	Impl operator=(const Impl& i);

	/** Main function for running data acquisition. It is executed asynchronously.
	* For every area one Run function is executed (since DaqController is derived from BaseController<SCOPE_NAREAS>). */
	ControllerReturnStatus Run(StopCondition* const sc, const uint32_t& _area) override {
		DBOUT(L"DaqController::Impl::Run area " << _area << L" beginning");
		ControllerReturnStatus returnstatus(ControllerReturnStatus::none);

		const DaqMode requested_mode = parameters.requested_mode();
		uint32_t chunksize = inputs[_area]->StandardChunkSize();
		const uint32_t requested_samples = inputs[_area]->RequestedSamples();
		uint32_t readsamples = 0;
		int32_t currentlyread = 0;
		bool timedout = false;

		// Main acquisition loop
		while ( !sc->IsSet() ) {
			// adjust size of last chunk to read if in nframes mode
			if ( requested_mode == DaqModeHelper::nframes ) {
				if ( (readsamples + chunksize) > requested_samples )
					chunksize = requested_samples - readsamples;
			}

			// Create a new chunk...
			auto chunk = std::make_shared<SCOPE_DAQCHUNK_T>(chunksize, parameters.areas[_area]->daq.inputs->channels(), _area);
			
			// With this loop we can interrupt a thread that is waiting here for a trigger (see FiberMRI program)
			// or which waits for samples (which never come because of an error)
			currentlyread = 0;
			do {
				// Read data from inputs into the chunk. Timeout 1 second.
				currentlyread = inputs[_area]->Read(*chunk, timedout, 1);
				DBOUT(L"DaqController::Impl::Run read");
				// In case of timeout attempt to read as long as stop condition is not set
			} while ( timedout && !sc->IsSet() );
			timedout = false;

			ScopeMessage<SCOPE_DAQCHUNKPTR_T> msg;
			msg.tag = ScopeMessageTag::nothing;
			msg.cargo = chunk;

			// ...and put it in the queue
			output_queues->at(_area).Enqueue(msg);

			// advance number of read pixels
			readsamples += currentlyread;
			DBOUT(L"DaqController::Impl::Run area " << _area << L" read " << readsamples << L" of requested " << requested_samples);

			// Check if we read enough samples (if not live scanning)
			if ( (requested_mode == DaqModeHelper::nframes) && (readsamples == requested_samples) ) {
				// if yes we want to stop
				sc->Set(true);
				returnstatus = ControllerReturnStatus::finished;
				DBOUT(L"DaqController::Impl::Run area " << _area << L" - all requested pixels read\n");
			}
		}

		// Force abort of online update
		outputs[_area]->AbortWrite();

		outputs[_area]->Stop();
		inputs[_area]->Stop();

		// Close the shutters
		for ( auto& s : shutters )
			s.Close();

		// Turn the resonance scanner relay off
		for ( auto& s : switches )
			s.TurnOff();

		ATLTRACE(L"DaqController::DaqControllerImpl::Run end\n");
		return returnstatus;
	}

public:
	/** Sets the output queues, generates initial ScannerVectors and initializes the shutters and the resonance scanner switches
	* @param[in] _oqueues output queues
	* @param[in] _parameters initial ScopeParameters set */
	explicit Impl(std::array<SynchronizedQueue<ScopeMessage<SCOPE_DAQCHUNKPTR_T>>, SCOPE_NAREAS>* const _oqueues, const parameters::Scope& _parameters)
		:  BaseController<SCOPE_NAREAS>::Impl(_parameters)
		, output_queues(_oqueues)
		, stimulation(nullptr) {
		std::fill(std::begin(chunksizes), std::end(chunksizes), 16000);
		std::fill(std::begin(online_update_done_flag), std::end(online_update_done_flag), false);
		uint32_t a = 0;
		for ( auto& sh : shutters )
			sh.Initialize(parameters.areas[a++]->daq.shutterline());
		a = 0;
		for ( auto& sw : switches )
			sw.Initialize(parameters.areas[a++]->daq.switchresonanceline());

		ZeroGalvoOutputs();
	}

	/** Stops and interrupts everything and zeros galvo outputs */
	virtual ~Impl() {
		StopAll();
		WaitForAll(-1);
		ZeroGalvoOutputs();
		DBOUT(L"DaqController::Impl::~DaqControllerImpl");
	}

	/** Starts the DaqController. Calculates ScannerVector and StimulationVector. Calculates various acquisition related rates. Creates tasks
	* sets timing and triggering. Writes ScannerVector and StimulationVector, opens the shutters, turns on the resonance scanner switches, starts all tasks,
	* starts all Run methods asynchronously and gets their futures.
	* @param[in] _params ScopeParameter set to work with */
	void Start(const parameters::Scope& _params) override {
		DBOUT(L"DaqController::Impl::Start");

		parameters = _params;
		// Reset outputs and inputs (configures tasks etc. inside them)
		for ( uint32_t a = 0; a < SCOPE_NAREAS ; a++ ) {
			// Choose output type depending on that area being a slave area
			if ( parameters.areas[a]->isslave() )
				outputs[a].reset(new SCOPE_SLAVEOUTPUTS_T(a, *dynamic_cast<parameters::SCOPE_SLAVEOUTPUTS_T*>(parameters.areas[a]->daq.outputs.get()), parameters));
			else
				outputs[a].reset(new SCOPE_OUTPUTS_T(a, *dynamic_cast<parameters::SCOPE_OUTPUTS_T*>(parameters.areas[a]->daq.outputs.get()), parameters));
			ScopeController scope_controller;
			inputs[a].reset(new SCOPE_INPUTS_T(a, dynamic_cast<parameters::SCOPE_INPUTS_T*>(scope_controller.GuiParameters.areas[a]->daq.inputs.get()), parameters));
		}

		// Calculate and write stimulationvector to device
		if ( parameters.stimulation.enable() ) {
			stimulation.reset(new SCOPE_STIMULATIONS_T(parameters));
			stimvec.SetParameters(parameters.stimulation);
			stimulation->Write(stimvec.GetVector());
			stimulation->Start();
		}
		else
			stimulation.reset(nullptr);

		// Write scannervectors to devices
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
			outputs[a]->Write(*scannervecs[a]->GetInterleavedVector(), 1);
		
		// Open shutters
		for ( auto& s : shutters )
			s.Open();

		// Turn the resonance scanner relay on
		for ( auto& s : switches )
			s.TurnOn();
		// Wait until the synchronization signal is in the steady state
//		std::this_thread::sleep_for(std::chrono::milliseconds(600));
//		std::this_thread::sleep_for(std::chrono::milliseconds(1500));

		// Define lambdas for starting all inputs and all outputs
		// always area 0 last, so it (e.g. its /ao/StartTrigger) can serve as common master trigger for everything else
		std::function<void(void)> start_inputs = [&](){
			for ( uint32_t a = 1 ; a < SCOPE_NAREAS ; a++ )
				inputs[a]->Start();
			inputs[0]->Start();
		};
		std::function<void(void)> start_outputs = [&](){
			for ( uint32_t a = 1 ; a < SCOPE_NAREAS ; a++ )
				outputs[a]->Start();
			outputs[0]->Start();
		};

		// start inputs or outputs first
		if ( parameters.startinputsfirst() ) {
			start_inputs();
			start_outputs();
		} else {
			start_outputs();
			start_inputs();
		}

		// Let "Run" run asynchronously
		for( uint32_t a = 0; a < SCOPE_NAREAS ; a++ ) {
			// Reset the stop condition
			stops[a].Set(false);
			// Get the async's future
			futures[a] = std::async(std::bind(&Impl::Run, this, &stops[a], a));
		}
	}

	/** Handles update of parameters during scanning
	* @post online update is done or aborted */
	void OnlineParameterUpdate(const parameters::Area& _areaparameters) {
		uint32_t area = _areaparameters.area();

		// update parameters
		*parameters.areas[area] = _areaparameters;
		// Note: scannervector was updated already from ScopeControllerImpl

		// Lock now so starting an async Worker is only possible if a putative previous wait on that lock finished
		std::unique_lock<std::mutex> lock(online_update_done_mutexe[area]);

		// If we are scanning live do async online update. Always catch the async future since the futures destructor waits (see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3451.pdf)
		if ( parameters.requested_mode() == DaqModeHelper::continuous )
			auto f = std::async(std::bind(&Impl::WorkerOnlineParameterUpdate, this, area));

		// wait until online update is done or aborted in async WorkerOnlineParameterUpdate
		while ( !online_update_done_flag[area] )
			online_update_done[area].wait(lock);
	}

	/** Does the actual writing to device for an online update */
	void WorkerOnlineParameterUpdate(const uint32_t _area) {
		DBOUT(L"WorkerOnlineParameterUpdate starting");
		auto scannervec = scannervecs[_area]->GetInterleavedVector();

		// Number of blocks => around 4 blocks per second frame time
		uint32_t blocks = round2ui32(4.0 * parameters.areas[_area]->FrameTime());
		
		DBOUT(L"WorkerOnlineParameterUpdate blocks" << blocks);
		online_update_done_flag[_area] = false;

		// If we call outputs[_area]->AbortWrite this Write exits prematurely
		outputs[_area]->Write(*scannervec, blocks);

		// when update is done, signal the waiting condition_variable in OnlineParameterUpdate
		online_update_done_flag[_area] = true;
		online_update_done[_area].notify_all();
		DBOUT(L"WorkerOnlineParameterUpdate ended");
	}

	/** Aborts a running online parameters update (aborts the block-wise Outputs::Write operation)
	* @post the waiting OnlineParameterUpdate should return rather quickly now */
	void AbortOnlineParameterUpdate(const uint32_t& _area) {
		// Abort the output on its next block write. Note, the Write is inside lock
		outputs[_area]->AbortWrite();
	}

	/** Zeros galvo outputs */
	void ZeroGalvoOutputs() {
		// Cancel all currently configured tasks
		for ( auto& o : outputs ) {
			if ( o.get() != nullptr )
				o.reset(nullptr);
		}
		for ( auto& i : inputs ) {
			if ( i.get() != nullptr )
				i.reset(nullptr);
		}
		stimulation.reset(nullptr);

		// Do the zero outputs tasks
		for ( const auto& ap : parameters.areas ) {
			if ( ap->isslave() )
				SCOPE_ZEROOUTPUTSSLAVE_T zero(*dynamic_cast<parameters::SCOPE_SLAVEOUTPUTS_T*>(ap->daq.outputs.get()));
			else
				SCOPE_ZEROOUTPUTS_T zero(*dynamic_cast<parameters::SCOPE_OUTPUTS_T*>(ap->daq.outputs.get()));
		}
	}

	/** Sets a scanner vector. Only called on startup. */
	void SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv) {
		scannervecs[_area] = _sv;
	}

	/** Opens/closes the shutter. Calls DaqController::Impl::OpenCloseShutter */
	void OpenCloseShutter(const uint32_t& _area, const bool& _open) {
		for ( auto& s : shutters )
			s.Set(_open);
	}

	/** @return current state of the first shutter */
	bool GetShutterState(const uint32_t _area) const {
		return shutters[_area].GetState();
	}

	/** Turns the resonance scanner relay on and off. */
	void TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
		for ( auto& s : switches )
			s.Set(_on);
	}

	/** @return current state of the first resonance scanner relay */
	bool GetSwitchResonanceState(const uint32_t _area) const {
		return switches[_area].GetState();
	}
};



}