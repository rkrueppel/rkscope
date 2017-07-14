#pragma once

#include "ScopeDatatypes.h"

namespace scope {

/** all parameters live in this namespace */
namespace parameters {

using boost::property_tree::wptree;

/** @defgroup ScopeParameters Parameter classes
* Every controller or hardware component uses parameter classes, class ScopeParameters incorporates
* all parameters needed to run the complete scope */


/** All parameter classes derive from this. Keeps a group of related parameters (ScopeValues) together, handles load and save
* from a boost::ptree, and cares about enabling/disabling during scanning.
* @warning no spaces in names allowed (xml saving...)
* @ingroup ScopeParameters */
class Base {
protected:
	/** You can put connections between ScopeValues in different classes in here.
	* The base class destructor calls disconnect on all of them.
	* Connections between ScopeValues of the same class do not have to go in there, once the members are destroyed the connection is
	* gone anyway. */
	std::vector<boost::signals2::connection> connection_collector;

public:
	virtual ~Base();

	/** load parameters from a boost::property_tree */
	virtual void Load(const wptree& pt) {}

	/** save parameters into a boost:property_tree */
	virtual void Save(wptree& pt) const {}

	/** set values that must not be changed to read-only during scanning. True = readonly, false = read&write */
	virtual void SetReadOnlyWhileScanning(const RunState& _runstate) {}
};

}

}