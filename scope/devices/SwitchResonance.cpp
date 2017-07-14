#include "stdafx.h"
#include "SwitchResonance.h"
#include "helpers/ScopeException.h"

namespace scope {

SwitchResonance::SwitchResonance()
	: state(false)
	, ondata(1)
	, offdata(0) {
}

SwitchResonance::~SwitchResonance() {
	task.WriteDigitalLines(&offdata, 1, true);				// Do not check for exception here
	state = false;
}

void SwitchResonance::Initialize(const std::wstring& _outputline) {
	try {
		task.CreateTask();
		task.CreateDOChannel(_outputline);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	TurnOff();
}

void SwitchResonance::TurnOn(void) {
	try {
		task.WriteDigitalLines(&ondata, 1, true);
		state = true;
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void SwitchResonance::TurnOff(void) {
	try {
		task.WriteDigitalLines(&offdata, 1, true);
		state = false;
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

void SwitchResonance::Set(const bool& _on) {
	if ( _on )
		TurnOn();
	else
		TurnOff();
}

}