#pragma once

#include "daqmx/DAQmxTask.h"

namespace scope {

/** Controls the four-step zoom scaling for a resonance scanner by two DAQmx digital line outputs
*  @ingroup ScopeComponentsHardware */
class ZoomResonance {

protected:
	/** the DAQmx task */
	DAQmx::CDAQmxDigitalOutTask task;

	/** current zoom state */
	mutable uint8_t state;

	/** 00 */
	const uint8_t factor1;

	/** 01 */
	const uint8_t factor2;

	/** 10 */
	const uint8_t factor3;

	/** 11 */
	const uint8_t factor4;

public:
	/** Constructor, reset to amplitude 1 */
	ZoomResonance();

	/** Destructor, reset to amplitude 1 */
	~ZoomResonance();

	/** Initialization */
	void Initialize(const std::wstring& _outputline);

	/** Sets the amplitude reduction factor */
	void Set(const uint8_t& _factor);

	/** @return the current zoom state as factor by which the scanner angle gets reduced (00 = factor 1 = largest FOV, 11 = factor 4 = smallest FOV) */
	uint8_t GetState() const { return state; }
};

}