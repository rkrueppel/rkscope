#include "StdAfx.h"
#include "TheScope.h"
#include "parameters\Area.h"

std::atomic<bool> scope::TheScope::instanciated(false);

namespace scope {
	
	TheScope::TheScope(const std::wstring& _initialparameterpath)
		: nareas(config::totalareas)
		, guiparameters(config::nmasters, config::nslaves)
		, counters(nareas)
		, fpubuttons(nareas)
		, scanmodebuttons(config::nmasters)
		, daq_to_pipeline(config::nmasters)
		, theDaq(config::nmasters, config::nslaves, config::slavespermaster, guiparameters, &daq_to_pipeline)
		, thePipeline(config::threads_pipeline, guiparameters, counters, &daq_to_pipeline, &pipeline_to_storage, &pipeline_to_display)
		, theStorage(config::threads_storage, guiparameters, &pipeline_to_storage)
		, theDisplay(config::threads_display, guiparameters, &pipeline_to_display)
		, theFPUs(guiparameters.allareas, fpubuttons)
		, theController(config::totalareas, guiparameters, counters, theDaq, thePipeline, theStorage, theDisplay, daq_to_pipeline, pipeline_to_storage, pipeline_to_display, theStage)
	{
		//Make sure that TheScope is instanciated only once
		assert(!instanciated);
		instanciated = true;
	
		// Loads initial parameters
		guiparameters.Load(_initialparameterpath);
		
		// Now initialize the hardware with the loaded parameters
		// Give guiparameters by reference, so hardware has parameters and can connect to ScopeNumbers
		theStage.Initialize(guiparameters.stage);
		
		// FPUController already has the reference to guiparameters (see initializer list above)
		theFPUs.Initialize();
		
		// Connect buttons to functions
		// QuitButton is connected in CMainDlgFrame
		runbuttons.startsingle.Connect(std::bind(&ScopeController::StartSingle, &theController));
		runbuttons.startlive.Connect(std::bind(&ScopeController::StartLive, &theController));
		runbuttons.startstack.Connect(std::bind(&ScopeController::StartStack, &theController));
		runbuttons.starttimeseries.Connect(std::bind(&ScopeController::StartTimeseries, &theController));
		runbuttons.startbehavior.Connect(std::bind(&ScopeController::StartBehavior, &theController));
		runbuttons.stop.Connect(std::bind(&ScopeController::Stop, &theController));

		stackbuttons.starthere.Connect(std::bind(&TheScope::StackStartHere, this));
		stackbuttons.stophere.Connect(std::bind(&TheScope::StackStopHere, this));
		
		// We cannot connect directly to XYZStage::SetZero because std::bind needs the class to be copyable (which it may  not)
		zerobuttons.stage.Connect(std::bind(&TheScope::SetStageZero, this));
		
		zerobuttons.galvos.Connect(std::bind(&TheScope::ZeroGalvoOutputs, this));
		
		for (uint32_t a = 0; a < config::totalareas; a++) {
			// Initially choose the first supported scannervector in the list
			SetScanMode(a, *ScannerSupportedVectors::List().begin());

			// Connect update functions in TheScope to update functions inside the ScannerVectors
			for (auto& sv : guiparameters.allareas[a]->scannervectorframesmap) {
				sv.second->ConnectOnlineUpdate(std::bind(&ScopeController::OnlineUpdate, &theController, a));
				sv.second->ConnectResolutionChange(std::bind(&TheScope::ResolutionChange, this, a));
			}
			
			// Connect update functions in TheScope to some parameters outside the ScannerVectors
			guiparameters.allareas[a]->daq.pixeltime.ConnectOther(std::bind(&ScopeController::OnlineUpdate, &theController, a));
			guiparameters.allareas[a]->daq.scannerdelay.ConnectOther(std::bind(&ScopeController::OnlineUpdate, &theController, a));
			guiparameters.allareas[a]->histrange.ConnectOther(std::bind(&ScopeController::OnlineUpdate, &theController, a));
			//guiparameters.areas[a]->frameresonance.yres.ConnectOther(std::bind(&ResolutionChange, this, a));

			// Connect FPU XY movements inside the FPUController!!
		}

		// Connect the buttons for scan mode switching (if a master area) to TheScope::SetScanMode
		uint32_t ma = 0;
		for (auto& bv : scanmodebuttons) {
			for (auto& b : bv.map)
				b.second.Connect(std::bind(&TheScope::SetScanMode, this, ma * (config::slavespermaster + 1), b.first));
			ma++;
		}


	}
	
