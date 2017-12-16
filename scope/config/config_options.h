#pragma once

namespace scope {
	class OutputsDAQmx;
	class ZeroOutputsDAQmx;
	class OutputsDAQmxSlave;
	class ZeroOutputsDAQmxSlave;
	class OutputsDAQmxLineClock;
	class ZeroOutputsDAQmxLineClock;
	class OutputsDAQmxResonance;
	class ZeroOutputsDAQmxResonance;
	class OutputsDAQmxResonanceSlave;
	class ZeroOutputsDAQmxResonanceSlave;
	class InputsDAQmx;
	class InputsFPGA;
	class FPGANoiseOutput;
	class FPGAPhotonCounter;
	class FPGADigitalDemultiplexer;
	class FPGAAnalogIntegrator;
	class FPGAAnalogDemultiplexer;
	class FPGAResonanceScanner;
	class FPGAResonanceScannerNI5771;
	class StimulationsDAQmx;
	class XYControl;
	class XYControlStanda;
	class FastZControl;
	class FastZControlETL;
	class XYZControl;
	class XYZControlGalil;
	class XYZControlSutter;
	template<uint32_t NAREAS, class DATA_T> class DaqMultiChunk;
	template<uint32_t NAREAS, class DATA_T> using DaqMultiChunkPtr = std::shared_ptr<DaqMultiChunk<NAREAS, DATA_T>>;
	template<uint32_t NAREAS, class DATA_T> class DaqMultiChunkResonance;
	template<uint32_t NAREAS, class DATA_T> using DaqMultiChunkResonancePtr = std::shared_ptr<DaqMultiChunkResonance<NAREAS, DATA_T>>;
	class ScopeMultiImage;
	class ScopeMultiImageResonanceSW;
	typedef std::shared_ptr<ScopeMultiImage> ScopeMultiImagePtr;
	typedef std::shared_ptr<const ScopeMultiImage> ScopeMultiImageCPtr;
	typedef std::shared_ptr<ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWPtr;
	typedef std::shared_ptr<const ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWCPtr;
	class ScopeOverlay;
	class ScopeOverlayResonanceSW;
	class PixelmapperFrameResonanceHW;
	class PixelmapperFrameResonanceSW;
	
	namespace parameters {
		class OutputsDAQmx;
		class OutputsDAQmxLineClock;
		class OutputsDAQmxSlave;
		class OutputsDAQmxResonance;
		class OutputsDAQmxResonanceSlave;
		class InputsDAQmx;
		class InputsFPGANoiseOutput;
		class InputsFPGAPhotonCounter;
		class InputsFPGADigitalDemultiplexer;
		class InputsFPGAAnalogIntegrator;
		class InputsFPGAAnalogDemultiplexer;
		class InputsFPGAResonanceScanner;
		class XYControl;
		class XYControlStanda;
		class FastZControl;
		class FastZControlETL;
		class XYZControl;
		class XYZControlGalil;
		class XYZControlSutter;
	}
	
	namespace gui {
		class CDAQmxPage;
		class CFPGANoiseOutputPage;
		class CFPGAPhotonCounterPage;
		class CFPGADigitalDemultiplexerPage;
		class CFPGAAnalogIntegratorPage;
		class CFPGAAnalogDemultiplexerPage;
		class CFPGAResonanceScannerPage;
		class CFPGAResonanceScannerNI5771Page;
	}

	namespace config {
		
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
			typedef parameters::OutputsDAQmx type_parameters;
			typedef parameters::OutputsDAQmxSlave type_slave_parameters;
		};

		template<>
		struct OutputTypeSelector<OutputEnum::TwoCardDAQmx> {
			typedef OutputsDAQmxLineClock type;
			typedef ZeroOutputsDAQmxLineClock type_zero;
			typedef OutputsDAQmxSlave type_slave;
			typedef ZeroOutputsDAQmxSlave type_slave_zero;
			typedef parameters::OutputsDAQmxLineClock type_parameters;
			typedef parameters::OutputsDAQmxSlave type_slave_parameters;
		};

