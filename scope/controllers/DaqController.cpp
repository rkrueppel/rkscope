#include "stdafx.h"
#include "DaqController.h"

namespace scope {

	DaqController::DaqController(const uint32_t& _nactives
		, const parameters::Scope& _parameters
		, std::array<SynchronizedQueue<ScopeMessage<config::DaqChunkPtrType>>, config::threads_daq>* const _oqueues
	)
		: BaseController(_nactives)
		, ctrlparams(_parameters)
		, output_queues(_oqueues)
		, masteroutputs()
		, slaveoutputs()
		, inputs()
		, stimulation(nullptr)
	{
		chunksizes.fill(16000);
		uint32_t a = 0;
		for (uint32_t a = 0; a < mastershutters.size(); a++) {
			mastershutters[a].Initialize(ctrlparams.masterareas[a].daq.shutterline());
			switches[a].Initialize(ctrlparams.masterareas[a].daq.switchresonanceline());
			online_update_done_flag[a] = false;
		}

		for ( uint32_t a = 0 ; a < slaveshutters.size() ; a++)
			slaveshutters[a].Initialize(ctrlparams.slaveareas[a].daq.shutterline());

		ZeroGalvoOutputs();
	}

	DaqController::~DaqController() {
		StopAll();
		WaitForAll(-1);
		ZeroGalvoOutputs();
		DBOUT(L"DaqController::~DaqControllerImpl");
	}


	ControllerReturnStatus DaqController::Run(StopCondition* const sc, const uint32_t& _masterarea) {
		DBOUT(L"DaqController::Run masterarea " << _masterarea << L" beginning");
		ControllerReturnStatus returnstatus(ControllerReturnStatus::none);

		const DaqMode requested_mode = ctrlparams.requested_mode();
		uint32_t chunksize = inputs[_masterarea]->StandardChunkSize();					// pixels per channel per chunk
		const uint32_t requested_samples = inputs[_masterarea]->RequestedSamples();		// total pixels per channel
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
				currentlyread = inputs[_masterarea]->Read(*chunk, timedout, 1);
				DBOUT(L"DaqController::Run read");
				// In case of timeout attempt to read as long as stop condition is not set
			} while (timedout && !sc->IsSet());
			timedout = false;

			ScopeMessage<config::DaqChunkPtrType> msg;
			msg.tag = ScopeMessageTag::nothing;
			msg.cargo = chunk;

			// ...and put it in the queue
			output_queues->at(_masterarea).Enqueue(msg);

			// advance number of read pixels
			readsamples += currentlyread;
			DBOUT(L"DaqController::Run masterarea " << _masterarea << L" read " << readsamples << L" of requested " << requested_samples);

