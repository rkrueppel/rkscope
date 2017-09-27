#include "StdAfx.h"
#include "TheScope.h"

std::atomic<bool> scope::TheScope::instanciated(false);

namespace scope {
	
	bool ThisIsSlaveArea(const uint32_t& _a) {
		#ifdef SCOPE_NBEAM_SETUP
			return _a != 0;
		#else
			return false;
		#endif
	}

	uint32_t ThisAreaOrMasterArea(const uint32_t& _a) {
		return ThisIsSlaveArea(_a)?0:_a;
	}
	
	TheScope::TheScope(const uint32_t& _nareas, const std::wstring& _initialparameterpath)
		: nareas(_nareas)
		, fpubuttonsvec(_nareas)
		, scanmodebuttonsvec(_nareas)
		, counters(_nareas)
		, daq_to_pipeline(_nareas)
		, pipeline_to_storage()
		, pipeline_to_display()
		, theDaq(_nareas, guiparameters, &daq_to_pipeline)
		, thePipeline(_nareas, guiparameters, counters, &daq_to_pipeline, &pipeline_to_storage, &pipeline_to_display)
		, theStorage(_nareas, guiparameters, &pipeline_to_storage)
		, theDisplay(_nareas, guiparameters, &pipeline_to_display)
		, theFPUs(_nareas, guiparameters.areas, fpubuttonsvec)
		, theStage()
		, theController(_nareas, guiparameters, counters, theDaq, thePipeline, theStorage, theDisplay, &daq_to_pipeline, &pipeline_to_storage, &pipeline_to_display)
	{
		//Make sure that TheScope is instanciated only once
		std::assert(!instanciated);
		instanciated = true;
	
		// Loads initial parameters
		LoadParameters(_initialparameterspath);
		
		// Now initialize the hardware with the loaded parameters
		// Give guiparameters by reference, so hardware has parameters and can connect to ScopeNumbers
		theStage.Initialize(guiparameters.stage);
		
		// FPUController already has the reference to guiparameters (see initializer list above)
		theFPUs.Initialize();
		
		theFPUS.ConnectButtons(fpubuttonsvec);
		
		// Connect buttons to functions
		// QuitButton is connected in CMainDlgFrame
		runbuttons.startsingle.Connect(std::bind(&ScopeController::StartSingle, &theController));
		runbuttons.startlive.Connect(std::bind(&ScopeController::StartLive, &theController));
		runbuttons.startstack.Connect(std::bind(&ScopeController::StartStack, &theController));
		runbuttons.starttimeseries.Connect(std::bind(&ScopeController::StartTimeseries, &theController));
		runbuttons.startbehavior.Connect(std::bind(&ScopeController::StartBehavior, &theController));
		runbuttons.stop.Connect(std::bind(&ScopeController::Stop, &theController));

		stackbuttons.starthere.Connect(std::bind(&StackStartHere, this));
		stackbuttons.stophere.Connect(std::bind(&StackStopHere, this));
		
		// We cannot connect directly to XYZStage::SetZero because std::bind needs the class to be copyable (which it may  not)
		miscbuttons.zerostage.Connect(std::bind(&SetStageZero, this));
		
		miscbuttons.zerogalvos.Connect(std::bind(&ZeroGalvoOutpus, this));
		
		for (uint32_t a = 0; a < nareas; a++) {
			// Initially choose the first supported scannervector in the list
			SetScanMode(a, *ScannerSupportedVectors::List().begin());

			// Connect the buttons for scan mode switching (if a master area) to TheScope::SetScanMode
			if (!ThisIsSlaveArea(a)) {
				for (auto& b : scanmodebuttonsvec[a].map)
					b.second.Connect(std::bind(&SetScanMode, this, a, b.first));
			}

			// Connect update functions in TheScope to update functions inside the ScannerVectors
			for (auto& sv : guiparameters.areas[a].scannervectorframesmap) {
				sv.second->ConnectOnlineUpdate(std::bind(&ScopeController::OnlineUpdate, theController, a));
				sv.second->ConnectResolutionChange(std::bind(&ResolutionChange, this, a));
			}
			
			// Connect update functions in TheScope to some parameters outside the ScannerVectors
			guiparameters.areas[a].daq.pixeltime.ConnectOther(std::bind(&ScopeController::OnlineUpdate, theController, a));
			guiparameters.areas[a].daq.scannerdelay.ConnectOther(std::bind(&ScopeController::OnlineUpdate, theController, a));
			guiparameters.areas[a].histrange.ConnectOther(std::bind(&ScopeController::OnlineUpdate, theController, a));
			//guiparameters.areas[a]->frameresonance.yres.ConnectOther(std::bind(&ResolutionChange, this, a));

			// Connect FPU XY movements inside the FPUController!!
		}

		SetGuiCtrlState();
	}
	
