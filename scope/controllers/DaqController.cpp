#include "stdafx.h"
#include "DaqController.h"

namespace scope {

	DaqController::DaqController(const uint32_t& _nactives
		, const parameters::Scope& _parameters
		, std::vector<SynchronizedQueue<ScopeMessage<SCOPE_DAQMULTICHUNKPTR_T>>>* const _oqueues
	)
		: BaseController(_nactives)
		, ctrlparams(_parameters)
		, output_queues(_oqueues)
		, stimulation(nullptr)
		, shutters(_nactives)
		, switches(_nactives)
		, chunksizes(_nactives, 16000)
		, scannervecs(SCOPE_NAREAS)
		, inputs(SCOPE_NAREAS)
		, outputs(SCOPE_NAREAS)
		, online_update_done(SCOPE_NAREAS)
		, online_update_done_mutexe(SCOPE_NAREAS)
	{

		uint32_t a = 0;
		for (auto& sh : shutters)
			sh.Initialize(ctrlparams.areas[a++].daq.shutterline());
		a = 0;
		for (auto& sw : switches)
			sw.Initialize(ctrlparams.areas[a++].daq.switchresonanceline());

		for (a = 0; a < SCOPE_NAREAS; a++)
			online_update_done_flag[a] = false;

		ZeroGalvoOutputs();
	}

	DaqController::~DaqController() {
		StopAll();
		WaitForAll(-1);
		ZeroGalvoOutputs();
		DBOUT(L"DaqController::~DaqControllerImpl");
	}


