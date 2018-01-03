#include "stdafx.h"
#include "config_choices.h"

namespace scope {
	
	namespace config {
		
		// Do some compile time checks for correct configuration
		static_assert( (scannerselect == ScannerEnum::RegularGalvo) || (scannerselect == ScannerEnum::ResonantGalvo), "Only regular and resonance galvos are supported right now.");
		static_assert( (totalareas>1) ? !std::is_same<InputFPGAType, FPGAPhotonCounter>::value : true, "You cannot use FPGAPhotonCounter for multiple areas");
		static_assert( (totalareas>1) ? !std::is_same<InputFPGAType, FPGAAnalogIntegrator>::value : true, "You cannot use FPGAAnalogIntegrator for multiple areas");
		static_assert( maxchannels > 0, "Come on...");
	}
}