		template<>
		struct OutputTypeSelector<OutputEnum::SimpleDAQmx_Resonance> {
			typedef OutputsDAQmxResonance type;
			typedef ZeroOutputsDAQmxResonance type_zero;
			typedef OutputsDAQmxResonanceSlave type_slave;
			typedef ZeroOutputsDAQmxResonanceSlave type_slave_zero;
			typedef parameters::OutputsDAQmxResonance type_parameters;
			typedef parameters::OutputsDAQmxResonanceSlave type_slave_parameters;
		};

		enum class InputEnum {
			DAQmx,
			FPGA_NoiseOutput,
			FPGA_Photoncounter,
			FPGA_Digitaldemultiplexer,
			FPGA_Analogintegrator,
			FPGA_Analogdemultiplexer,
			FPGA_Resonancescanner,
			FPGA_ResonancescannerNI5771
		};

		template<InputEnum>
		struct InputTypeSelector {
			typedef InputsDAQmx type;
			typedef parameters::InputsDAQmx type_parameters;
			typedef FPGANoiseOutput type_fpga;
			typedef gui::CDAQmxPage type_guipage;
		};
		
		template<>
		struct InputTypeSelector<InputEnum::FPGA_NoiseOutput> {
			typedef InputsFPGA type;
			typedef parameters::InputsFPGANoiseOutput type_parameters;
			typedef FPGANoiseOutput type_fpga;
			typedef gui::CFPGANoiseOutputPage type_guipage;
		};

		template<>
		struct InputTypeSelector<InputEnum::FPGA_Photoncounter> {
			typedef InputsFPGA type;
			typedef parameters::InputsFPGAPhotonCounter type_parameters;
			typedef FPGAPhotonCounter type_fpga;
			typedef gui::CFPGAPhotonCounterPage type_guipage;
		};
		
		template<>
		struct InputTypeSelector<InputEnum::FPGA_Digitaldemultiplexer> {
			typedef InputsFPGA type;
			typedef parameters::InputsFPGADigitalDemultiplexer type_parameters;
			typedef FPGADigitalDemultiplexer type_fpga;
			typedef gui::CFPGADigitalDemultiplexerPage type_guipage;
		};
		
		template<>
		struct InputTypeSelector<InputEnum::FPGA_Analogintegrator> {
			typedef InputsFPGA type;
			typedef parameters::InputsFPGAAnalogIntegrator type_parameters;
			typedef FPGAAnalogIntegrator type_fpga;
			typedef gui::CFPGAAnalogIntegratorPage type_guipage;
		};
		
		template<>
		struct InputTypeSelector<InputEnum::FPGA_Analogdemultiplexer> {
			typedef InputsFPGA type;
			typedef parameters::InputsFPGAAnalogDemultiplexer type_parameters;
			typedef FPGAAnalogDemultiplexer type_fpga;
			typedef gui::CFPGAAnalogDemultiplexerPage type_guipage;
		};
		
		template<>
		struct InputTypeSelector<InputEnum::FPGA_Resonancescanner> {
			typedef InputsFPGA type;
			typedef parameters::InputsFPGAResonanceScanner type_parameters;
			typedef FPGAResonanceScanner type_fpga;
			typedef gui::CFPGAResonanceScannerPage type_guipage;
		};
		
		template<>
		struct InputTypeSelector<InputEnum::FPGA_ResonancescannerNI5771> {
			typedef InputsFPGA type;
			typedef parameters::InputsFPGAAnalogIntegrator type_parameters;
			typedef FPGAResonanceScannerNI5771 type_fpga;
			typedef gui::CFPGAResonanceScannerNI5771Page type_guipage;
		};

		enum class FPUXYStageEnum {
			None,
			Standa
		};

		template<FPUXYStageEnum>
		struct FPUXYStageTypeSelector {
			typedef XYControl type;
			typedef parameters::XYControl type_parameters;
		};

