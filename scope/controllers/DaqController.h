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
	* and they both run simultaneously without blocking"
	* @tparam NMASTERS Number of master areas
	* @tparam NSLAVES Number of slave areas
	* @tparam SLAVESPERMASTER How many slave areas per master area
	* @tparam MASTERSINALLAREAS Indexes of the master areas in a vector of all areas. E.g. allareas 0:master0, 1:slave0, 2:master1, 3:slave1 => mastersinallareas {{0,2}} */
	template<uint32_t NMASTERS, uint32_t NSLAVES, uint32_t SLAVESPERMASTER, const std::vector<uint32_t>* MASTERSINALLAREAS>
	class DaqController
		: public BaseController {

	protected:
		/** array holding the output queues to the PipelineControllers */
		std::vector<SynchronizedQueue<ScopeMessage<config::DaqChunkPtrType>>>* const output_queues;

		/** The outputs for the master areas. */
		std::vector<std::unique_ptr<Outputs>> outputs;

		/** The inputs for all master areas (their slaves are read with them together). */
		std::vector<std::unique_ptr<Inputs>> inputs;

		/** The stimulation output */
		std::unique_ptr<config::StimulationsType> stimulation;

		/** shutters */
		std::array<Shutter, NMASTERS+NSLAVES> shutters;

		/** vector holding SwitchResonance class */
		std::array<SwitchResonance, NMASTERS + NSLAVES> switches;

		/** The scanner vector for frame scanning. Pointer to base class, since current scan mode can be changed via GUI. */
		std::array<ScannerVectorFrameBasicPtr, NMASTERS + NSLAVES> scannervecs;

		/** stimulation */
		StimulationVector stimvec;

		/** size of a read chunk in samples per channel */
		std::vector<uint32_t> chunksizes;

		/** condition variables to wait for until online updates is done (new frame is completely written to buffer or aborted) */
		std::array<std::condition_variable, NMASTERS + NSLAVES> online_update_done;

		/** bool flag to set after online update is done */
		std::array<std::atomic<bool>, NMASTERS + NSLAVES> online_update_done_flag;

		/** mutexe for the condition variables */
		std::array<std::mutex, NMASTERS + NSLAVES> online_update_done_mutexe;
	
		parameters::Scope ctrlparams;

	public:
		/** Sets the output queues, generates initial ScannerVectors and initializes the shutters and the resonance scanner switches
		* @param[in] _oqueues output queues
		* @param[in] _parameters initial ScopeParameters set */
		DaqController(const parameters::Scope& _parameters, std::vector<SynchronizedQueue<ScopeMessage<config::DaqChunkPtrType>>>* const _oqueues)
			: BaseController(NMASTERS)
			, ctrlparams(_parameters)
			, output_queues(_oqueues)
			, outputs(0)
			, inputs(0)
			, stimulation(nullptr)
			, chunksizes(NMASTERS + NSLAVES, 16000)
		{
			online_update_done_flag.fill(false);

			for (uint32_t a = 0; a < shutters.size(); a++) {
				shutters[a].Initialize(ctrlparams.allareas[a]->daq.shutterline());
				switches[a].Initialize(ctrlparams.allareas[a]->daq.switchresonanceline());
			}
			ZeroGalvoOutputs();
		}


		/** disable copy */
		DaqController(DaqController& other) = delete;

		/** disable assignment */
		DaqController& operator=(DaqController& other) = delete;

		/** Stops and interrupts everything and zeros galvo outputs */
		~DaqController() {
			StopAll();
			WaitForAll(-1);
			ZeroGalvoOutputs();
			DBOUT(L"DaqController::~DaqControllerImpl");
		}

		/** Main function for running data acquisition. It is executed asynchronously.
		* For every area one Run function is executed (since DaqController is derived from BaseController). */
		ControllerReturnStatus Run(StopCondition* const sc, const uint32_t& _area) override {
			DBOUT(L"DaqController::Run masterarea " << _area << L" beginning");
			ControllerReturnStatus returnstatus(ControllerReturnStatus::none);

			const DaqMode requested_mode = ctrlparams.requested_mode();
			uint32_t chunksize = inputs[_area]->StandardChunkSize();					// pixels per channel per chunk
			const uint32_t requested_samples = inputs[_area]->RequestedSamples();		// total pixels per channel
			uint32_t readsamples = 0;
			int32_t currentlyread = 0;
			bool timedout = false;

			// Main acquisition loop
			while (!sc->IsSet()) {
				// adjust size of last chunk to read if in nframes mode
				if (requested_mode == DaqModeHelper::nframes) {
					if ((readsamples + chunksize) > requested_samples)
						chunksize = requested_samples - readsamples;
				}

				// Create a new chunk...
				auto chunk = std::make_shared<config::DaqChunkType>(chunksize);

				// With this loop we can interrupt a thread that is waiting here for a trigger (see FiberMRI program)
				// or which waits for samples (which never come because of an error)
				currentlyread = 0;
				do {
					// Read data from inputs into the chunk. Timeout 1 second.
					currentlyread = inputs[_area]->Read(*chunk, timedout, 1);
					DBOUT(L"DaqController::Run read");
					// In case of timeout attempt to read as long as stop condition is not set
				} while (timedout && !sc->IsSet());
				timedout = false;

				ScopeMessage<config::DaqChunkPtrType> msg;
				msg.tag = ScopeMessageTag::nothing;
				msg.cargo = chunk;

				// ...and put it in the queue
				output_queues->at(_area).Enqueue(msg);

				// advance number of read pixels
				readsamples += currentlyread;
				DBOUT(L"DaqController::Run masterarea " << _area << L" read " << readsamples << L" of requested " << requested_samples);

				// Check if we read enough samples (if not live scanning)
				if ((requested_mode == DaqModeHelper::nframes) && (readsamples == requested_samples)) {
					// if yes we want to stop
					sc->Set(true);
					returnstatus = ControllerReturnStatus::finished;
					DBOUT(L"DaqController::Run masterarea " << _area << L" - all requested pixels read\n");
				}
			}

			// Force abort of online update
			outputs[_area]->AbortWrite();
			for (uint32_t sa = 0; sa < SLAVESPERMASTER; sa++)
				outputs[_area + 1 + sa]->AbortWrite();

			outputs[_area]->Stop();
			for (uint32_t sa = 0; sa < SLAVESPERMASTER; sa++)
				outputs[_area + 1 + sa]->Stop();

			inputs[_area]->Stop();

			// Close the shutters
			for (auto& s : shutters)
				s.Close();

			// Turn the resonance scanner relay off
			for (auto& s : switches)
				s.TurnOff();

			ATLTRACE(L"DaqController::Run end\n");
			return returnstatus;
		}

		/** Starts the DaqController. Calculates ScannerVector and StimulationVector. Calculates various acquisition related rates. Creates tasks
		* sets timing and triggering. Writes ScannerVector and StimulationVector, opens the shutters, turns on the resonance scanner switches, starts all tasks,
		* starts all Run methods asynchronously and gets their futures.
		* @param[in] _params ScopeParameter set to work with */
		void Start(const parameters::Scope& _params) {
			DBOUT(L"DaqController:::Start");

			ctrlparams = _params;

			// Reset outputs and inputs (configures tasks etc. inside them)
			outputs.clear();
			inputs.clear();
			uint32_t a = 0;
			for (uint32_t ma = 0; ma < NMASTERS; ma++) {
				outputs.push_back(std::make_unique<config::OutputType>(a, *dynamic_cast<config::OutputParametersType*>(ctrlparams.allareas[a]->daq.outputs.get()), ctrlparams));
				inputs.push_back(std::make_unique<config::InputType>(a, dynamic_cast<config::InputParametersType*>(ctrlparams.allareas[a]->daq.inputs.get()), ctrlparams));
				a++;
				for (uint32_t sa = 0; sa < SLAVESPERMASTER; sa++) {
					outputs.push_back(std::make_unique<config::SlaveOutputType>(a, *dynamic_cast<config::SlaveOutputParametersType*>(ctrlparams.allareas[a]->daq.outputs.get()), ctrlparams));
					a++;
				}
			}

			// Calculate and write stimulationvector to device
			if (ctrlparams.stimulation.enable()) {
				stimulation = std::make_unique<config::StimulationsType>(ctrlparams);
				stimvec.SetParameters(ctrlparams.stimulation);
				stimulation->Write(stimvec.GetVector());
				stimulation->Start();
			}
			else
				stimulation.reset(nullptr);

			// Write scannervectors to devices
			for (uint32_t ma = 0; ma < NMASTERS; ma++) {
				outputs[MASTERSINALLAREAS->at(ma)]->Write(*scannervecs[MASTERSINALLAREAS->at(ma)]->GetInterleavedVector(), 1);
				for (uint32_t sa = 0; sa < SLAVESPERMASTER; sa++) {
					outputs[MASTERSINALLAREAS->at(ma) + 1 + sa]->Write(*scannervecs[MASTERSINALLAREAS->at(ma) + 1 + sa]->GetInterleavedVector(), 1);
				}
			}


			// Open shutters
			for (auto& s : shutters)
				s.Open();

			// Turn the resonance scanner relay on
			if (config::scannerselect == config::ScannerEnum::ResonantGalvo)
				for (auto& s : switches)
					s.TurnOn();

			// Wait until the synchronization signal is in the steady state
			//		std::this_thread::sleep_for(std::chrono::milliseconds(600));
			//		std::this_thread::sleep_for(std::chrono::milliseconds(1500));

			// Define lambdas for starting all inputs and all outputs
			// always area 0 last, so it (e.g. its /ao/StartTrigger) can serve as common master trigger for everything else
			std::function<void(void)> start_inputs = [&]() {
				for (uint32_t a = 1; a < inputs.size(); a++)
					inputs[a]->Start();
				inputs[0]->Start();
			};
			std::function<void(void)> start_outputs = [&]() {
				for (uint32_t a = 1; a < outputs.size(); a++)
					outputs[a]->Start();
				outputs[0]->Start();
			};


			// start inputs or outputs first, depends on the triggering configuration you chose for the NI cards
			if (ctrlparams.startinputsfirst()) {
				start_inputs();
				start_outputs();
			}
			else {
				start_outputs();
				start_inputs();
			}

			// Let "Run" run asynchronously, one thread for each master!
			for (uint32_t ma = 0; ma < NMASTERS; ma++) {
				// Reset the stop condition
				stops[ma].Set(false);
				// Get the async's future
				futures[ma] = std::async(std::bind(&DaqController::Run, this, &stops[ma], ma));
			}
		}

		/** Handles update of parameters during scanning
		* @post online update is done or aborted */
		void OnlineParameterUpdate(const uint32_t& _area, const parameters::BaseArea& _areaparameters) {

			// update parameters
			*ctrlparams.allareas[_area].get() = _areaparameters;
			// Note: scannervector was updated already from ScopeController

			// Lock now so starting an async Worker is only possible if a putative previous wait on that lock finished
			std::unique_lock<std::mutex> lock(online_update_done_mutexe[_area]);

			// If we are scanning live do async online update. Always catch the async future since the futures destructor waits (see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3451.pdf)
			if (ctrlparams.requested_mode() == DaqModeHelper::continuous)
				auto f = std::async(std::bind(&DaqController::WorkerOnlineParameterUpdate, this, _area));

			// wait until online update is done or aborted in async WorkerOnlineParameterUpdate
			while (!online_update_done_flag[_area])
				online_update_done[_area].wait(lock);
		}

		/** Does the actual writing to device for an online update */
		void WorkerOnlineParameterUpdate(const uint32_t _area) {
			DBOUT(L"WorkerOnlineParameterUpdate starting");
			auto scannervec = scannervecs[_area]->GetInterleavedVector();

			// Number of blocks => around 4 blocks per second frame time
			uint32_t blocks = round2ui32(4.0 * ctrlparams.allareas[_area]->FrameTime());

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

		/** Sets all galvos to zero position, needed for  microscope alignment. */
		void ZeroGalvoOutputs() {
			// Cancel all currently configured tasks
			for (auto& o : outputs) {
				if (o.get() != nullptr)
					o.reset(nullptr);
			}
			for (auto& i : inputs) {
				if (i.get() != nullptr)
					i.reset(nullptr);
			}
			stimulation.reset(nullptr);

			// Do the zero outputs tasks
			for (uint32_t a = 0; a < ctrlparams.allareas.size(); a++) {
				if (ctrlparams.allareas[a]->areatype() == AreaTypeHelper::Master)
					config::OutputZeroType zero(*dynamic_cast<config::OutputParametersType*>(ctrlparams.allareas[a].get()->daq.outputs.get()));
				else
					config::SlaveOutputZeroType zero(*dynamic_cast<config::SlaveOutputParametersType*>(ctrlparams.allareas[a]->daq.outputs.get()));
			}
		}

		/** Sets a scanner vector. Only called on startup. */
		void SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv) {
			scannervecs[_area] = _sv;
		}

		/** Opens/closes the shutter. */
		void OpenCloseShutter(const uint32_t& _area, const bool& _open) {
			shutters[_area].Set(_open);
		}

		/** @return current shutter state */
		bool GetShutterState(const uint32_t& _area) const {
			return shutters[_area].GetState();
		}

		/** Turns the resonance scanner relay on and off. */
		void TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
			for (auto& s : switches)
				s.Set(_on);
		}

		/** @return current resonance scanner relay state */
		bool GetSwitchResonanceState(const uint32_t& _area) const {
			return switches[_area].GetState();
		}
	};

}