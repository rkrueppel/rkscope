#include "StdAfx.h"
#include "XYZControl.h"
#include "parameters/Devices.h"

namespace scope {

XYZControl::XYZControl()
	: initialized(false)
	, pollinterval(1000) {
}

XYZControl::~XYZControl() {
	StopPolling();
}

void XYZControl::Initialize(parameters::XYZControl& _params) {
	xyzpos[0] = &_params.xpos;
	xyzpos[1] = &_params.ypos;
	xyzpos[2] = &_params.zpos;
	pollinterval = round2ui32(_params.pollinterval());
	// Polling more frequently will most definitely lead to problems...
	if ( pollinterval > 100 ) {
		// we need to call from the this pointer to call the most derived version of StartPolling
		this->StartPolling();
	}
	initialized = true;
}

void XYZControl::StartPolling() {
	fut = std::async([this]() {
		while (!stop.IsSet()) {
			UpdatePositionValues();
			std::this_thread::sleep_for(std::chrono::milliseconds(pollinterval));
		} 
		return true;
	} );
}

void XYZControl::StopPolling() {
	stop.Set();
	if ( fut.valid() )
		fut.wait();
}

void XYZControl::UpdatePositionValues() {
	// Do nothing
}

void XYZControl::SetZero() {
	SetZeroXAxis();
	SetZeroYAxis();
	SetZeroZAxis();
}

}