#include "StdAfx.h"
#include "XYControlStanda.h"
#include "parameters/Devices.h"
#include "helpers/Helpers.h"
#include "helpers/ScopeException.h"
#include "controllers/ScopeLogger.h"

#ifdef SCOPE_USE_STANDA_XYFPUSTAGE

namespace scope {

XYControlStanda::XYControlStanda(parameters::XYControlStanda& _params)
	: XYControl(_params)
	, speed(10.0)
	, microsteps_per_micron(3.226) {
}

XYControlStanda::~XYControlStanda(void) {
	StopPolling();				
	try {
		std::lock_guard<std::mutex> lock(mutex);
		// Do not turn off the device -> with the motors switched off, push buttons do not work
		/*for ( auto d : dev )
			TurnOffAndSaveToFlash(d);
			*/
		CheckError(USMC_Close());
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void XYControlStanda::Initialize() {
	try {
		std::lock_guard<std::mutex> lock(mutex);
		CheckError(USMC_Init(devices));

		std::stringstream msg;
		msg << "XYControlStanda connected\r\n";
		for ( uint32_t i = 0 ; i < devices.NOD ; i++ )
			msg << "Device " << i << " - " << devices.Serial[i] << " - " << devices.Version[i] << "\r\n";
		CA2W tmp(msg.str().c_str());
		ScopeLogger::GetInstance().Log(std::wstring(tmp), log_info);
		
		//assert(devices.NOD == SCOPE_NAREAS);
		
		// zero based device number
		dev[0] = static_cast<DWORD>(_params.devicex());
		dev[1] = static_cast<DWORD>(_params.devicey());
		speed = static_cast<float>(_params.speed());
		microsteps_per_micron = _params.microstepspermicron();
		for ( uint32_t d = 0 ; d < 2 ; d++ )
			InitAxis(dev[d]);

		// call base class Initialize, which connects ScopeValues, sets initialized to true, and starts the polling thread
		XYControl::Initialize();

		std::wstring msg2(L"Initialized XYControlStanda");
		ScopeLogger::GetInstance().Log(msg2, log_info);
	}
	catch (...) {
		initialized = false;
		ScopeExceptionHandler(__FUNCTION__);
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	_params.stageinfo = L"XYControlStanda\r\n\r\nStart Parameters\r\n" + GetStartParameters() + L"\r\n\r\nParameters\r\n"+ GetParameters() + L"\r\n\r\nMode\r\n" + GetMode();
}

void XYControlStanda::InitAxis(const DWORD& _device) {
	CheckError(USMC_GetMode(_device, mode));
	// Do not wait for external sync signal
	mode.SyncINOp = false;					
	// Power on ?
	mode.ResetD = false;					
	CheckError(USMC_SetMode(_device, mode));
	CheckError(USMC_GetStartParameters(_device, stprms));
	// set slow start
	stprms.SlStart = true;					
	stprms.LoftEn = false;
	stprms.WSyncIN = false;
	stprms.ForceLoft = false;
}

void XYControlStanda::TurnOffAndSaveToFlash(const DWORD& _device) {
	// Initialize structures
	CheckError(USMC_GetParameters(_device, prms));
	CheckError(USMC_GetMode(_device, mode));
	// Go to Full Step (automaticaly), then Turn Off
	mode.ResetD = TRUE;
	CheckError(USMC_SetMode(_device, mode));
	// Wait until Previous Comand is Done
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));;
		CheckError(USMC_GetState(_device, state));
	} while(state.Power == TRUE);
	// Remember CurPos in Parameters Only While State.Power - FALSE
	prms.StartPos = state.CurPos;
	CheckError(USMC_SetParameters( _device, prms));
	// Then of Course You Need to SaveToFlash
	CheckError(USMC_SaveParametersToFlash( _device ));
	// Now You Can Unplug ME
}

void XYControlStanda::StartPolling() {
	// If something went wrong during initialization we do not want to poll (and probably cause more errors)
	if ( initialized ) {
		fut = std::async([this]() {
			while (!stop.IsSet()) {
				try {
					std::lock_guard<std::mutex> lock(mutex);
					UpdatePositionValues();
				}
				catch (...) { ScopeExceptionHandler(__FUNCTION__); }
				std::this_thread::sleep_for(std::chrono::milliseconds(pollinterval));
			} 
			return true;
		} );
	}
}

void XYControlStanda::UpdatePositionValues() {
	for ( uint32_t d = 0 ; d < 2 ; d++ ) {
		CheckError(USMC_GetState(dev[d], state));
		// Convert from current position (in microsteps, 1/8 step) to micrometers and truncate to one digit
		double micron = round(static_cast<double>(state.CurPos)/microsteps_per_micron * 10.0) / 10.0;
		// Use Set with "call other signal" false, thus MoveAbsolute is not called as consequence (that would lead to circular calls...)
		pos[d].Set(micron, true, false);
	}
}

void XYControlStanda::SetZero() {
	try {
		std::lock_guard<std::mutex> lock(mutex);
		for ( uint32_t d = 0 ; d < 2 ; d++ )
			CheckError(USMC_SetCurrentPosition(dev[d], 0));
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void XYControlStanda::MoveRelative(const double& _xrel, const double& _yrel) {
	try {
		std::lock_guard<std::mutex> lock(mutex);
		for ( uint32_t d = 0 ; d < 2 ; d++ ) {
			CheckError(USMC_GetState(dev[d], state));
			DBOUT(L"XYControlStanda::MoveRelative from " << state.CurPos << L" by " << _xrel << L" " << _yrel);
			CheckError(USMC_Start(dev[d], state.CurPos+round2i32(microsteps_per_micron*((d==0)?_xrel:_yrel)), speed, stprms));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		//UpdatePositionValues();
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void XYControlStanda::MoveAbsolute(const double& _xabs, const double& _yabs) {
	try {
		std::lock_guard<std::mutex> lock(mutex);
		for ( uint32_t d = 0 ; d < 2 ; d++ ) {
			CheckError(USMC_GetState(dev[d], state));
			DBOUT(L"XYControlStanda::MoveAbsolute to " << _xabs << L" " << _yabs);
			CheckError(USMC_Start(dev[d], round2i32(microsteps_per_micron*((d==0)?_xabs:_yabs)), speed, stprms));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void XYControlStanda::CheckError(const DWORD& errorcode) {
	if ( errorcode != 0 ) {
		char perrorstring[256];
		USMC_GetLastErr(perrorstring, 256);
		throw( ScopeException(perrorstring) );
	}
}

std::wstring XYControlStanda::GetParameters() {
	std::lock_guard<std::mutex> lock(mutex);
	std::wstringstream stream;
	try {
		for ( uint32_t d = 0 ; d < 2 ; d++ ) {
			CheckError(USMC_GetParameters(dev[d], prms));
			stream << L"Axis: " << d << L" Device: " << dev[d] << L"\r\nAccel time (ms): " << prms.AccelT << L"\r\nDecel time (ms): " << prms.DecelT << L"\r\nCurrent reduction time (ms): ";
			stream << prms.PTimeout << L"\r\nTime to speed B time (ms): " << prms.BTimeout1 << L"\r\nReset time (ms): " << prms.BTimeoutR;
			stream << L"\r\nSpeed at reset (steps/s): " << prms.MinP << L"\r\nSpeed B (steps/s): " << prms.BTO1P << L"\r\nSteps for backlash: " << prms.MaxLoft;
			stream << L"\r\nPosition in flash: " << prms.StartPos << L"\r\nSteps per revolution: " << prms.RTDelta << L"\r\nSteps missed for error flag: ";
			stream << prms.RTMinError << L"\r\nMax temperature (C): " << prms.MaxTemp << L"\r\nSync out pulse duration: " << prms.SynOUTP;
			stream << L"\r\nLast phase backlash speed: " << prms.LoftPeriod << L"\r\nEncoder steps per rev/SM steps per rev: " << prms.EncMult << L"\r\n";
		}
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	return stream.str();
}

std::wstring XYControlStanda::GetMode() {
	std::lock_guard<std::mutex> lock(mutex);
	std::wstringstream stream;
	try {
		for ( uint32_t d = 0 ; d < 2 ; d++ ) {
			CheckError(USMC_GetMode(dev[d], mode));
			stream << L"Axis: " << d << L" Device: " << dev[d] << L"\r\nButtons disabled: " << mode.PMode << L"\r\nCurrent reduction: " << mode.PReg << L"\r\nReset: " << mode.ResetD;
			stream << L"\r\nQuick power off: " << mode.EMReset << L"\r\nTrailer 1: " << mode.Tr1T << L"\r\nTrailer 2: " << mode.Tr2T << L"\r\nRotary transducer: ";
			stream << mode.RotTrT << L"\r\nTrailers swapped: " << mode.TrSwap << L"\r\nTrailer 1 operation: " << mode.Tr1En << L"\r\nTrailer 2 operation: ";
			stream << mode.Tr2En << L"\r\nRotary transducer operation: " << mode.RotTeEn << L"\r\nRotary transducer operation select: " << mode.RotTrOp;
			stream << L"\r\nButton 1: " << mode.Butt1T << L"\r\nButton 2: " << mode.Butt2T << L"\r\nReset rotary transducer: " << mode.ResetRT;
			stream << L"\r\nSync Out enabled: " << mode.SyncOUTEn << L"\r\nSync Out counter reset: " << mode.SyncOUTR << L"\r\nSync In mode: " << mode.SyncINOp;
			stream << L"\r\nSync count: " << mode.SyncCount << L"\r\nSync invert: " << mode.SyncInvert << L"\r\nEnable Encoder: " << mode.EncoderEn;
			stream << L"\r\nInvert Encoder: " << mode.EncoderInv << L"\r\nReset Encoder position: " << mode.ResBEnc << L"\r\nReset SM position to encoder position: ";
			stream << mode.ResEnc << L"\r\n";
		}
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	return stream.str();
}

std::wstring XYControlStanda::GetState() {
	std::lock_guard<std::mutex> lock(mutex);
	std::wstringstream stream;
	try {
		for ( uint32_t d = 0 ; d < 2 ; d++ ) {
			CheckError(USMC_GetState(dev[d], state));
			stream << L"Axis: " << d << L" Device: " << dev[d] << L"\r\nCurrent position: " << state.CurPos << L"\r\nTemperature: " << state.Temp << L"\r\nStep divisor: " << state.SDivisor;
			stream << L"\r\nBacklash status: " << state.Loft << L"\r\nFull power: " << state.FullPower << L"\r\nCurrent direction: " << state.CW_CCW;
			stream << L"\r\nPower: " << state.Power << L"\r\nFull speed: " << state.FullSpeed << L"\r\nAfter reset: " << state.AReset << L"\r\nRotating: ";
			stream << state.RUN << L"\r\nInput sync state: " << state.SyncIN << L"\r\nOutput sync state: " << state.SyncOUT << L"\r\nRotary transducer state: ";
			stream << state.RotTr << L"\r\nRotary transducer error: " << state.RotTrErr << L"\r\nEmergeny disable state: " << state.EmReset << L"\r\nTrailer 1 state: ";
			stream << state.Trailer1 << L"\r\nTrailer 2 state: " << state.Trailer2 << L"\r\nVoltage: " << state.Voltage << L"\r\n";
		}
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	return stream.str();
}

std::wstring XYControlStanda::GetStartParameters() {
	std::lock_guard<std::mutex> lock(mutex);
	std::wstringstream stream;
	try {
		for ( uint32_t d = 0 ; d < 2 ; d++ ) {
			CheckError(USMC_GetStartParameters(dev[d], stprms));
			stream << L"Axis: " << d << L" Device: " << dev[d] << L"\r\nStep divisor: " << stprms.SDivisor << L"\r\nBacklash direction: " << stprms.DefDir << L"\r\nAutomatic backlash: ";
			stream << stprms.LoftEn << L"\r\nSlow start/stop enabled: " << stprms.SlStart << L"\r\nWait for input sync signal: " << stprms.WSyncIN;
			stream << L"\r\nReset sync out counter: " << stprms.SyncOUTR << L"\r\nForce backlash: " << stprms.ForceLoft << L"\r\n";
		}
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	return stream.str();
}

}

#endif