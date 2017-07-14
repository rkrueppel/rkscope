#pragma once

#include "daqmx/DAQmxTask.h"

namespace scope {

/** Controls a shutter via DAQmx one line digital output
*  @ingroup ScopeComponentsHardware */
class Shutter {

protected:
	/** the DAQmx task */
	DAQmx::CDAQmxDigitalOutTask task;

	/** current shutter state */
	mutable std::atomic<bool> state;

	/** a one */
	const uint8_t ondata;

	/** a zero */
	const uint8_t offdata;

public:
	Shutter();

	/** Shutter gets closed on destruction */
	~Shutter();

	/** Shutter gets closed on initialization */
	void Initialize(const std::wstring& _outputline);

	/** Opens the shutter */
	void Open(void);

	/** Closes the shutter */
	void Close(void);

	/** Open/Closes the shutter */
	void Set(const bool& _open);

	/** @return the current shutter state (true = open) */
	bool GetState() const { return state; }
};

}