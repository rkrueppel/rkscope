#pragma once

/** @file ScopeDefinesExample.h This is an example file for all macro definitions for Scope. You must copy this file as ScopeDefines.h (which is automatically excluded
from git) and adapt to your hardware. */

/* Macro definitions for Scope.
* This allows compile time selection of the number of areas and the specific hardware of the microscope.
* This approach is simpler than a factory pattern.
* If you mess stuff up here you will run into severe problems!!!! */

/* Number of areas compiled for */
#define SCOPE_NAREAS				1

// The type of scanner you are using
// Choose ONE option
#define SCOPE_USE_REGULAR_GALVO_SCANNER
//#define SCOPE_USE_RESONANT_GALVO_SCANNER
//#define SCOPE_USE_AOD_SCANNER
//#define SCOPE_USE_FIBER_SCANNER

// Type of pixel mapping for a resonance system, choose only ONE option, ONLY if you chose SCOPE_USE_RESONANT_GALVO_SCANNER above
//#define SCOPE_RESONANCE_SOFTWARE_MAPPING
//#define SCOPE_RESONANCE_HARDWARE_MAPPING

/* Do you have an n-beam multi-area setup with one scanner but multiple areas, then define.
* On an nbeam setup aach area (except the first) is a slave area and has only its FPU movement, Pockels, and fast z axis (ETL). There is only one master area (area 0),
* the others are slaves.
* @warning in your configuration file, make sure the first area is the master area (set IsSlaveArea to false) and the other are slaves (set IsSlaveArea to true)! */
//#define SCOPE_NBEAM_SETUP

// Choose ONE option
#define SCOPE_USE_SIMPLEDAQMX_OUTPUTS
//#define SCOPE_USE_TWOCARDDAQMX_OUTPUTS
//#define SCOPE_USE_SIMPLEDAQMX_RESONANCE_OUTPUTS

// Choose ONE option
#define SCOPE_USE_DAQMX_INPUTS
//#define SCOPE_USE_FPGAPHOTONCOUNTER_INPUTS
//#define SCOPE_USE_FPGADIGITALDEMULTIPLEXER_INPUTS
//#define SCOPE_USE_FPGAANALOGINTEGRATOR_INPUTS
//#define SCOPE_USE_FPGAANALOGDEMULTIPLEXER_INPUTS
//#define SCOPE_USE_FPGARESONANCESCANNER_INPUTS

// Choose ONE option
#define SCOPE_USE_NOXYFPUSTAGE
//#define SCOPE_USE_STANDA_XYFPUSTAGE

// Choose ONE option
#define SCOPE_USE_NOZFPUSTAGE
//#define SCOPE_USE_ETLZFPUSTAGE

// Choose ONE option
#define SCOPE_USE_NO_XYZSTAGE
//#define SCOPE_USE_GALIL_XYZSTAGE
//#define SCOPE_USE_SUTTER_XYZSTAGE

// Do you have a resonance scanner in your setup, then define.
#define SCOPE_USE_RESONANCESCANNER		false


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Do not tinker around with the following stuff, unless you really know what you are doing
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default definitions
#define SCOPE_DAQCHUNKPTR_T						DaqChunkPtr
#define SCOPE_RESONANCEPIXELMAPPER_T			PixelmapperFrameResonanceHW
#define SCOPE_MULTIIMAGE_T						ScopeMultiImage
#define SCOPE_MULTIIMAGEPTR_T					ScopeMultiImagePtr
#define SCOPE_OVERLAY_T							ScopeOverlay

#ifdef SCOPE_USE_REGULAR_GALVO_SCANNER
	#define SCOPE_SCANNERTYPE					ScannerTypeHelper::Mode::Regular
#endif

