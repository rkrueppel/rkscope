#include "stdafx.h"

#include "DaqChunkResonance.h"

namespace scope {

DaqChunkResonance::DaqChunkResonance(const uint32_t& _perchannel, const uint32_t& _nchannels, const uint32_t& _area)
	: DaqChunk(_perchannel, _nchannels, _area)
	, resSync(_perchannel, 0)
	, lastsyncsig(_nchannels, std::begin(resSync)) {

	for (auto& s : lastsyncsig )
		s = std::begin(resSync);
}

}