			// Check if we read enough samples (if not live scanning)
			if ((requested_mode == DaqModeHelper::nframes) && (readsamples == requested_samples)) {
				// if yes we want to stop
				sc->Set(true);
				returnstatus = ControllerReturnStatus::finished;
				DBOUT(L"DaqController::Run masterarea " << _masterarea << L" - all requested pixels read\n");
			}
		}

		// Force abort of online update
		masteroutputs[_masterarea]->AbortWrite();
		for (uint32_t s = _masterarea * config::slavespermaster; s < (_masterarea + 1)*config::slavespermaster; s++)
			slaveoutputs[s]->AbortWrite();

		masteroutputs[_masterarea]->Stop();
		for (uint32_t s = _masterarea * config::slavespermaster; s < (_masterarea + 1)*config::slavespermaster; s++)
			slaveoutputs[s]->Stop();

		inputs[_masterarea]->Stop();

		// Close the shutters
		for (auto& s : mastershutters)
			s.Close();
		for (auto& s : slaveshutters)
			s.Close();

		// Turn the resonance scanner relay off
		for (auto& s : switches)
			s.TurnOff();

		ATLTRACE(L"DaqController::Run end\n");
		return returnstatus;
	}

	void DaqController::Start(const parameters::Scope& _params) {
		DBOUT(L"DaqController:::Start");

		ctrlparams = _params;

		// Reset outputs and inputs (configures tasks etc. inside them)
		for ( uint32_t a = 0 ; a < config::nmasters ; a++ ) {
			masteroutputs[a] = std::make_unique<config::OutputType>(a, *dynamic_cast<config::OutputParametersType*>(ctrlparams.masterareas[a].daq.outputs.get()), ctrlparams);
			inputs[a] = std::make_unique<config::InputType>(a, dynamic_cast<config::InputParametersType*>(ctrlparams.masterareas[a].daq.inputs.get()), ctrlparams);
		}
		for ( uint32_t a = 0 ; a < config::nslaves ; a++ ) 
			slaveoutputs[a] = std::make_unique<config::SlaveOutputType>(a, *dynamic_cast<config::SlaveOutputParametersType*>(ctrlparams.masterareas[a].daq.outputs.get()), ctrlparams);

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
		for (uint32_t a = 0; a < config::nmasters; a++)
			masteroutputs[a]->Write(*scannervecs[a]->GetInterleavedVector(), 1);

		// Open shutters
		for (auto& s : slaveshutters)
			s.Open();
		for (auto& s : mastershutters)
			s.Open();

		// Turn the resonance scanner relay on
		if ( config::scannerselect == config::ScannerEnum::ResonantGalvo)
			for (auto& s : switches)
				s.TurnOn();

		// Wait until the synchronization signal is in the steady state
		//		std::this_thread::sleep_for(std::chrono::milliseconds(600));
		//		std::this_thread::sleep_for(std::chrono::milliseconds(1500));

		// Define lambdas for starting all inputs and all outputs
		// always area 0 last, so it (e.g. its /ao/StartTrigger) can serve as common master trigger for everything else
		std::function<void(void)> start_inputs = [&]() {
			for (uint32_t a = 1; a < config::nmasters; a++)
				inputs[a]->Start();
			inputs[0]->Start();
		};
		std::function<void(void)> start_masteroutputs = [&]() {
			for (uint32_t a = 1; a < config::nmasters; a++)
				masteroutputs[a]->Start();
			masteroutputs[0]->Start();
		};
		std::function<void(void)> start_slaveoutputs = [&]() {
			for (uint32_t a = 1; a < config::nslaves; a++)
				slaveoutputs[a]->Start();
			slaveoutputs[0]->Start();
		};

		// start inputs or outputs first
		if (ctrlparams.startinputsfirst()) {
			start_inputs();
			start_slaveoutputs();
			start_masteroutputs();
		}
		else {
			start_slaveoutputs();
			start_masteroutputs();
			start_inputs();
		}

		// Let "Run" run asynchronously
		for (uint32_t a = 0; a < config::nmasters; a++) {
			// Reset the stop condition
			stops[a].Set(false);
			// Get the async's future
			futures[a] = std::async(std::bind(&DaqController::Run, this, &stops[a], a));
		}
	}

	void DaqController::OnlineParameterUpdate(const uint32_t& _masterarea, const parameters::MasterArea& _areaparameters) {

		// update parameters
		ctrlparams.masterareas[_masterarea] = _areaparameters;
		// Note: scannervector was updated already from ScopeControllerImpl

		// Lock now so starting an async Worker is only possible if a putative previous wait on that lock finished
		std::unique_lock<std::mutex> lock(online_update_done_mutexe[_masterarea]);

		// If we are scanning live do async online update. Always catch the async future since the futures destructor waits (see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3451.pdf)
		if (ctrlparams.requested_mode() == DaqModeHelper::continuous)
			auto f = std::async(std::bind(&DaqController::WorkerOnlineParameterUpdate, this, _masterarea));

		// wait until online update is done or aborted in async WorkerOnlineParameterUpdate
		while (!online_update_done_flag[_masterarea])
			online_update_done[_masterarea].wait(lock);
	}

	void DaqController::WorkerOnlineParameterUpdate(const uint32_t _masterarea) {
		DBOUT(L"WorkerOnlineParameterUpdate starting");
		auto scannervec = scannervecs[_masterarea]->GetInterleavedVector();

		// Number of blocks => around 4 blocks per second frame time
		uint32_t blocks = round2ui32(4.0 * ctrlparams.masterareas[_masterarea].FrameTime());

		DBOUT(L"WorkerOnlineParameterUpdate blocks" << blocks);
		online_update_done_flag[_masterarea] = false;

		// If we call outputs[_area]->AbortWrite this Write exits prematurely
		masteroutputs[_masterarea]->Write(*scannervec, blocks);

		// when update is done, signal the waiting condition_variable in OnlineParameterUpdate
		online_update_done_flag[_masterarea] = true;
		online_update_done[_masterarea].notify_all();
		DBOUT(L"WorkerOnlineParameterUpdate ended");
	}

	void DaqController::AbortOnlineParameterUpdate(const uint32_t& _masterarea) {
		// Abort the output on its next block write. Note, the Write is inside lock
		masteroutputs[_masterarea]->AbortWrite();
	}

	void DaqController::ZeroGalvoOutputs() {
		// Cancel all currently configured tasks
		for (auto& o : masteroutputs) {
			if (o.get() != nullptr)
				o.reset(nullptr);
		}
		for (auto& o : slaveoutputs) {
			if (o.get() != nullptr)
				o.reset(nullptr);
		}
		for (auto& i : inputs) {
			if (i.get() != nullptr)
				i.reset(nullptr);
		}
		stimulation.reset(nullptr);

		// Do the zero outputs tasks
		for (const auto& ma : ctrlparams.masterareas)
			config::OutputZeroType zero(*dynamic_cast<config::OutputParametersType*>(ma.daq.outputs.get()));

		for (const auto& sa : ctrlparams.slaveareas)
			config::SlaveOutputZeroType zero(*dynamic_cast<config::SlaveOutputParametersType*>(sa.daq.outputs.get()));
	}

	void DaqController::SetScannerVector(const uint32_t& _masterarea, ScannerVectorFrameBasicPtr _sv) {
		scannervecs[_masterarea] = _sv;
	}

	void DaqController::OpenCloseMasterShutter(const uint32_t& _masterarea, const bool& _open) {
		mastershutters[_masterarea].Set(_open);
	}

	void DaqController::OpenCloseSlaveShutter(const uint32_t& _slavearea, const bool& _open) {
		slaveshutters[_slavearea].Set(_open);
	}

	bool DaqController::GetMasterShutterState(const uint32_t& _masterarea) const {
		return mastershutters[_masterarea].GetState();
	}

	bool DaqController::GetSlaveShutterState(const uint32_t& _slavearea) const {
		return slaveshutters[_slavearea].GetState();
	}

	void DaqController::TurnOnOffSwitchResonance(const uint32_t& _masterarea, const bool& _on) {
		for (auto& s : switches)
			s.Set(_on);
	}

	bool DaqController::GetSwitchResonanceState(const uint32_t& _masterarea) const {
		return switches[_masterarea].GetState();
	}

}