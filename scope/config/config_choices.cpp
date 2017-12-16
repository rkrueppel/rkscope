#include "stdafx.h"
#include "config_choices.h"

namespace scope {
	
	namespace config {
		
		// Do some compile time checks for correct configuration
		static_assert(masterarea[0], "Area 0 has to be a master.");
		static_assert(!slavearea[0], "Area 0 cannot be slave.");
		static_assert(nareas == (nmasters + nslaves), "Number of areas has to be equal number of master plus number of slaves.");
		//static_assert(!_msareaconflict(masterarea, slavearea), "The master and slave area arrays are conflicting.");
		static_assert( (scannerselect == ScannerEnum::RegularGalvo) || (scannerselect == ScannerEnum::ResonantGalvo), "Only regular and resonance galvos are supported right now.");
		static_assert( (nareas>1) ? !std::is_same<InputFPGAType, FPGAPhotonCounter>::value : true, "You cannot use FPGAPhotonCounter for multiple areas");
		static_assert( (nareas>1) ? !std::is_same<InputFPGAType, FPGAAnalogIntegrator>::value : true, "You cannot use FPGAAnalogIntegrator for multiple areas");
		static_assert( maxchannels > 0, "Come on...");
	}
}
