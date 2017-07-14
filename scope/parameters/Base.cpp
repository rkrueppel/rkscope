#include "stdafx.h"
#include "Base.h"

namespace scope {
	namespace parameters {

Base::~Base() {
	// This disconnects all connections. There is still one glitch, since the derived class destructor is called first
	// derived members are already destructed before we call disconnect here
	for ( auto& c : connection_collector )
		c.disconnect();
}

	}
}