#include "stdafx.h"
#include "Shutter.h"
#include "helpers/ScopeException.h"

namespace scope {

Shutter::Shutter()
	: state(false)
	, ondata(1)
	, offdata(0) {
}

Shutter::~Shutter() {
	task.WriteDigitalLines(&offdata, 1, true);				// Do not check for exception here
	state = false;
}

void Shutter::Initialize(const std::wstring& _outputline) {
	try {
		task.CreateTask();
		task.CreateDOChannel(_outputline);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	Close();
}

void Shutter::Open(void) {
	try {
		task.WriteDigitalLines(&ondata, 1, true);
		state = true;
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void Shutter::Close(void) {
	try {
		task.WriteDigitalLines(&offdata, 1, true);
		state = false;
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void Shutter::Set(const bool& _open) {
	if ( _open )
		Open();
	else
		Close();
}

}