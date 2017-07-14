#include "stdafx.h"
#include "XYZControlSutter.h"
#include "controllers/ScopeLogger.h"

#ifdef SCOPE_USE_SUTTER_XYZSTAGE

namespace scope {

XYZControlSutter::XYZControlSutter(void)
	: sc(nullptr)
	, microstepspermicron(5) {
}

XYZControlSutter::~XYZControlSutter(void) {
	StopPolling();
}

void XYZControlSutter::Initialize(parameters::XYZControlSutter& _params) {
	microstepspermicron = _params.microstepspermicron();

	try {
		sc = std::unique_ptr<SutterController>(new SutterController(_params.comstring()));
		
		ScopeLogger scope_logger;
		scope_logger.Log(L"Connected XYControlSutter to Sutter controller", log_info);

		// call base class Initialize, which connects ScopeValues, sets initialized to true, and starts the polling thread
		XYZControl::Initialize(_params);

		SetZero();

		UpdateStatus();
		UpdatePositionValues();

		scope_logger.Log( L"Initialized XYZControlSutter", log_info);
		
	}
	catch (...) { 
		initialized = false;
		ScopeExceptionHandler(__FUNCTION__, true, true);
	}
}

uint16_t XYZControlSutter::BytesToInt16(const std::vector<BYTE>& _bytevec, const uint32_t& _from) {
	if ( _bytevec.size() < (_from+2) )
		return 0;

	SutterValue16 b;							// the union does the magic
	b.intval = 0;
	b.charval[0] = _bytevec[_from];
	b.charval[1] = _bytevec[_from+1];
	return b.intval;
}

std::vector<BYTE> XYZControlSutter::Int16ToBytes(const uint16_t& _i) {
	SutterValue16 b;
	b.intval = _i;
	std::vector<BYTE> bytevec(2,0);
	bytevec[0] = b.charval[0];
	bytevec[1] = b.charval[1];
	return bytevec;
}

uint32_t XYZControlSutter::BytesToInt32(const std::vector<BYTE>& _bytevec, const uint32_t& _from) {
	if ( _bytevec.size() < (_from+4) )
		return 0;

	SutterValue32 b;							// the union does the magic
	b.intval = 0;
	b.charval[0] = _bytevec[_from];
	b.charval[1] = _bytevec[_from+1];
	b.charval[2] = _bytevec[_from+2];
	b.charval[3] = _bytevec[_from+3];
	return b.intval;
}

std::vector<BYTE> XYZControlSutter::Int32ToBytes(const uint32_t& _i) {
	SutterValue32 b;
	b.intval = _i;
	std::vector<BYTE> bytevec(4,0);
	bytevec[0] = b.charval[0];
	bytevec[1] = b.charval[1];
	bytevec[2] = b.charval[2];
	bytevec[3] = b.charval[3];
	return bytevec;
}

void XYZControlSutter::UpdateStatus() {
	std::vector<BYTE> ret = sc->Command("s\r", 33);		// return is 32 bytes status information plus trailing CR
		
	stepdiv = BytesToInt16(ret, 24);
	stepmult = BytesToInt16(ret, 26);
	speed = BytesToInt16(ret, 28);
	version = BytesToInt16(ret, 30);
}

void XYZControlSutter::SetVelocity( const uint16_t& _vel) {
	try {
		// Get current velocity resolution (whatever is set on the hand controller)
		UpdateStatus();
	
		uint16_t devvel(_vel);
		const bool highres = (speed > 32786);
	
		// higher velocities are not recommended
		if ( highres && (devvel > 1310) )
			devvel = 1310;
		if ( !highres && (devvel > 3000) )
			devvel = 3000;
	
		// Set highest bit for highres mode (50 microsteps/step)
		if ( highres )
			devvel += 32786;
	
		std::string cmd = "V";
		cmd.append(reinterpret_cast<char*>(Int16ToBytes(devvel).data()));
		cmd.append("\r");

		std::vector<BYTE> ret = sc->Command(cmd, 1);
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void XYZControlSutter::UpdatePositionValues() {
	try {
		std::vector<BYTE> ret = sc->Command("c\r", 13);		// return ist 3*4 bytes plus trailing CR = 13 bytes

		uint32_t x = BytesToInt32(ret, 0);
		uint32_t y = BytesToInt32(ret, 4);
		uint32_t z = BytesToInt32(ret, 8);

		xyzpos[0]->Set(static_cast<double>(x) / microstepspermicron);
		xyzpos[1]->Set(static_cast<double>(y) / microstepspermicron);
		xyzpos[2]->Set(static_cast<double>(z) / microstepspermicron);
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void XYZControlSutter::SetZero() {
	try {
		sc->Command("o\r", 1);								// return is only CR
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void XYZControlSutter::MoveRelative(const double& _xrel, const double& _yrel, const double& _zrel) {
	try {
		// Get current position
		std::vector<BYTE> ret = sc->Command("c\r", 13);		// return ist 3*4 bytes plus trailing CR = 13 bytes

		double xum = BytesToInt32(ret, 0) / microstepspermicron;
		double yum= BytesToInt32(ret, 4) / microstepspermicron;
		double zum = BytesToInt32(ret, 8) / microstepspermicron;

		std::string cmd("m");
		cmd.append(reinterpret_cast<char*>(Int32ToBytes(round2ui32((xum+_xrel)*microstepspermicron)).data()));
		cmd.append(reinterpret_cast<char*>(Int32ToBytes(round2ui32((yum+_yrel)*microstepspermicron)).data()));
		cmd.append(reinterpret_cast<char*>(Int32ToBytes(round2ui32((zum+_zrel)*microstepspermicron)).data()));
		cmd.append("\r");
	
		ret = sc->Command(cmd, 1);
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }

	UpdatePositionValues();
}

void XYZControlSutter::MoveAbsolute(const double& _xabs, const double& _yabs, const double& _zabs) {
	try {
		std::string cmd("m");
		cmd.append(reinterpret_cast<char*>(Int32ToBytes(round2ui32(_xabs*microstepspermicron)).data()));
		cmd.append(reinterpret_cast<char*>(Int32ToBytes(round2ui32(_yabs*microstepspermicron)).data()));
		cmd.append(reinterpret_cast<char*>(Int32ToBytes(round2ui32(_zabs*microstepspermicron)).data()));
		cmd.append("\r");
	
		std::vector<BYTE> ret = sc->Command(cmd, 1);
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__); }

	UpdatePositionValues();
}


}

#endif