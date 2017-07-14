#include "StdAfx.h"
#include "GalilController.h"
#include "helpers/ScopeException.h"
#include "helpers/helpers.h"

// Only use this code if we need it
#ifdef SCOPE_USE_GALIL_XYZSTAGE

namespace scope {

std::mutex GalilController::mutex;

GalilController::GalilController(const std::wstring& _comstring)
	: comstring(_comstring) {
}

std::wstring GalilController::LibraryVersion() {
	std::lock_guard<std::mutex> lock(mutex);
	std::wstring cstr(CA2W(Ctrl().libraryVersion().c_str()));
	return cstr;
}

std::wstring GalilController::Connection() {
	std::lock_guard<std::mutex> lock(mutex);
	std::wstring cstr(CA2W(Ctrl().connection().c_str()));
	return cstr;
}

void GalilController::SetTimeout(const int32_t& _milliseconds) {
	Ctrl().timeout_ms = _milliseconds;
}

std::wstring GalilController::Command(const std::wstring& _cmd) {
	std::lock_guard<std::mutex> lock(mutex);
	std::string cmd(CW2A(_cmd.c_str()));
	std::string ret("");
	ret = Ctrl().command(cmd);
	std::wstring cret(CA2W(ret.c_str()));
	return cret;
}

double GalilController::CommandValue(const std::wstring& _cmd) {
	std::lock_guard<std::mutex> lock(mutex);
	std::string cmd = CW2A(_cmd.c_str());
	return Ctrl().commandValue(cmd);
}

Galil& GalilController::Ctrl() {
	try {
		std::string str = CW2A(comstring.c_str());
		return CreateInstance(str);
	} catch(...) { ScopeExceptionHandler(__FUNCTION__, true, true, true, true); }
}

Galil& GalilController::CreateInstance(const std::string& stdcomstring) {
	std::string tmp(stdcomstring);
	static std::unique_ptr<Galil> _instance(new Galil(tmp));
	return *_instance;

}

}

#endif