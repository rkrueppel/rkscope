#pragma once

namespace scope {
	class OutputsDAQmx;
	class ZeroOutputsDAQmx;
	class OutputsDAQmxSlave;
	class ZeroOutputsDAQmxSlave;
	class OutputsDAQmxLineClock;
	class ZeroOutputsDAQmxLineClock;
	class OutputsDAQmxResonance;
	class InputsDAQmx;
	class InputsFPGAPhotonCounter;
	class XYControl;
	class XYControlStanda;
	class FastZControl;
	class FastZControlETL;
	class XYZControl;
	class XYZControlGalil;
	class XYZControlSutter;
	template<uint32_t NAREAS, class DATA_T> class DaqMultiChunk;
	template<uint32_t NAREAS, class DATA_T> using DaqMultiChunkPtr = std::shared_ptr<DaqMultiChunk<NAREAS, DATA_T>>;

	template<uint32_t NAREAS, class DATA_T>class DaqMultiChunkResonance;
	template<uint32_t NAREAS, class DATA_T> using DaqMultiChunkResonancePtr = std::shared_ptr<DaqMultiChunkResonance<NAREAS, DATA_T>>;

	namespace config {
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
		};

		template<>
		struct OutputTypeSelector<OutputEnum::TwoCardDAQmx> {
			typedef OutputsDAQmxLineClock type;
			typedef ZeroOutputsDAQmxLineClock type_zero;
		};

		template<>
		struct OutputTypeSelector<OutputEnum::SimpleDAQmx_Resonance> {
			typedef OutputsDAQmxResonance type;
		};

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
		struct XYZStageTypeSelector {
			typedef XYZControl type;
		};

		template<>
		struct XYZStageTypeSelector<XYZStageEnum::Galil> {
			typedef XYZControlGalil type;
		};

		template<>
		struct XYZStageTypeSelector<XYZStageEnum::Sutter> {
			typedef XYZControlSutter type;
		};

		enum class DaqMultiChunkEnum {
			Regular,
			Resonance
		};

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

		typedef OutputTypeSelector<config_outputselect>::type MyOutputType;
		typedef OutputTypeSelector<config_outputselect>::type MyOutputZeroType;
		typedef OutputTypeSelector<config_outputselect>::type_slave MySlaveOutputType;
		typedef InputTypeSelector<config_inputselect>::type MyInputType;
		typedef XYStageTypeSelector<config_xystageselect>::type MyXYStageType;
		typedef ZStageTypeSelector<config_zstageselect>::type MyZStageType;
		typedef XYZStageTypeSelector<config_xyzstageselect>::type MyXYZStage;
		typedef DaqMultiChunkSelector<config_daqmultichunk>::type MyDaqMultiChunkType;
		typedef DaqMultiChunkSelector<config_daqmultichunk>::type_ptr MyDaqMultiChunkPtrType;
	}
}