	void TheScope::CreateAndShowMainWindow() {
		wndmain = std::make_unique<scope::gui::CMainDlgFrame>(theController, theDisplay, guiparameters, runbuttons, fpubuttonsvec, scanmodebuttonsvec, stackbuttons, zerobuttons, counters);

		RECT rec = { 20,20,440,980 };						// 262x403
		if (wndmain->CreateEx(HWND(0), rec) == NULL)
			throw (std::exception("Main window creation failed"));

		wndmain->ShowWindow(SW_SHOWDEFAULT);

		// Set main window title to current Scope git commit hash
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Git commit " + revstr + L")";
		wndmain->SetWindowText(revstr.c_str());
		
		// Connect the quit button
		theController.runbuttons.quit.Connect(std::bind(&CMainDlgFrame::QuitApplication, wndmain));
		
	}

	void TheScope::Version() const {
		// Extract last commit date
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Last Git commit " + revstr + L")";
		DBOUT(revstr.c_str());
	}

	void TheScope::StackStartHere() {
		for (uint32_t a = 0; a < nareas; a++) {
			// If z stage every area has the same start position
			if (guiparameters.stack.zdevicetype() == ZDeviceHelper::ZStage)
				guiparameters.stack.startat[a].position = theStage.CurrentZPosition();
			// If fast z then each area can have different positions
			else
				guiparameters.stack.startat[a].position = guiparameters.areas[a]->Currentframe().fastz();

			guiparameters.stack.startat[a].pockels = guiparameters.areas[a]->Currentframe().pockels();
		}
	}

	void TheScope::StackStopHere() {
		for (uint32_t a = 0; a < nareas; a++) {
			// If z stage every area has the same start position
			if (guiparameters.stack.zdevicetype() == ZDeviceHelper::ZStage)
				guiparameters.stack.stopat[a].position = theStage.CurrentZPosition();
			// If fast z then each area can have different positions
			else
				guiparameters.stack.stopat[a].position = guiparameters.areas[a]->Currentframe().fastz();

			guiparameters.stack.stopat[a].pockels = guiparameters.areas[a]->Currentframe().pockels();
		}
	}
	
	void TheScope::SetStageZero() {
		theStage.SetZero();
	}

	void TheScope::ZeroGalvoOutputs() {
		if (parameters.run_state() == RunStateHelper::Mode::Stopped) {
			DBOUT(L"Zeroing galvo outputs\n");
			theDaq.ZeroGalvoOutputs();
		}
	}
	
	void TheScope::OpenCloseShutter(const uint32_t& _area, const bool& _open) {
		theDaq.OpenCloseShutter(_area, _open);
	}

	bool TheScope::GetShutterState(const uint32_t& _area) const {
		return theDaq.GetShutterState(_area);
	}

	void TheScope::TurnOnOffSwitchResonance(const uint32_t& _area, const bool& _on) {
		theDaq.TurnOnOffSwitchResonance(_area, _on);
	}

	/** @return current state of the first resonance scanner relay  */
	bool TheScope::GetSwitchResonanceState(const uint32_t& _area) const {
		return theDaq.GetSwitchResonanceState(_area);
	}
	
	void TheScope::SetFPUButtonsState(const bool& state) {
		for (auto& b : fpubuttonsvec) {
			b.left.Enable(state);
			b.right.Enable(state);
			b.up.Enable(state);
			b.down.Enable(state);
		}
	}
	
	void TheScope::SetGuiCtrlState() {
		// Signal other GUI elements (e.g. preset combo box in CFrameScanPage, see CFrameScanPage::SetReadOnlyWhileScanning)
		wndmain.SetReadOnlyWhileScanning((parameters.run_state() == RunStateHelper::Mode::Stopped) ? false : true);

		// This takes care of all the parameters
		guiparameters.SetReadOnlyWhileScanning(parameters.run_state());

		// Now we set the state of all buttons
		const bool buttonenabler = (parameters.run_state() == RunStateHelper::Mode::Stopped) ? true : false;

		runbuttons.startsingle.Enable(buttonenabler);
		runbuttons.startlive.Enable(buttonenabler);
		runbuttons.startstack.Enable(buttonenabler);
		runbuttons.starttimeseries.Enable(buttonenabler);
		runbuttons.startbehavior.Enable(buttonenabler);

		/*for (auto& smb : scanmodebuttonsvec)
			for (auto& b : smb.map)
				b.second.Enable
		*/	
			
		for (uint32_t a = 0; a < nareas; a++) {
			// Only enable scan mode buttons for master area and only if the mode is supported by builtin scanners
			if (!ThisIsSlaveArea(a)) {
				for (auto& b : scanmodebuttonsvec[a].map)
					b.second.Enable(buttonenabler && ScannerSupportedVectors::IsBuiltinSupported(b.first));
			}
			// Disable slave area's scan mode buttons
			else {
				for (auto& b : scanmodebuttonsvec[a].map)
					b.second.Enable(false);
			}
		}
		// Leave these enabled during live scanning
		stackbuttons.starthere.Enable(buttonenabler || (parameters.run_state() == RunStateHelper::Mode::RunningContinuous));
		stackbuttons.stophere.Enable(buttonenabler || (parameters.run_state() == RunStateHelper::Mode::RunningContinuous));
	}
	
	void TheScope::ResolutionChange(const uint32_t& _area) {
		theDisplay.ResolutionChange(guiparameters.areas[_area]);
	}
	
	void TheScope::SetScanMode(const uint32_t& _area, const ScannerVectorType& _mode) {
		// Make sure the choosen mode/scannervector is supported by the built-in scannertype
		if (ScannerSupportedVectors::IsBuiltinSupported(_mode)) {
			// Triggers update of areas.currentframe (connected to areas.ChangeScanMode)
			guiparameters.areas[_area].scanmode = _mode;
			parameters = guiparameters;
			// Get fresh ScannerVector for that scan mode
			ScannerVectorFillType filltype = (parameters.areas[_area].isslave()) ? SCOPE_SLAVEFRAMEVECTORFILL : SCOPE_MASTERFRAMEVECTORFILL;
			scope_controller.framescannervecs[_area] = ScannerVectorFrameBasic::Factory(parameters.areas[_area].scanmode(), filltype);
			theDaq.SetScannerVector(_area, scope_controller.framescannervecs[_area]);
			thePipeline.SetScannerVector(_area, scope_controller.framescannervecs[_area]);
			
			wndmain.ChangeScanMode(_area, _mode);
		}
		else {
			throw ScopeException("Choosen ScanMode is not supported by built-in scanner type");
		}
	}
	
	void TheScope::PrepareQuit() {
		// MainDlgWindows already destroyed when this is called from wWinMain in scope.cpp
		//SetFPUButtonsState(false);
		//StopButton.Enable(false);
		theStage.StopPolling();
		for (const auto& s : theFPUs.theXYStages)
			s->StopPolling();
		Stop();
		//SetGuiCtrlState();
	}

}