		template<>
		struct FPUXYStageTypeSelector<FPUXYStageEnum::Standa> {
			typedef XYControlStanda type;
			typedef parameters::XYControlStanda type_parameters;
		};

		enum class FPUZStageEnum {
			None,
			ETL
		};

		template<FPUZStageEnum>
		struct FPUZStageTypeSelector {
			typedef FastZControl type;
			typedef parameters::FastZControl type_parameters;
		};

		template<>
		struct FPUZStageTypeSelector<FPUZStageEnum::ETL> {
			typedef FastZControlETL type;
			typedef parameters::FastZControlETL type_parameters;
		};

		enum class XYZStageEnum {
			None,
			Galil,
			Sutter
		};

		template<XYZStageEnum>
		struct XYZStageTypeSelector {
			typedef XYZControl type;
			typedef parameters::XYZControl type_parameters;
		};

		template<>
		struct XYZStageTypeSelector<XYZStageEnum::Galil> {
			typedef XYZControlGalil type;
			typedef parameters::XYZControlGalil type_parameters;
		};

		template<>
		struct XYZStageTypeSelector<XYZStageEnum::Sutter> {
			typedef XYZControlSutter type;
			typedef parameters::XYZControlSutter type_parameters;
		};
		
		enum class StimulationsEnum {
			DAQmx
		};
		
		template <StimulationsEnum>
		struct StimulationsTypeSelector {
			typedef StimulationsDAQmx type;
		};

		enum class DaqMultiChunkEnum {
			Regular,
			Resonance
		};

		template <DaqMultiChunkEnum>
		struct DaqMultiChunkTypeSelector {
			template<uint32_t NAREAS, class DATA_T> using type = DaqMultiChunk<NAREAS, DATA_T>;
			template<uint32_t NAREAS, class DATA_T> using type_ptr = DaqMultiChunkPtr<NAREAS, DATA_T>;
		};

		template<>
		struct DaqMultiChunkTypeSelector<DaqMultiChunkEnum::Resonance> {
			template<uint32_t NAREAS, class DATA_T> using type = DaqMultiChunkResonance<NAREAS, DATA_T>;
			template<uint32_t NAREAS, class DATA_T> using type_ptr = DaqMultiChunkResonancePtr<NAREAS, DATA_T>;
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
			Hardware,
			Software
		};

		template<ResonancePixelmapperEnum>
		struct ResonancePixelmapperTypeSelector {
			typedef PixelmapperFrameResonanceHW type;
		};

		template<>
		struct ResonancePixelmapperTypeSelector<ResonancePixelmapperEnum::Software> {
			typedef PixelmapperFrameResonanceSW type;
		};

		enum class FramevectorFillEnum {
			FullframeXYZP,
			LineXPColumnYZ,
			LineZP
		};
		
		template<OutputEnum OUTPUT>
		struct FramevectorFillSelector {
			static constexpr FramevectorFillEnum fill_master = FramevectorFillEnum::FullframeXYZP;
			static constexpr FramevectorFillEnum fill_slave = FramevectorFillEnum::LineZP;
		};

		template<>
		struct FramevectorFillSelector<OutputEnum::TwoCardDAQmx> {
			static constexpr FramevectorFillEnum fill_master = FramevectorFillEnum::LineXPColumnYZ;
		};

		template<long unsigned int SIZE>
		constexpr uint32_t _counttrue(const std::array<bool, SIZE>& _ar) {
			uint32_t n = 0;
			for(auto& a : _ar)
				if ( a ) n++;
			return n;
		}
		
		template<long unsigned int SIZE>
		constexpr bool _msareaconflict(const std::array<bool, SIZE>& _arm, const std::array<bool, SIZE>& _ars) {
			for ( uint32_t i = 0 ; i < SIZE ; i++) {
				if ( !(_arm[i] ^ _ars[i]) )
					return true;
			}
			return false;
		}
	}
}
