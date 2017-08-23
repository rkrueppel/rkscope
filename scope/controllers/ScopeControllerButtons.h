#pragma once
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeButton.h"
#include "helpers/ScopeScanModeButton.h"

namespace scope {
	/** Keeps the buttons controlling FPU movements together. */
	struct FPUButtons {
		/** @name the cardinal movement directions and some other helpful buttons
		* @{ */
		ScopeButton left;
		ScopeButton right;
		ScopeButton up;
		ScopeButton down;
		ScopeButton setzero;
		/** @} */
	};

	struct RunButtons {
		ScopeButton startsingle;
		ScopeButton startlive;
		ScopeButton startstack;
		ScopeButton starttimeseries;
		ScopeButton startbehavior;
		ScopeButton stop;
		ScopeButton quit;
	};

	struct StackButtons {
		ScopeButton starthere;
		ScopeButton stophere;
	};

	/** Keeps the buttons controlling scan modes together. */
	struct ScanModeButtons {
		/** A map with buttons for all scan modes */
		std::map<ScannerVectorTypeHelper::Mode, ScopeScanModeButton> map;

		/** Fill the map */
		ScanModeButtons() {
			map.emplace(ScannerVectorTypeHelper::Mode::Bidirectional, ScopeScanModeButton(ScannerVectorTypeHelper::Mode::Bidirectional));
			map.emplace(ScannerVectorTypeHelper::Mode::LineStraight, ScopeScanModeButton(ScannerVectorTypeHelper::Mode::LineStraight));
			map.emplace(ScannerVectorTypeHelper::Mode::Planehopper, ScopeScanModeButton(ScannerVectorTypeHelper::Mode::Planehopper));
			map.emplace(ScannerVectorTypeHelper::Mode::ResonanceBiDi, ScopeScanModeButton(ScannerVectorTypeHelper::Mode::ResonanceBiDi));
			map.emplace(ScannerVectorTypeHelper::Mode::ResonanceHopper, ScopeScanModeButton(ScannerVectorTypeHelper::Mode::ResonanceHopper));
			map.emplace(ScannerVectorTypeHelper::Mode::Sawtooth, ScopeScanModeButton(ScannerVectorTypeHelper::Mode::Sawtooth));
		}
	};
}