	void TheScope::CreateAndShowMainWindow() {
		wndmain = std::make_unique<scope::gui::CMainDlgFrame>(theController, theDaq, theDisplay, guiparameters, runbuttons, fpubuttons, scanmodebuttons, stackbuttons, zerobuttons, counters);

		RECT rec = { 20,20,440,980 };						// 262x403
		if (wndmain->CreateEx(HWND(0), rec) == NULL)
			throw (std::exception("Main window creation failed"));

		wndmain->ShowWindow(SW_SHOWDEFAULT);

		// Set main window title to current Scope git commit hash
		std::wstring revstr = CA2W(STR(LASTGITCOMMIT));
		revstr = L"Scope (Git commit " + revstr + L")";
		wndmain->SetWindowText(revstr.c_str());
		
		// Connect the quit button
		runbuttons.quit.Connect(std::bind(&gui::CMainDlgFrame::QuitApplication, wndmain.get()));

		SetGuiCtrlState();
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
				guiparameters.stack.startat[a].position = guiparameters.allareas[a]->Currentframe().fastz();

			guiparameters.stack.startat[a].pockels = guiparameters.allareas[a]->Currentframe().pockels();
		}
	}

	void TheScope::StackStopHere() {
		for (uint32_t a = 0; a < nareas; a++) {
			// If z stage every area has the same start position
			if (guiparameters.stack.zdevicetype() == ZDeviceHelper::ZStage)
				guiparameters.stack.stopat[a].position = theStage.CurrentZPosition();
			// If fast z then each area can have different positions
			else
				guiparameters.stack.stopat[a].position = guiparameters.allareas[a]->Currentframe().fastz();

			guiparameters.stack.stopat[a].pockels = guiparameters.allareas[a]->Currentframe().pockels();
		}
	}
	
	void TheScope::SetStageZero() {
		theStage.SetZero();
	}

	void TheScope::ZeroGalvoOutputs() {
		if (guiparameters.run_state() == RunStateHelper::Mode::Stopped) {
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
		for (auto& b : fpubuttons) {
			b.left.Enable(state);
			b.right.Enable(state);
			b.up.Enable(state);
			b.down.Enable(state);
		}
	}
	
	void TheScope::SetGuiCtrlState() {
		// Signal other GUI elements (e.g. preset combo box in CFrameScanPage, see CFrameScanPage::SetReadOnlyWhileScanning)
		wndmain->SetReadOnlyWhileScanning((guiparameters.run_state() == RunStateHelper::Mode::Stopped) ? false : true);

		// This takes care of all the parameters
		guiparameters.SetReadOnlyWhileScanning(guiparameters.run_state());

		// Now we set the state of all buttons
		const bool buttonenabler = (guiparameters.run_state() == RunStateHelper::Mode::Stopped) ? true : false;

		runbuttons.startsingle.Enable(buttonenabler);
		runbuttons.startlive.Enable(buttonenabler);
		runbuttons.startstack.Enable(buttonenabler);
		runbuttons.starttimeseries.Enable(buttonenabler);
		runbuttons.startbehavior.Enable(buttonenabler);

		/*for (auto& smb : scanmodebuttonsvec)
			for (auto& b : smb.map)
				b.second.Enable
		*/	
			
		// Only enable scan mode buttons for master area and only if the mode is supported by builtin scanners
		for ( auto& ar : scanmodebuttons )
			for (auto& b : ar.map)
				b.second.Enable(buttonenabler && ScannerSupportedVectors::IsBuiltinSupported(b.first));

		// Leave these enabled during live scanning
		stackbuttons.starthere.Enable(buttonenabler || (guiparameters.run_state() == RunStateHelper::Mode::RunningContinuous));
		stackbuttons.stophere.Enable(buttonenabler || (guiparameters.run_state() == RunStateHelper::Mode::RunningContinuous));
	}
	
	void TheScope::ResolutionChange(const uint32_t& _area) {
		theDisplay.ResolutionChange(*guiparameters.allareas[_area]);
	}
	
	void TheScope::SetScanMode(const uint32_t& _area, const ScannerVectorType& _mode) {
		// Make sure the choosen mode/scannervector is supported by the built-in scannertype
		if (ScannerSupportedVectors::IsBuiltinSupported(_mode)) {
			// Triggers update of areas.currentframe (connected to areas.ChangeScanMode)
			guiparameters.allareas[_area]->scanmode = _mode;
			//parameters = guiparameters;
			// Get fresh ScannerVector for that scan mode
			config::FramevectorFillEnum ft = (guiparameters.allareas[_area]->areatype() == AreaTypeHelper::Slave) ? config::framevectorfill_slave : config::framevectorfill_master;
			ScannerVectorFillType filltype;
			switch (ft) {
			case config::FramevectorFillEnum::FullframeXYZP:
				filltype = ScannerVectorFillTypeHelper::FullframeXYZP;
				break;
			case config::FramevectorFillEnum::LineXPColumnYZ:
				filltype = ScannerVectorFillTypeHelper::LineXPColumnYZ;
				break;
			case config::FramevectorFillEnum::LineZP:
				filltype = ScannerVectorFillTypeHelper::LineZP;
				break;
			}
			theController.framescannervecs[_area] = ScannerVectorFrameBasic::Factory(guiparameters.allareas[_area]->scanmode(), filltype);
			theDaq.SetScannerVector(_area, theController.framescannervecs[_area]);
			thePipeline.SetScannerVector(_area, theController.framescannervecs[_area]);
			
			wndmain->ChangeScanMode(_area, _mode);
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
		theController.Stop();
		//SetGuiCtrlState();
	}

}