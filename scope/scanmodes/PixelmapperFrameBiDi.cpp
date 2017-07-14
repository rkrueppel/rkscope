#include "stdafx.h"
#include "PixelmapperFrameBiDi.h"
#include "helpers/ScopeImage.h"
#include "helpers/ScopeMultiImage.h"

namespace scope {

PixelmapperFrameBiDi::PixelmapperFrameBiDi()
	: PixelmapperBasic(ScannerTypeHelper::Regular, ScannerVectorTypeHelper::Bidirectional) {
}

PixelmapperResult PixelmapperFrameBiDi::LookupChunk(DaqChunkPtr const _chunk, const uint16_t& _currentavgcount) {
	PixelmapperResult result(Nothing);
	uint32_t n = 1;
	const uint32_t multiplier = _currentavgcount;								// currentavgcount = 0: first frame, multiply by 0 -> overwrite last image pixel (for running update of old pixels), see PipelineController
	const uint32_t divisor = std::max<uint32_t>(1, _currentavgcount + 1);		// currentavgcount = 1: second frame, multiply by 1, divide by 2
	const uint32_t halfdivisor = std::max<uint32_t>(1, divisor >> 2);			// etc etc

	auto lookit = lastlookup;
	DaqChunk::iterator channelend;
	DaqChunk::iterator chunkit;
	// Go through all channels
	for ( uint32_t c = 0 ; c < _chunk->NChannels() ; c++ ) {
		// Get the data
		ScopeImageAccessU16 imagedata(*current_frame->GetChannel(c));
		std::vector<uint16_t>* const dataptr(imagedata.GetData());
		// Which sample did we map last in this chunk (initially std::begin)
		chunkit = _chunk->lastmapped[c];
		// where does this channel end in the chunk's data vector
		channelend = std::begin(_chunk->data)+(c+1)*_chunk->PerChannel();
		// Which pixel did we last look up in the lookup vector
		lookit = lastlookup;
		// Advance iterators in chunk and lookup vector in parallel
		auto lookupend = std::end(*lookup);
		for (  ; (lookit != lookupend) && (chunkit != channelend) ; lookit++, chunkit++ ) {
			// Do a little calculation for the online averaging
			n = (static_cast<uint32_t>(dataptr->operator[](*lookit)) * multiplier) + static_cast<uint32_t>(*chunkit);
			dataptr->operator[](*lookit) =  static_cast<uint16_t>(  n / divisor + ((n%divisor)>halfdivisor?1u:0u) );
		}
		// save in the chunk which sample was last mapped
		_chunk->lastmapped[c] = chunkit;
	}
	// save which pixel we last looked up
	if ( lookit == std::end(*lookup) ) {
		lastlookup = std::begin(*lookup);
		result =  PixelmapperResult(result | FrameComplete);
	}
	else
		lastlookup = lookit;

	if ( chunkit == channelend )
		result = PixelmapperResult(result | EndOfChunk);

	return result;
}

}