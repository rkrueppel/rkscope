#pragma once

#include "PixelmapperBasic.h"

// Forward declarations
namespace scope {
	class DaqChunkResonance;
	typedef std::shared_ptr<DaqChunkResonance> DaqChunkResonancePtr;
	class ScopeMultiImageResonanceSW;
	typedef std::shared_ptr<ScopeMultiImageResonanceSW> ScopeMultiImageResonanceSWPtr;
}

namespace scope {

/** Maps acquired pixels into an image analysing the resonance scanner sync signal, takes care of return fractions, forth/back lines etc */
class PixelmapperFrameResonanceSW : public PixelmapperBasic {

protected:
	/** pointer to the current frame to be mapped into (for SW resonance mapping, different type than the one declared in PixelmapperBasic). */
	ScopeMultiImageResonanceSWPtr current_frame;

	/** pointer to the current averaged frame to be mapped into */
	ScopeMultiImageResonanceSWPtr current_averaged_frame;

	/** true only for mapping first chunk */
	bool firstchunk;

public:
	PixelmapperFrameResonanceSW();

	/** Maps a chunk */
	PixelmapperResult LookupChunk(DaqChunkResonancePtr const _chunk, const uint16_t& _currentavgcount) override;

	/** Sets pointer to the current averaged (displayed) frame to be mapped into. */
	void SetCurrentAveragedFrame(ScopeMultiImageResonanceSWPtr const _current_averaged_frame);
};

}