#ifdef SCOPE_USE_RESONANT_GALVO_SCANNER
	#define SCOPE_SCANNERTYPE					ScannerTypeHelper::Mode::Resonance
	#ifdef SCOPE_RESONANCE_SOFTWARE_MAPPING
		#undef SCOPE_DAQCHUNKPTR_T
		#define SCOPE_DAQCHUNKPTR_T				DaqChunkResonancePtr
		#undef SCOPE_MULTIIMAGE_T
		#define SCOPE_MULTIIMAGE_T				ScopeMultiImageResonanceSW
		#undef SCOPE_MULTIIMAGEPTR_T
		#define SCOPE_MULTIIMAGEPTR_T			ScopeMultiImageResonanceSWPtr
		#undef SCOPE_OVERLAY_T
		#define SCOPE_OVERLAY_T					ScopeOverlayResonanceSW
		#undef SCOPE_RESONANCEPIXELMAPPER_T
		#define SCOPE_RESONANCEPIXELMAPPER_T	PixelmapperFrameResonanceSW
	#elif SCOPE_RESONANCE_HARDWARE_MAPPING
		#dundef SCOPE_RESONANCEPIXELMAPPER_T
		#define SCOPE_RESONANCEPIXELMAPPER_T	PixelmapperFrameResonanceHW
	#endif
#endif

#ifdef SCOPE_USE_AOD_SCANNER
	#define SCOPE_SCANNERTYPE					ScannerTypeHelper::Mode::AOD
#endif

#ifdef SCOPE_USE_FIBER_SCANNER
	#define SCOPE_SCANNERTYPE					ScannerTypeHelper::Mode::Fiber
#endif

#ifdef SCOPE_USE_SIMPLEDAQMX_OUTPUTS
	#define SCOPE_OUTPUTS_T						OutputsDAQmx
	#define SCOPE_SLAVEOUTPUTS_T				OutputsDAQmxSlave
	#define SCOPE_ZEROOUTPUTS_T					ZeroOutputsDAQmx
	#define SCOPE_ZEROOUTPUTSSLAVE_T			ZeroOutputsDAQmxSlave
	#define SCOPE_MASTERFRAMEVECTORFILL			ScannerVectorFillTypeHelper::Mode::FullframeXYZP
	#define SCOPE_SLAVEFRAMEVECTORFILL			ScannerVectorFillTypeHelper::Mode::LineZP
#endif

#ifdef SCOPE_USE_TWOCARDDAQMX_OUTPUTS
	#define SCOPE_OUTPUTS_T						OutputsDAQmxLineClock
	#define SCOPE_SLAVEOUTPUTS_T				OutputsDAQmxSlave
	#define SCOPE_ZEROOUTPUTS_T					ZeroOutputsDAQmxLineClock
	#define SCOPE_ZEROOUTPUTSSLAVE_T			ZeroOutputsDAQmxSlave
	#define SCOPE_MASTERFRAMEVECTORFILL			ScannerVectorFillTypeHelper::Mode::LineXPColumnYZ
	#define SCOPE_SLAVEFRAMEVECTORFILL			ScannerVectorFillTypeHelper::Mode::LineZP
#endif

#ifdef SCOPE_USE_DAQMX_INPUTS
	#define SCOPE_NAREAS						1
	#define SCOPE_USE_FPGA						FALSE
	#define SCOPE_INPUTS_T						InputsDAQmx
	#define SCOPE_INPUTS_PARAMETERS_T			InputsDAQmx
	#define SCOPE_FPGA_T						FPGANoiseOutput
	#define SCOPE_INPUTSINFOPAGE_T				CDAQmxPage
#endif

#ifdef SCOPE_USE_FPGAPHOTONCOUNTER_INPUTS
	#define SCOPE_NAREAS						1
	#define SCOPE_USE_FPGA						TRUE
	#define SCOPE_INPUTS_T						InputsFPGA
	#define SCOPE_INPUTS_PARAMETERS_T			InputsFPGAPhotonCounter
	#define SCOPE_FPGA_T						FPGAPhotonCounter
	#define SCOPE_INPUTSINFOPAGE_T				CFPGAPhotonCounterPage
#endif

#ifdef SCOPE_USE_FPGADIGITALDEMULTIPLEXER_INPUTS
	#define SCOPE_NAREAS						2
	#define SCOPE_USE_FPGA						TRUE
	#define SCOPE_INPUTS_T						InputsFPGA
	#define SCOPE_INPUTS_PARAMETERS_T			InputsFPGADigitalDemultiplexer
	#define SCOPE_FPGA_T						FPGADigitalDemultiplexer
	#define SCOPE_INPUTSINFOPAGE_T				CFPGADigitalDemultiplexerPage
#endif

