#pragma once

#include "daqmx/DAQmxTask.h"

namespace scope {

/** Controls the relay for turning on and off the resonance scanner via DAQmx one line digital output
*  @ingroup ScopeComponentsHardware */
class SwitchResonance {

protected:
	/** the DAQmx task */
	DAQmx::CDAQmxDigitalOutTask task;

	/** current relay state */
	mutable std::atomic<bool> state;

	/** a one */
	const uint8_t ondata;

	/** a zero */
	const uint8_t offdata;

public:
	SwitchResonance();

	/** Relay gets closed on destruction */
	~SwitchResonance();

	/** Relay gets closed on initialization */
	void Initialize(const std::wstring& _outputline);

	/** Turns the relay on */
	void TurnOn(void);

	/** Turns the relay off */
	void TurnOff(void);

	/** Turns the relay on and off */
	void Set(const bool& _on);

	/** @return the current relay state (true = on) */
	bool GetState() const { return state; }
};

}