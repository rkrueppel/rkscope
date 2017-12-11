#pragma once

#include "config_options.h"

namespace scope {
	
	namespace config {
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
		constexpr StimulationsEnum config_stimulations = StimulationsEnum::DAQmx;
		constexpr MultiImageEnum config_multiimage = MultiImageEnum::Regular; // ResonanceSW
		constexpr OverlayEnum config_overlay = OverlayEnum::Regular;

		typedef OutputTypeSelector<config_outputselect>::type MyOutputType;
		typedef OutputTypeSelector<config_outputselect>::type MyOutputZeroType;
		typedef OutputTypeSelector<config_outputselect>::type_slave MySlaveOutputType;
		typedef InputTypeSelector<config_inputselect>::type MyInputType;
		typedef XYStageTypeSelector<config_xystageselect>::type MyXYStageType;
		typedef ZStageTypeSelector<config_zstageselect>::type MyZStageType;
		typedef XYZStageTypeSelector<config_xyzstageselect>::type MyXYZStage;
		typedef DaqMultiChunkTypeSelector<config_daqmultichunk>::type MyDaqMultiChunkType;
		typedef DaqMultiChunkTypeSelector<config_daqmultichunk>::type_ptr MyDaqMultiChunkPtrType;
		typedef StimulationsTypeSelector<config_stimulations>::type MyStimulationsType;
		typedef MultiImageTypeSelector<config_multiimage>::type MyMultiImageType;
		typedef OverlayTypeSelector<config_overlay>::type MyOverlayType;
	}
}