#ifdef SCOPE_USE_FPGAANALOGINTEGRATOR_INPUTS
	#define SCOPE_NAREAS						1
	#define SCOPE_USE_FPGA						TRUE
	#define SCOPE_INPUTS_T						InputsFPGA
	#define SCOPE_INPUTS_PARAMETERS_T			InputsFPGAAnalogIntegrator
	#define SCOPE_FPGA_T						FPGAAnalogIntegrator
	#define SCOPE_INPUTSINFOPAGE_T				CFPGAAnalogIntegratorPage
#endif

#ifdef SCOPE_USE_FPGAANALOGDEMULTIPLEXER_INPUTS
	#define SCOPE_NAREAS						2
	#define SCOPE_USE_FPGA						TRUE
	#define SCOPE_INPUTS_T						InputsFPGA
	#define SCOPE_INPUTS_PARAMETERS_T			InputsFPGAAnalogDemultiplexer
	#define SCOPE_FPGA_T						FPGAAnalogDemultiplexer
	#define SCOPE_INPUTSINFOPAGE_T				CFPGAAnalogDemultiplexerPage
#endif

#ifdef SCOPE_USE_STANDA_XYFPUSTAGE
	#define SCOPE_FPUXYCONTROL_T				XYControlStanda
#endif

#ifdef SCOPE_USE_NOXYFPUSTAGE
	#define SCOPE_FPUXYCONTROL_T				XYControl
#endif

#ifdef SCOPE_USE_ETLZFPUSTAGE
	#define SCOPE_FPUZCONTROL_T					FastZControlETL
#endif

#ifdef SCOPE_USE_NOZFPUSTAGE
	#define SCOPE_FPUZCONTROL_T					FastZControl
#endif

#ifdef SCOPE_USE_GALIL_XYZSTAGE
	#define SCOPE_XYZCONTROL_T					XYZControlGalil
#endif

#ifdef SCOPE_USE_SUTTER_XYZSTAGE
	#define SCOPE_XYZCONTROL_T					XYZControlSutter
#endif

#ifdef SCOPE_USE_NO_XYZSTAGE
	#define SCOPE_XYZCONTROL_T					XYZControl
#endif

#define SCOPE_STIMULATIONS_T					StimulationsDAQmx

#ifdef SCOPE_NBEAM_SETUP
	#define SCOPE_NBEAM_SLAVES					(SCOPE_NAREAS-1)
#else
	#define SCOPE_NBEAM_SLAVES					0
#endif

/* Maximum number of channels supported. You can have more if you add buttons etc etc. to e.g. CChannelFrame */
#define SCOPE_MAXCHANNELS						4

/* Macro definitions for debugging Scope */

/* set to true if you installed the Direct2D debug layer and want to get debug output in debug builds.
* See (http://msdn.microsoft.com/en-us/library/ee794278%28v=vs.85%29.aspx) and (http://archive.msdn.microsoft.com/Direct2DTools ) */
#define SCOPE_DIRECT2D_DEBUG					true

/* If defined true the DAQmx functions throw exceptions on errors
* If false only debug output */
#define DAQMX_THROW_EXCEPTION					false

/* User defined messages  */

/* used by CChannelFrame and CColorList */
#define WM_COLORSELECTED						WM_APP + 1

/* used by CChannelFrame and CChannelView to update frame's statusbar while cursor is moved over view's ScopeOverlay image */
#define WM_UPDATEMOUSEPIXEL						WM_APP + 2

/* used by CHistogramFrame and CHistogramView to update frame's statusbar while view's limit bars are moved */
#define WM_UPDATEHISTOLIMITS					WM_APP + 3

/* used by ScopeEditCtrl to do the update of the control on the screen asynchronously */
#define WM_UPDATECONTROL						WM_APP + 4

/** This is the main namespace for all Scope stuff */
namespace scope {

/** @return true if area _a is a slave area */
bool ThisIsSlaveArea(const uint32_t& _a);

/** @return the master area 0 (if nbeam setup) or the area _a */
uint32_t ThisAreaOrMasterArea(const uint32_t& _a);

/** Checks some basic stuff with defines in ScopeDefines.h. Does static_assert to warn you at compile time that you configured something wrong */
void CheckScopeDefines();

}