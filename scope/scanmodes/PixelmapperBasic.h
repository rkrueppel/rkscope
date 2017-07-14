#pragma once

#include "ScopeDatatypes.h"

// Forward declarations
namespace scope {
	class DaqChunk;
	typedef std::shared_ptr<DaqChunk> DaqChunkPtr;
	class DaqChunkResonance;
	typedef std::shared_ptr<DaqChunkResonance> DaqChunkResonancePtr;
	class ScopeMultiImage;
	template<class T> class ScopeDatatypeBase;
	typedef ScopeDatatypeBase<ScannerVectorTypeHelper> ScannerVectorType;
	typedef std::shared_ptr<ScopeMultiImage> ScopeMultiImagePtr;
	namespace parameters {
		class ScannerVectorFrameBasic;
	}
}

namespace scope {

/** Enum with the possible results of pixelmapping */
enum PixelmapperResult {
	Nothing = 0x0,
	Error = 0x01,
	EndOfChunk = 0x02,
	FrameComplete = 0x04,
	EndOfChunkAndFrameComplete = 0x02 & 0x04
};

/** Parent class for pixel mappers */
class PixelmapperBasic {

protected:
	/** Type/scan mode of this pixelmapper */
	const ScannerVectorTypeHelper::Mode type;

	/** pointer to the current frame to be mapped into */
	ScopeMultiImagePtr current_frame;

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
	PixelmapperBasic(const ScannerType& _scanner, const ScannerVectorType& _type);

	/** We need a virtual destructor here, so that derived types get correctly destroyed */
	virtual ~PixelmapperBasic();

	/** A static factor method for pixelmappers */
	static std::unique_ptr<PixelmapperBasic> Factory(const ScannerType& _scanner, const ScannerVectorType& _type);

	/** Sets pointer to the current frame to be mapped into. */
	virtual void SetCurrentFrame(ScopeMultiImagePtr const _current_frame);

	/** Set current parameters */
	virtual void SetParameters(parameters::ScannerVectorFrameBasic* const _svparameters);

	/** Sets the vectors for lookup */
	virtual void SetLookupVector(const std::vector<std::size_t>* const _lookup);

	/** Different flavors of mapping a chunk 
	* @{ */
	virtual PixelmapperResult LookupChunk(DaqChunkPtr const _chunk, const uint16_t& _currentavgcount);
	virtual PixelmapperResult LookupChunk(DaqChunkResonancePtr const _chunk, const uint16_t& _currentavgcount);
	/* @} */
};

}