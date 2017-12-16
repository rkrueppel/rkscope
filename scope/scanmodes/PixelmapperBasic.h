#pragma once

#include "helpers/ScopeDatatypes.h"
#include "parameters/Scope.h"
#include "helpers/DaqMultiChunk.h"
#include "helpers/DaqMultiChunkResonance.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeMultiImage.h"

namespace scope {

	/** Enum with the possible results of pixelmapping */
	enum PixelmapperResult {
		Nothing = 0x0,
		Error = 0x01,
		EndOfChunk = 0x02,
		FrameComplete = 0x04,
		EndOfChunkAndFrameComplete = 0x02 & 0x04
	};

	/** Parent class for pixel mappers
	* @tparam NAREAS defines how many areas are mapped in parallel (e.g. multiarea configuration with only one scanner-pair) */
	template<uint32_t NAREAS = 1>
	class PixelmapperBasic {

		protected:
			/** Type/scan mode of this pixelmapper */
			const ScannerVectorTypeHelper::Mode type;

			/** pointer to the current frame to be mapped into */
			std::array<ScopeMultiImagePtr, NAREAS> current_frames;

			/** current scanner vector parameter set (needs to be pointer for dynamic_cast in derived classes and because the parameters object is localized in ScopeController) */
			parameters::ScannerVectorFrameBasic* svparameters;

			/** for position lookup */
			const std::vector<std::size_t>* lookup;
			
			/** last position in lookup where we looked up last time */
			std::vector<std::size_t>::const_iterator lastlookup;

		public:
			/** Initializes.
			* @param[in] _scanner Type of builting scanner
			* @param[in] _type Type of underlying scanner vector, set when derived class calls base constructor. Used to generate a fitting parameters set via  parameters::ScannerVectorFrameBasic::Factory. */
			PixelmapperBasic(const ScannerType& _scanner, const ScannerVectorType& _type)
				: type(_type)
				, current_frames()
				, svparameters(parameters::ScannerVectorFrameBasic::Factory(_type).release())
				, lookup(nullptr) {
			}

			/** We need a virtual destructor here, so that derived types get correctly destroyed */
			virtual ~PixelmapperBasic() { }

			/** A static factor method for pixelmappers */
			static std::unique_ptr<PixelmapperBasic<NAREAS>> Factory(const ScannerType& _scanner, const ScannerVectorType& _type) {
				switch ( (ScannerTypeHelper::Mode)_scanner ) {
					case ScannerTypeHelper::Regular:
						switch ( (ScannerVectorTypeHelper::Mode)_type ) {
						case ScannerVectorTypeHelper::Bidirectional:
							return std::unique_ptr<PixelmapperFrameBiDi>(new PixelmapperFrameBiDi());
						// Not yet implemented
						//case ScannerVectorTypeHelper::Planehopper:
						//	return std::unique_ptr<PixelmapperBasic>(new PixelmapperBasic());
						
						default:
							return std::unique_ptr<PixelmapperFrameSaw>(new PixelmapperFrameSaw());
						}
						break;
					case ScannerTypeHelper::Resonance:
						switch ( (ScannerVectorTypeHelper::Mode)_type ) {
						case ScannerVectorTypeHelper::ResonanceBiDi:
							return std::unique_ptr<SCOPE_RESONANCEPIXELMAPPER_T>(new SCOPE_RESONANCEPIXELMAPPER_T());
						}
						break;
				}
			}

			/** Sets pointer to the current frame to be mapped into. */
			virtual void SetCurrentFrame(const uint32_t& _area, ScopeMultiImagePtr const _current_frame) {
				current_frames[_area] = _current_frame;
			}

			/** Set current parameters */
			virtual void SetParameters(parameters::ScannerVectorFrameBasic* const _svparameters) {
				this->svparameters = _svparameters;
			}

			/** Sets the vectors for lookup */
			virtual void SetLookupVector(const std::vector<std::size_t>* const _lookup) {
				lookup = _lookup;
				lastlookup = std::begin(*lookup);
			}

			/** Different flavors of mapping a chunk 
			* MultiChunks with more than 1 area are for configurations where all areas run in sync with the same parameters (e.g. multi-area with just one galvo-set)
			* @{ */
			virtual PixelmapperResult LookupChunk(config::DaqMultiChunkPtrType const _chunk, const uint16_t& _currentavgcount) {
				PixelmapperResult result(Nothing);
				return result;
			}

			/* @} */
			
	};

}