#pragma once

#include "Base.h"
#include "helpers\ScopeNumber.h"

namespace scope {

	namespace parameters {

/** Parameters of a single plane for fast plane hopping/switching
* @ingroup ScopeParameters */
class PlaneProperties
	: public Base {

public:
	/** z position (um) */
	ScopeNumber<double> position;

	/** Pockels/intensity value (Volts) for that plane */
	ScopeNumber<double> pockels;

	/** Quick construct */
	PlaneProperties(const double& _position = 0.0, const double& _pockels = 0.0);

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

}

}