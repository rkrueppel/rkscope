#pragma once

#include "parameters/Scope.h"

namespace scope {

/** Calculates a basic digital stimulation pattern. */
class StimulationVector {

protected:
	/** parameter set */
	parameters::Stimulation parameters;

	/** pointer to data vector */
	std::shared_ptr<std::vector<uint8_t>> const vecptr;

protected:
	/** Recalculates the stimulation vector */
	void UpdateVector();

public:
	StimulationVector();

	/** Sets parameters */
	void SetParameters(const parameters::Stimulation& _parameters) { parameters = _parameters; }
	
	/** @return pointer to calculated stimulation vector */
	std::shared_ptr<const std::vector<uint8_t>> GetVector() const { return vecptr; }
};

}