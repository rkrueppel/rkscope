#pragma once

#include "config_options.h"

namespace scope {
	
	namespace config {
		constexpr uint32_t nareas = 1;
		constexpr std::array<bool, nareas> masterarea {{true}}; // e.g. {{true, false}}
		constexpr std::array<bool, nareas> slavearea {{false}}; // e.g. {{false, true}}
		constexpr uint32_t nmasters = 1; // _counttrue(masterarea);
		constexpr uint32_t nslaves = 0; // _counttrue(slavearea);
		constexpr uint32_t areaspermaster = 1; // i.e. the master area itself
		constexpr NBeamSetupEnum nbeamsetup = NBeamSetupEnum::SingleBeam; // SingleBeam, MultiBeam
		constexpr ScannerEnum scannerselect = ScannerEnum::RegularGalvo; // RegularGalvo, ResonantGalvo, AOD, Fibre
		constexpr OutputEnum outputselect = OutputEnum::SimpleDAQmx; // SimpleDAQmx, TwoCardDAQmx, SimpleDAQmx_Resonance
		constexpr uint32_t nchannels = 2;
		typedef uint16_t daqdatatype;
		constexpr InputEnum inputselect = InputEnum::DAQmx; //DAQmx, FPGA_NoiseOutput, FPGA_Photoncounter, FPGA_Digitaldemultiplexer, FPGA_Analogintegrator, FPGA_Analogdemultiplexer, FPGA_Resonancescanner, FPGA_ResonancescannerNI5771
		constexpr DaqChunkEnum daqchunkselect = DaqChunkEnum::Regular; // Regular, Resonance
		constexpr FPUXYStageEnum fpuxystageselect = FPUXYStageEnum::None; // None, Standa
		constexpr FPUZStageEnum fpuzstageselect = FPUZStageEnum::None; // None, ETL
		constexpr XYZStageEnum xyzstageselect = XYZStageEnum::None; // None, Galil, Sutter
		constexpr StimulationsEnum stimulations = StimulationsEnum::DAQmx; // DAQmx
		constexpr MultiImageEnum multiimage = MultiImageEnum::Regular; // Regular, ResonanceSW
		constexpr OverlayEnum overlay = OverlayEnum::Regular; // Regular, ResonanceSW
		constexpr ResonancePixelmapperEnum resonancepixelmapper = ResonancePixelmapperEnum::Hardware; // Hardware, Software
		
		constexpr FramevectorFillEnum framevectorfill_master = FramevectorFillSelector<outputselect>::fill_master;
		constexpr FramevectorFillEnum framevectorfill_slave = FramevectorFillSelector<outputselect>::fill_slave;

		/* Number of threads in the controllers */
		constexpr uint32_t threads_daq = nmasters;
		constexpr uint32_t threads_pipeline = nmasters;
		constexpr uint32_t threads_display = nareas;
		constexpr uint32_t threads_storage = nareas;
		
		/* Maximum number of channels supported by Scope. You can have more if you add buttons etc etc. to e.g. CChannelFrame */
		constexpr uint32_t maxchannels = 4;

		typedef uint16_t daqdatatype;
		
		typedef OutputTypeSelector<outputselect>::type OutputType;
		typedef OutputTypeSelector<outputselect>::type_zero OutputZeroType;
		typedef OutputTypeSelector<outputselect>::type_parameters OutputParametersType;
		typedef OutputTypeSelector<outputselect>::type_slave SlaveOutputType;
		typedef OutputTypeSelector<outputselect>::type_slave_zero SlaveOutputZeroType;
		typedef OutputTypeSelector<outputselect>::type_slave_parameters SlaveOutputParametersType;
		typedef InputTypeSelector<inputselect>::type InputType;
		typedef InputTypeSelector<inputselect>::type_parameters InputParametersType;
		typedef InputTypeSelector<inputselect>::type_fpga InputFPGAType;
		typedef DaqChunkTypeSelector<daqchunkselect>::type<nchannels, nareas, daqdatatype> DaqChunkType;
		typedef DaqChunkTypeSelector<daqchunkselect>::type_ptr<nchannels, nareas, daqdatatype> DaqChunkPtrType;
 		typedef FPUXYStageTypeSelector<fpuxystageselect>::type FPUXYStageType;
		typedef FPUXYStageTypeSelector<fpuxystageselect>::type_parameters FPUXYStageParametersType;
		typedef FPUZStageTypeSelector<fpuzstageselect>::type FPUZStageType;
		typedef FPUZStageTypeSelector<fpuzstageselect>::type_parameters FPUZStageParametersType;
		typedef XYZStageTypeSelector<xyzstageselect>::type XYZStageType;
		typedef XYZStageTypeSelector<xyzstageselect>::type_parameters XYZStageParametersType;
		typedef StimulationsTypeSelector<stimulations>::type StimulationsType;
		typedef MultiImageTypeSelector<multiimage>::type MultiImageType;
		typedef MultiImageTypeSelector<multiimage>::type_ptr MultiImagePtrType;
		typedef MultiImageTypeSelector<multiimage>::type_constptr MultiImageCPtrType;
		typedef OverlayTypeSelector<overlay>::type OverlayType;
		typedef ResonancePixelmapperTypeSelector<resonancepixelmapper, areaspermaster>::type ResonancePixelmapperType;
		
		/** @return the masterarea of _a, returns _a if _a is master, assumes area 0 is always a master */
		constexpr uint32_t MyMaster(const uint32_t& _a) {
			for ( uint32_t a = _a; a > 0 ; a-- ) 
				if (masterarea[a]) return a;
			return 0;
		}

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
		
	}
}