#include "stdafx.h"
#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "devices/Inputs.h"
#include "devices/InputsDAQmx.h"
#include "devices/InputsFPGA.h"
#include "devices/Outputs.h"
#include "devices/OutputsDAQmx.h"
#include "devices/OutputsDAQmxLineClock.h"
#include "devices/OutputsDAQmxResonance.h"
#include "devices/OutputsDAQmxSlave.h"
#include "gui/DAQmxPage.h"
#include "gui/FPGANoiseOutputPage.h"
#include "gui/FPGAAnalogDemultiplexerPage.h"
#include "gui/FPGAAnalogDemultiplexerResonancePage.h"
#include "gui/FPGAAnalogIntegratorPage.h"
#include "gui/FPGAResonanceScannerPage.h"
#include "gui/FPGAResonanceScanner_NI5771Page.h"
#include "gui/FPGADigitalDemultiplexerPage.h"
#include "gui/FPGAPhotonCounterPage.h"
#include "parameters/Inputs.h"
#include "parameters/Outputs.h"

namespace scope {

void CheckScopeDefines() {
	static_assert( (SCOPE_SCANNERTYPE == ScannerTypeHelper::Mode::Regular) || (SCOPE_SCANNERTYPE == ScannerTypeHelper::Mode::Resonance), "Only regular and resonance scanner type supported right now");

	static_assert( (SCOPE_USE_FPGA) ? std::is_same<SCOPE_INPUTS_T, InputsFPGA>::value : true, "You have to use InputsFPGA if you use an FPGA");

	static_assert( (!SCOPE_USE_FPGA)? !std::is_same<SCOPE_INPUTS_T, InputsFPGA>::value : true, "You cannot use InputsFPGA if you do not use an FPGA");

	static_assert( (SCOPE_NAREAS>1) ? !std::is_same<SCOPE_FPGA_T, FPGAPhotonCounter>::value : true, "You cannot use FPGAPhotonCounter for multiple areas");
	
	static_assert( (SCOPE_NAREAS>1) ? !std::is_same<SCOPE_FPGA_T, FPGAAnalogIntegrator>::value : true, "You cannot use FPGAAnalogIntegrator for multiple areas");

	static_assert( (SCOPE_NAREAS>1) ? !std::is_same<SCOPE_FPGA_T, FPGAResonanceScanner>::value : true, "You cannot use FPGAAnalogIntegrator for multiple areas");

	static_assert( (SCOPE_NAREAS>1) ? !std::is_same<SCOPE_FPGA_T, FPGAResonanceScannerNI5771>::value : true, "You cannot use FPGAAnalogIntegrator for multiple areas");

	static_assert( (SCOPE_NAREAS>1) ? SCOPE_USE_FPGA : true, "You have to use an FPGA for a multi-area configuration");

	static_assert( (std::is_same<SCOPE_FPGA_T, FPGAPhotonCounter>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAPhotonCounter>::value : true, "The inputs parameters type must match the FPGA type");

	static_assert( (std::is_same<SCOPE_FPGA_T, FPGADigitalDemultiplexer>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGADigitalDemultiplexer>::value : true, "The inputs parameters type must match the FPGA type");

	static_assert( (std::is_same<SCOPE_FPGA_T, FPGAAnalogIntegrator>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogIntegrator>::value : true, "The inputs parameters type must match the FPGA type");

	static_assert( (std::is_same<SCOPE_FPGA_T, FPGAResonanceScanner>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAResonanceScanner>::value : true, "The inputs parameters type must match the FPGA type");

	static_assert( (std::is_same<SCOPE_FPGA_T, FPGAResonanceScannerNI5771>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogIntegrator>::value : true, "The inputs parameters type must match the FPGA type");

	static_assert( (std::is_same<SCOPE_FPGA_T, FPGAAnalogDemultiplexer>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogDemultiplexer>::value : true, "The inputs parameters type must match the FPGA type");

	static_assert( (std::is_same<SCOPE_FPGA_T, FPGAAnalogDemultiplexerResonance>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogDemultiplexer>::value : true, "The inputs parameters type must match the FPGA type");
	
	static_assert( (std::is_same<SCOPE_FPGA_T, FPGANoiseOutput>::value&&SCOPE_USE_FPGA) ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGANoiseOutput>::value : true, "The inputs parameters type must match the FPGA type");

	static_assert( !SCOPE_USE_FPGA ? std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsDAQmx>::value : true, "The inputs parameters type must be InputsDAQmx for DAQmx inputs");

	static_assert( (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsDAQmx>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CDAQmxPage>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogDemultiplexer>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGAAnalogDemultiplexerPage>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogDemultiplexer>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGAAnalogDemultiplexerResonancePage>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogIntegrator>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGAAnalogIntegratorPage>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAResonanceScanner>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGAResonanceScannerPage>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAAnalogIntegrator>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGAResonanceScannerNI5771Page>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGADigitalDemultiplexer>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGADigitalDemultiplexerPage>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGAPhotonCounter>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGAPhotonCounterPage>::value)
		|| (std::is_same<parameters::SCOPE_INPUTS_PARAMETERS_T, parameters::InputsFPGANoiseOutput>::value && std::is_same<gui::SCOPE_INPUTSINFOPAGE_T, gui::CFPGANoiseOutputPage>::value), "The inputs information page must match the inputs type");

	static_assert(SCOPE_NAREAS > 0, "Come on!");

	static_assert( (std::is_same<SCOPE_OUTPUTS_T, OutputsDAQmx>::value && (SCOPE_MASTERFRAMEVECTORFILL == ScannerVectorFillTypeHelper::Mode::FullframeXYZP))
		|| (std::is_same<SCOPE_OUTPUTS_T, OutputsDAQmxLineClock>::value && (SCOPE_MASTERFRAMEVECTORFILL == ScannerVectorFillTypeHelper::Mode::LineXPColumnYZ))
		|| (std::is_same<SCOPE_OUTPUTS_T, OutputsDAQmxResonance>::value && (SCOPE_MASTERFRAMEVECTORFILL == ScannerVectorFillTypeHelper::Mode::FullframeXYZP)), "You have to use the correct scanner vector fill type for your choosen Outputs type");

	static_assert( (std::is_same<SCOPE_OUTPUTS_T, OutputsDAQmx>::value && std::is_same<SCOPE_ZEROOUTPUTS_T, ZeroOutputsDAQmx>::value)
		|| (std::is_same<SCOPE_OUTPUTS_T, OutputsDAQmxLineClock>::value && std::is_same<SCOPE_ZEROOUTPUTS_T, ZeroOutputsDAQmxLineClock>::value)
		|| (std::is_same<SCOPE_OUTPUTS_T, OutputsDAQmxResonance>::value && std::is_same<SCOPE_ZEROOUTPUTS_T, ZeroOutputsDAQmxResonance>::value), "You have to use the ZeroOutputs type corresponding to your choosen Outputs type");

	static_assert( std::is_same<SCOPE_SLAVEOUTPUTS_T, OutputsDAQmxSlave>::value && (SCOPE_SLAVEFRAMEVECTORFILL == ScannerVectorFillTypeHelper::Mode::LineZP), "You have to use the correct scanner vector fill type for your choosen slave Outputs type");

	static_assert( (SCOPE_MAXCHANNELS <=4 ) && (SCOPE_MAXCHANNELS > 0), "Buttons etc are only included for up to 4 channels. If you need more, add the appropriate resource to the CChannelFrame menu etc.!");


}

}