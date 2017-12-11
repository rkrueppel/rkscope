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
	class ScopeMultiImage;
	class ScopeMultiImageResonanceSW;
	typedef std::shared_ptr<ScopeMultiImage> ScopeMultiImagePtr;
	typedef std::shared_ptr<const ScopeMultiImage> ScopeMultiImageCPtr;
	typedef std::shared_ptr<ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWPtr;
	typedef std::shared_ptr<const ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWCPtr;

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
		struct DaqMultiChunkTypeSelector {
			typedef DaqMultiChunk type;
			typedef DaqMultiChunkPtr type_ptr;
		};

		template<>
		struct DaqMultiChunkTypeSelector<DaqMultiChunkEnum::Resonance> {
			template<uint32_t NAREAS, class DATA_T> using DaqMultiChunkResonance = type;
			template<uint32_t NAREAS, class DATA_T> using DaqMultiChunkResonancePtr = type_ptr;
		};
		
		enum class StimulationsEnum {
			DAQmx
		};
		
		template <StimulationEnum>
		struct StimulationsTypeSelector {
			typedef StimulationsDAQmx type;
		};
		
		enum class MultiImageEnum {
			Regular,
			ResonanceSW
		};
		
		template <MultiImageEnum>
		struct MultiImageTypeSelector {
			typedef ScopeMultiImage type;
			typedef ScopeMultiImagePtr type_ptr;
			typedef ScopeMultiImageCPtr type_constptr;
		};
		
		template<>
		struct MultiImageTypeSelector<MultiImageEnum::ResonanceSW> {
			typedef ScopeMultiImageResonanceSW type;
			typedef ScopeMultiImageResonanceSWPtr type_ptr;
			typedef ScopeMultiImageResonanceSWCPtr type_constptr;
		};
		
		enum class OverlayEnum {
			Regular,
			ResonanceSW
		};
		
		template <OverlayEnum>
		struct OverlayTypeSelector {
			typedef ScopeOverlay type;
		};
		
		template<>
		struct OverlayTypeSelector<OverlayEnum::ResonanceSW> {
			typedef ScopeOverlayResonanceSW type;
		};

		enum class ResonancePixelmapperEnum {
			Software,
			Hardware
		};

		template<ResonancePixelmapperEnum>
		struct ResonancePixelmapperTypeSelector {
			typedef PixelmapperFrameResonanceSW type;
		};

		template<>
		struct ResonancePixelmapperTypeSelector<ResonancePixelmapperEnum::Hardware> {
			typdef PixelmapperFrameResonanceHW type;
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
			static_assert(AREAS > 1, "MultiBeamSetup: Number of areas has to be bigger than one.");
			constexpr static uint32_t areas = AREAS;
			constexpr static uint32_t slaves = AREAS - 1;
		};



		
	}
}
