#include "stdafx.h"
#include "Plane.h"

namespace scope {

namespace parameters {

// save some typing here...
using namespace boost::property_tree;

PlaneProperties::PlaneProperties(const double& _position, const double& _pockels)
	: position(_position, -1000.0, 1000.0, L"ZPosition_um")
	, pockels(0.0, 0.0, 1.0, L"Pockels_Fraction") {
}

void PlaneProperties::Load(const wptree& pt) {
	position.SetFromPropertyTree(pt);
	pockels.SetFromPropertyTree(pt);
}

void PlaneProperties::Save(wptree& pt) const {
	position.AddToPropertyTree(pt);
	pockels.AddToPropertyTree(pt);
}

}

}