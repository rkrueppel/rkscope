#include "stdafx.h"
#include "XYZControlGalil.h"
#include "GalilController.h"
#include "parameters/Devices.h"
#include "controllers/ScopeLogger.h"

// Only use this code if we need it
#ifdef SCOPE_USE_GALIL_XYZSTAGE

namespace scope {

XYZControlGalil::XYZControlGalil()
	: gc(nullptr)
	, xcountspermicron(1.0)
	, ycountspermicron(1.0) 
	, zcountspermicron(1.0) {
}

XYZControlGalil::~XYZControlGalil() {
	StopPolling();
}

void XYZControlGalil::Initialize(parameters::XYZControlGalil& _params) {
	xcountspermicron = _params.xcountspermicron();
	ycountspermicron = _params.ycountspermicron();
	zcountspermicron = _params.zcountspermicron();

	try {
		gc = std::unique_ptr<GalilController>(new GalilController(_params.comstring()));
		
		ScopeLogger scope_logger;
		std::wstringstream msg;
		msg << L"Connected XYControlGalil to Galil controller, library version " << gc->LibraryVersion() << L"\n" , gc->Connection();
		scope_logger.Log(msg.str(), log_info);

		gc->Command(L"SH");					// SH: servo here = switch on motor, set zero position
		gc->Command(L"WT100");
		gc->Command(L"SB1");				// SB: sets bit 1 -> loosens z axis brake
		gc->Command(L"WT500");				// WT: wait 500ms for the brake to loose				

		// call base class Initialize, which connects ScopeValues, sets initialized to true and starts the polling thread
		XYZControl::Initialize(_params);

		scope_logger.Log(L"Initialized XYZControlGalil", log_info);

	}
	catch (...) { 
		initialized = false;
		ScopeExceptionHandler(__FUNCTION__, true, true);
	}
	SetZero();
}

void XYZControlGalil::UpdatePositionValues() {
	double p = gc->CommandValue(L"PAX=?");					// PAX=?: Position absolute x
	xyzpos[0]->Set(p/xcountspermicron);
	p = gc->CommandValue(L"PAY=?");							// PAY=?: Position absolute y
	xyzpos[1]->Set(p/ycountspermicron);
	p = gc->CommandValue(L"PAZ=?");							// PAZ=?: Position absolute z
	xyzpos[2]->Set(p/zcountspermicron);
}

double XYZControlGalil::CurrentXPosition() {
	double pos = 0;
	try {
		pos = gc->CommandValue(L"PAX=?");		// PAX=?: gives position absolute x
		pos /= xcountspermicron;
		xyzpos[0]->Set(pos);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
	return pos;
}

double XYZControlGalil::CurrentYPosition() {
	double pos = 0;
	try {
		pos = gc->CommandValue(L"PAY=?");		// PAY=?: gives position absolute y
		pos /= ycountspermicron;
		xyzpos[1]->Set(pos);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
	return pos;
}

double XYZControlGalil::CurrentZPosition() {
	double pos = 0;
	try {
		pos = gc->CommandValue(L"PAZ=?");		// PAZ=?: gives position absolute z
		pos /= zcountspermicron;
		xyzpos[2]->Set(pos);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
	return pos;
}

void XYZControlGalil::SetZeroXAxis() {
	try {
		gc->Command(L"DPX=0");					// DP: define position = set current motor and command position
		UpdatePositionValues();
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
}

void XYZControlGalil::SetZeroYAxis() {
	try {
		gc->Command(L"DPY=0");					// DP: define position = set current motor and command position
		UpdatePositionValues();
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
}

void XYZControlGalil::SetZeroZAxis() {
	try {
		gc->Command(L"DPZ=0");					// DP: define position = set current motor and command position
		UpdatePositionValues();
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
}

void XYZControlGalil::MoveRelative(const double& _xrel, const double& _yrel, const double& _zrel) {
	try {
		if ( (abs(_xrel) > 2000) || (abs(_yrel) > 2000) || (abs(_zrel) > 2000) )
			throw ScopeException("XYZControlGalil::MoveRelative travel range too large");
		int32_t countsx = round2i32(static_cast<double>(xcountspermicron) * _xrel);
		int32_t countsy = round2i32(static_cast<double>(ycountspermicron) * _yrel);
		int32_t countsz = round2i32(static_cast<double>(zcountspermicron) * _zrel);
		std::wstringstream cmd;
		cmd << L"PR " << countsx << L"," << countsy << L"," << countsz;
		DBOUT(L"XYZControlGalil::MoveRelative " << cmd.str());
		gc->Command(cmd.str());										// PR: Position relative = move relative, in encoder counts
		gc->Command(L"BG");										// BG: Begin move
		gc->Command(L"MG TIME");								// just make sure the motion is complete when MoveSlow returns
	} catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
	UpdatePositionValues();
}

void XYZControlGalil::MoveAbsolute(const double& _xabs, const double& _yabs, const double& _zabs) {
	try {
		if ( (abs(_xabs) > 20000) || (abs(_yabs) > 20000) || (abs(_zabs) > 20000) )
			throw ScopeException("XYZControlGalil::MoveAbsolute travel range too large");
		int32_t countsx = round2i32(static_cast<double>(xcountspermicron) * _xabs);
		int32_t countsy = round2i32(static_cast<double>(ycountspermicron) * _yabs);
		int32_t countsz = round2i32(static_cast<double>(zcountspermicron) * _zabs);
		std::wstringstream cmd;										// PA: Position absolute = move absolute, in encoder counts
		cmd << L"PA " << countsx << L"," << countsy << L"," << countsz;
		gc->Command(cmd.str());
		gc->Command(L"BG");										// BG: Begin move
		// Motion complete does not work with external commands!
		// gc->Ctrl()->command("MC");							// MC: Motion complete = wait with next command execution until current motion finished
		gc->Command(L"MG TIME");								// just make sure the motion is complete when MoveAbsolute returns
	} catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
	UpdatePositionValues();
}

}

#endif