#include "stdafx.h"
#include "ZoomResonance.h"
#include "helpers/ScopeException.h"

namespace scope {

ZoomResonance::ZoomResonance()
	: state(0)
	, factor1(0)
	, factor2(1)
	, factor3(2) 
	, factor4(3) {
}

ZoomResonance::~ZoomResonance() {
	task.WriteDigitalLines(&factor1, 1, true);				// Do not check for exception here
	state = factor1;
}

void ZoomResonance::Initialize(const std::wstring& _outputline) {
	try {
		task.CreateTask();
		task.CreateDOChannel(_outputline);
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	Set(factor1);
}

void ZoomResonance::Set(const uint8_t& _factor) {
	try {
		task.WriteDigitalLines(&_factor, 1, true);
		state = &_factor;
	} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
}

}