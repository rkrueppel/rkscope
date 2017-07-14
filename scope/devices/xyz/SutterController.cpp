#include "stdafx.h"
#include "SutterController.h"
#include "helpers\ScopeException.h"
#include "helpers\helpers.h"
#include "controllers\ScopeLogger.h"

#ifdef SCOPE_USE_SUTTER_XYZSTAGE

namespace scope {

std::mutex SutterController::mutex;

SutterController::SutterController(const std::wstring& _comstring)
	: comstring(_comstring) {
}

SerialConnection& SutterController::Ctrl() {
	return CreateInstance(comstring);
}

void SutterController::CheckError(const std::vector<BYTE>& _received) {
	// Returned bytes should end with a carriage return/CR/"\r"/0x0D
	if ( _received.back() != 0x0D )
		throw ScopeException("SutterController::CheckError");
}

std::vector<BYTE> SutterController::Command(const std::string& _cmd, const uint32_t& _answerbytes) {
	std::lock_guard<std::mutex> lock(mutex);
	if ( _cmd.back() != '\r' ) {
		DBOUT(L"SutterController::Command command not terminated by CR!");
		return std::vector<BYTE>(_answerbytes, 0);
	}

	Ctrl().Send(_cmd);

	std::vector<BYTE> received(Ctrl().Receive(_answerbytes));
	CheckError(received);
	return received;
}

SerialConnection& SutterController::CreateInstance(const std::wstring& _comstring) {
	// Serial connection needs comstring in a very special format for COM ports, e.g. //.//COM1 
	std::wstring realcomstring = L"//.//" + _comstring;

	static std::unique_ptr<SerialConnection> connection(new SerialConnection(realcomstring));
	return *connection;
}

}

#endif