#pragma once

/** @file ScopeDefinesExample.h This is an example file for all  definitions for Scope. You must copy this file as ScopeDefines.h (which is automatically excluded
from git) and adapt to your hardware. */

/* Macro definitions for Scope.
* This allows compile time selection of the number of areas and the specific hardware of the microscope.
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
* On an nbeam setup each area (except the first) is a slave area and has only its FPU movement, Pockels, and fast z axis (ETL). There is only one master area (area 0),
* the others are slaves.
* Pixel acquisition is done in one thread only since pixels for all areas are generated in sync.
* Pixelmapping for all areas is done in one thread only since the lookup vector is by definition identical for all areas.
* Displaying is done in one thread per area, since every area could have different display properties (e.g. channel colors, overlays etc)
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

namespace scope {
	enum class ScannerEnum {
		RegularGalvo,
		ResonantGalvo,
		AOD,
		Fibre
	};
	
	enum class OutputEnum {
		SimpleDAQmx,
		TwoCardDAQmx,
		SimpleDAQmx_Resonance
	};
	
	template<OutputEnum>
	struct OutputTypeSelector {
		typedef OutputsDAQmx type;
		typedef ZeroOutputsDAQmx type_zero;
		typedef OutputsDAQmxSlave type_slave;
		typedef ZeroOutputsDAQmxSlave type_slave_zero;
	}
	
	template<>
	struct OutputTypeSelector<OutputEnum::TwoCardDAQmx> {
		typedef OutputsDAQmxLineClock type;
		typedef ZeroOutputsDAQmxLineClock type_zero;
	}
	
	template<>
	struct OutputTypeSelector<OutputEnum::SimpleDAQmx_Resonance> {
		typedef OutputsDAQmxResonance type;
	}
	
	enum class InputEnum {
		DAQmx,
		FPGA_Photoncounter,
		FPGA_Digitaldemultiplexer,
		FPGA_Analogintegrator,
		FPGA_Analogdemultiplexer,
		FPGA_Resonancescanner
	};
	
	template<InputEnum>
	struct InputTypeSelector {
		typedef InputsDAQmx type;
	};
	
	template<>
	struct InputTypeSelector<InputEnum::FPGA_Photoncounter> {
		typedef InputsFPGAPhotonCounter type;
	};
	
	enum class XYStageEnum {
		None,
		Standa
	};
	
	template<XYStageEnum>
	struct XYStageTypeSelector {
		typedef XYControl type;
	};
	
	template<>
	struct XYStageTypeSelector<XYStageEnum::Standa> {
		typedef XYControlStanda type;
	};
	
	enum class ZStageEnum {
		None,
		ETL
	};
	
	template<ZStageEnum>
	struct ZStageTypeSelector {
		typedef FastZControl type;
	};
	
	template<>
	struct ZStageTypeSelector<ZStageEnum::ETL> {
		typedef FastZControlETL type;
	};
	
	enum class XYZStageEnum {
		None,
		Galil,
		Sutter
	};
	
	template<XYZStageEnum>
	struct XYZStageSelector {
		typdedef XYZControl type;
	};
	
	template<>
	struct XYZStageSelector<XYZStageEnum::Galil> {
		typdedef XYZControlGalil type;
	};
	
	template<>
	struct XYZStageSelector<XYZStageEnum::Sutter> {
		typdedef XYZControlSutter type;
	};
	
	enum class DaqMultiChunkEnum {
		Regular,
		Resonance
	}
	
	template <DaqMultiChunkEnum>
	struct DaqMultiChunkSelector {
		typedef DaqMultiChunk type;
		typedef DaqMultiChunkPtr type_ptr;
	};
	
	template<>
	struct DaqMultiChunkSelector<DaqMultiChunkEnum::Resonance> {
		typedef DaqMultiChunkResonance type;
		typedef DaqMultiChunkResonancePtr type_ptr;
	};
	
	enum class NBeamSetupEnum {
		SingleBeam,
		MultiBeam
	};
	
	template<NBeamSetupEnum, uint32_t AREAS>
	struct NBeamSetupSelector {
		constexpr static uint32_t areas = 1;
		constexpr static uint32_t slaves = 0;
	};
	
	template<uint32_t AREAS>
	struct NBeamSetupSelector<NBeamSetupEnum::MultiBeam, AREAS> {
		static_assert(AREAS > 1, "MultiBeamSetup: Zahl der Areas muss größer als 1 sein.");
		constexpr static uint32_t areas = AREAS;
		constexpr static uint32_t slaves = AREAS - 1;
	};
	
	constexpr uint32_t config_nareas = 1;
	constexpr NBeamSetupEnum config_nbeamsetup = NBeamSetupEnum::SingleBeam;
	constexpr uint32_t config_nslaveareas = NBeamSetupSelector<config_nbeamsetup, config_nareas>::slaves;
	constexpr ScannerEnum config_scannerselect = ScannerEnum::RegularGalvo;
	constexpr OutputEnum config_outputselect = OutputEnum::SimpleDAQmx;
	constexpr InputEnum config_inputselect = InputEnum::DAQmx;
	constexpr XYStageEnum config_xystageselect = XYStageEnum::None;
	constexpr ZStageEnum config_zstageselect = ZStageEnum::None;
	constexpr XYZStageEnum config_xyzstageselect = XYZStageEnum::None;
	constexpr DaqMultiChunkEnum config_daqmultichunk = DaqMultiChunkEnum::Regular;
	
	typedef OutputSelector<config_outputselect>::type MyOutputType;
	typedef OutputSelector<config_outputselect>::type MyOutputZeroType;
	typedef OutputSelector<config_outputselect>::type_slave MySlaveOutputType;
	typedef InputTypeSelector<config_inputselect>::type MyInputType;
	typedef XYStageTypeSelector<config_xystageselect>::type MyXYStageType;
	typedef ZStageTypeSelector<config_zstageselect>::type MyZStageType;
	typedef XYZStageTypeSelector<config_xyzstageselect>::type MyXYZStage;
	typedef DaqMultiChunkSelector<config_daqmultichunk>::type MyDaqMultiChunkType;
	typedef DaqMultiChunkSelector<config_daqmultichunk>::type_ptr MyDaqMultiChunkPtrType;
}

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
#define SCOPE_DAQMULTICHUNK_T					DaqMultiChunk<1>
#define SCOPE_DAQMULTICHUNKPTR_T				DaqMultiChunkPtr<1>
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
		#undef SCOPE_DAQMULTICHUNK_T
		#define SCOPE_DAQMULTICHUNK_T			DaqMultiChunkResonance<1>
		#undef SCOPE_DAQMULTICHUNKPTR_T
		#define SCOPE_DAQMULTICHUNKPTR_T		DaqMultiChunkResonancePtr<1>
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
	#define SCOPE_NBEAM_AREAS					SCOPE_NAREAS
	#define SCOPE_NBEAM_DAQS					1
	#define SCOPE_NBEAM_PIPELINES				1
	#define SCOPE_NBEAM_STORAGES				SCOPE_NAREAS
	#define SCOPE_NBEAM_DISPLAYS				SCOPE_NAREAS
	#undef SCOPE_DAQMULTICHUNK_T
	#define SCOPE_DAQMULTICHUNK_T				DaqMultiChunkResonance<SCOPE_NBEAM_AREAS>
	#undef SCOPE_DAQMULTICHUNKPTR_T
	#define SCOPE_DAQMULTICHUNKPTR_T			DaqMultiChunkResonancePtr<SCOPE_NBEAM_AREAS>
#else
	#define SCOPE_NBEAM_SLAVES					0
	#define SCOPE_NBEAM_AREAS					1
	#define SCOPE_NBEAM_DAQS					SCOPE_NAREAS
	#define SCOPE_NBEAM_PIPELINES				SCOPE_NAREAS
	#define SCOPE_NBEAM_STORAGES				SCOPE_NAREAS
	#define SCOPE_NBEAM_DISPLAYS				SCOPE_NAREAS
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
	
	typedef DaqMultiChunk<SCOPE_NAREAS, uint16_t> ScopeDaqMultiChunk;
	
	/** @return true if area _a is a slave area */
	template<class C> bool ThisIsSlaveArea(const C& c, const typename C::iterator& i) {
		#ifdef SCOPE_NBEAM_SETUP
				return i != std::begin(c);
		#else
				return false;
		#endif
	}

	/** @return true if area _a is a slave area */
	bool ThisIsSlaveArea(const uint32_t& _a);

	/** @return the master area 0 (if nbeam setup) or the area _a */
	template<class C> typename C::iterator ThisAreaOrMasterArea(const C& c, const typename C::iterator& i) {
		return ThisIsSlaveArea(c, i) ? std::begin(c) : i;
	}

	/** @return the master area 0 (if nbeam setup) or the area _a */
	uint32_t ThisAreaOrMasterArea(const uint32_t& _a);

	/** Checks some basic stuff with defines in ScopeDefines.h. Does static_assert to warn you at compile time that you configured something wrong */
	void CheckScopeDefines();

}