	ControllerReturnStatus DaqController::Run(StopCondition* const sc, const uint32_t& _area) {
		DBOUT(L"DaqController::Run area " << _area << L" beginning");
		ControllerReturnStatus returnstatus(ControllerReturnStatus::none);

		const DaqMode requested_mode = ctrlparams.requested_mode();
		uint32_t chunksize = inputs[_area]->StandardChunkSize();
		const uint32_t requested_samples = inputs[_area]->RequestedSamples();
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
			auto chunk = std::make_shared<SCOPE_DAQMULTICHUNK_T>(chunksize, ctrlparams.areas[_area].daq.inputs->channels());

			// With this loop we can interrupt a thread that is waiting here for a trigger (see FiberMRI program)
			// or which waits for samples (which never come because of an error)
			currentlyread = 0;
			do {
				// Read data from inputs into the chunk. Timeout 1 second.
				currentlyread = inputs[_area]->Read(_area, *chunk, timedout, 1);
				DBOUT(L"DaqController::Impl::Run read");
				// In case of timeout attempt to read as long as stop condition is not set
			} while (timedout && !sc->IsSet());
			timedout = false;

			ScopeMessage<SCOPE_DAQMULTICHUNKPTR_T> msg;
			msg.tag = ScopeMessageTag::nothing;
			msg.cargo = chunk;

			// ...and put it in the queue
			output_queues->at(_area).Enqueue(msg);

			// advance number of read pixels
			readsamples += currentlyread;
			DBOUT(L"DaqController::Impl::Run area " << _area << L" read " << readsamples << L" of requested " << requested_samples);

			// Check if we read enough samples (if not live scanning)
			if ((requested_mode == DaqModeHelper::nframes) && (readsamples == requested_samples)) {
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
		for (auto& s : shutters)
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
		for (uint32_t a = 0; a < nactives; a++) {
			// Choose output type depending on that area being a slave area
			if (ctrlparams.areas[a].isslave())
				outputs[a] = std::make_unique<SCOPE_SLAVEOUTPUTS_T>(a, *dynamic_cast<parameters::SCOPE_SLAVEOUTPUTS_T*>(ctrlparams.areas[a].daq.outputs.get()), ctrlparams);
			else
				outputs[a] = std::make_unique<SCOPE_OUTPUTS_T>(a, *dynamic_cast<parameters::SCOPE_OUTPUTS_T*>(ctrlparams.areas[a].daq.outputs.get()), ctrlparams);
			
			inputs[a].reset(new SCOPE_INPUTS_T(a, dynamic_cast<parameters::SCOPE_INPUTS_T*>(ctrlparams.areas[a].daq.inputs.get()), ctrlparams));
		}

		// Calculate and write stimulationvector to device
		if (ctrlparams.stimulation.enable()) {
			stimulation = std::make_unique<SCOPE_STIMULATIONS_T>(ctrlparams);
			stimvec.SetParameters(ctrlparams.stimulation);
			stimulation->Write(stimvec.GetVector());
			stimulation->Start();
		}
		else
			stimulation.reset(nullptr);

		// Write scannervectors to devices
		for (uint32_t a = 0; a < nactives; a++)
			outputs[a]->Write(*scannervecs[a]->GetInterleavedVector(), 1);

		// Open shutters
		for (auto& s : shutters)
			s.Open();

		// Turn the resonance scanner relay on
		for (auto& s : switches)
			s.TurnOn();
		// Wait until the synchronization signal is in the steady state
		//		std::this_thread::sleep_for(std::chrono::milliseconds(600));
		//		std::this_thread::sleep_for(std::chrono::milliseconds(1500));

		// Define lambdas for starting all inputs and all outputs
		// always area 0 last, so it (e.g. its /ao/StartTrigger) can serve as common master trigger for everything else
		std::function<void(void)> start_inputs = [&]() {
			for (uint32_t a = 1; a < nactives; a++)
				inputs[a]->Start();
			inputs[0]->Start();
		};
		std::function<void(void)> start_outputs = [&]() {
			for (uint32_t a = 1; a < nactives; a++)
				outputs[a]->Start();
			outputs[0]->Start();
		};

		// start inputs or outputs first
		if (ctrlparams.startinputsfirst()) {
			start_inputs();
			start_outputs();
		}
		else {
			start_outputs();
			start_inputs();
		}

		// Let "Run" run asynchronously
		for (uint32_t a = 0; a < nactives; a++) {
			// Reset the stop condition
			stops[a].Set(false);
			// Get the async's future
			futures[a] = std::async(std::bind(&DaqController::Run, this, &stops[a], a));
		}
	}

	void DaqController::OnlineParameterUpdate(const parameters::Area& _areaparameters) {
		uint32_t area = _areaparameters.area();

		// update parameters
		ctrlparams.areas[area] = _areaparameters;
		// Note: scannervector was updated already from ScopeControllerImpl

		// Lock now so starting an async Worker is only possible if a putative previous wait on that lock finished
		std::unique_lock<std::mutex> lock(online_update_done_mutexe[area]);

		// If we are scanning live do async online update. Always catch the async future since the futures destructor waits (see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3451.pdf)
		if (ctrlparams.requested_mode() == DaqModeHelper::continuous)
			auto f = std::async(std::bind(&DaqController::WorkerOnlineParameterUpdate, this, area));

		// wait until online update is done or aborted in async WorkerOnlineParameterUpdate
		while (!online_update_done_flag[area])
			online_update_done[area].wait(lock);
	}

	void DaqController::WorkerOnlineParameterUpdate(const uint32_t _area) {
		DBOUT(L"WorkerOnlineParameterUpdate starting");
		auto scannervec = scannervecs[_area]->GetInterleavedVector();

		// Number of blocks => around 4 blocks per second frame time
		uint32_t blocks = round2ui32(4.0 * ctrlparams.areas[_area].FrameTime());

		DBOUT(L"WorkerOnlineParameterUpdate blocks" << blocks);
		online_update_done_flag[_area] = false;

		// If we call outputs[_area]->AbortWrite this Write exits prematurely
		outputs[_area]->Write(*scannervec, blocks);

		// when update is done, signal the waiting condition_variable in OnlineParameterUpdate
		online_update_done_flag[_area] = true;
		online_update_done[_area].notify_all();
		DBOUT(L"WorkerOnlineParameterUpdate ended");
	}

	void DaqController::AbortOnlineParameterUpdate(const uint32_t& _area) {
		// Abort the output on its next block write. Note, the Write is inside lock
		outputs[_area]->AbortWrite();
	}

	void DaqController::ZeroGalvoOutputs() {
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
		for (const auto& ap : ctrlparams.areas) {
			if (ap.isslave())
				SCOPE_ZEROOUTPUTSSLAVE_T zero(*dynamic_cast<parameters::SCOPE_SLAVEOUTPUTS_T*>(ap.daq.outputs.get()));
			else
				SCOPE_ZEROOUTPUTS_T zero(*dynamic_cast<parameters::SCOPE_OUTPUTS_T*>(ap.daq.outputs.get()));
		}
	}

	void DaqController::SetScannerVector(const uint32_t& _area, ScannerVectorFrameBasicPtr _sv) {
		scannervecs[_area] = _sv;
	}

	void DaqController::OpenCloseShutter(const uint32_t& _area, const bool& _open) {
		for (auto& s : shutters)
			s.Set(_open);
	}

	bool DaqController::GetShutterState(const uint32_t& _area) const {
		return shutters[_area].GetState();
	}

	void DaqController::TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
		for (auto& s : switches)
			s.Set(_on);
	}

	bool DaqController::GetSwitchResonanceState(const uint32_t& _area) const {
		return switches[_area].GetState();
	}

}