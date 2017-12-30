#pragma once
#include "PixelmapperBasic.h"
#include "helpers/helpers.h"

namespace scope {

	/** Maps acquired pixels into an image, takes care of onset cutoff, retrace etc.
	* C: Cutoff, I: real image, R: retrace
	* Chunk has all samples C&I&R, but we only want to use I samples
	* It gets complicated because the end of a chunk can be anywhere
	*
	* 	CCCCCCCCCCCCC
	* 	CCCCCCCCCCCCC
	* 	CCIIIIIIIIIRR
	* 	CCIIIIIIIIIRR
	* 	CCIIIIIIIIIRR
	* 	CCIIIIIIIIIRR
	* 	CCIIIIIIIIIRR
	* 	RRRRRRRRRRRRR
	* 	RRRRRRRRRRRRR
	* @tparam NAREAS defines how many areas are mapped in parallel (e.g. multiarea configuration with only one scanner-pair) */
	template<uint32_t NCHANNELS = 2, uint32_t NAREAS = 1>
	class PixelmapperFrameSaw
	 : public PixelmapperBasic<NCHANNELS, NAREAS> {

		public:
			PixelmapperFrameSaw()
				: PixelmapperBasic(ScannerTypeHelper::Regular, ScannerVectorTypeHelper::Sawtooth)
			{ }

			/** Maps multi chunks in parallel via one lookup vector */
			PixelmapperResult LookupChunk(DaqMultiChunk<NCHANNELS, NAREAS, uint16_t> const _chunk, const uint16_t* _currentavgcount) override {
				PixelmapperResult result(Nothing);
				uint32_t n = 1;
				const uint32_t multiplier = _currentavgcount;								// currentavgcount = 0: first frame, multiply by 0 -> overwrite last image pixel (for running update of old pixels), see PipelineController
				const uint32_t divisor = std::max<uint32_t>(1, _currentavgcount + 1);		// currentavgcount = 1: second frame, multiply by 1, divide by 2
				const uint32_t halfdivisor = std::max<uint32_t>(1, divisor >> 2);			// etc etc

				auto lookit = lastlookup;
				std::array<DaqChunk<uint16_t>::iterator, NAREAS> channelend;
				std::array<DaqChunk<uint16_t>::iterator, NAREAS> chunkit;
				// Go through all channels
				for ( uint32_t c = 0 ; c < NCHANNELS ; c++ ) {
					std::vector<ScopeImageAccessU16> imagedata;
					std::vector<std::vector<uint16_t>* const> dataptr;
					for ( uint32_t a = 0 ; a < NAREAS ; a++ ) {
						// Get the data
						imagedata.push_back(ScopeImageAccessU16 (*current_frames[a]->GetChannel(c));
						dataptr.push_back(imagedata[a].GetData());
						
						// Which sample did we map last in this chunk (initially st::begin)
						chunkit[a] = _chunk.lastmapped[a][c];
						// where does this channel end in the chunk's data vector
						channelend[a] = _chunk.GetDataStart(a) + (c+1)*_chunk.PerChannel();
					}
					
					// Which pixel did we last look up in the lookup vector
					lookit = lastlookup;
					// Advance iterators in chunk and lookup vector in parallel
					auto lookupend = std::end(*lookup);
					for (  ; (lookit != lookupend) && (chunkit[0] != channelend[0]) ; lookit++) {
						for ( uint32_t a = 0 ; a < NAREAS ; a++ ) {
							// Do a little calculation for the online averaging
							n = (static_cast<uint32_t>(dataptr[a]->operator[](*lookit)) * multiplier) + static_cast<uint32_t>(*chunkit[a]);
							dataptr[a]->operator[](*lookit) =  static_cast<uint16_t>(  n / divisor + ((n%divisor)>halfdivisor?1u:0u) );
							chunkit[a]++;
						}
					}
					// save in the chunk which sample was last mapped
					for ( uint32_t a = 0 ; a < NAREAS ; a++)
						_chunk->lastmapped[a][c] = chunkit[a];
				}
				// save which pixel we last looked up
				if ( lookit == lookup->end() ) {
					lastlookup = std::begin(*lookup);
					result =  PixelmapperResult(result | FrameComplete);
				}
				else
					lastlookup = lookit;

				if ( chunkit[0] == channelend[0] )
					result = PixelmapperResult(result | EndOfChunk);

				return result;
			}
	};

}