#pragma once

#include "helpers/ScopeDatatypes.h"
#include "parameters/Scope.h"
#include "helpers/DaqChunks.h"
#include "helpers\ScopeImage.h"
#include "helpers/ScopeMultiImage.h"

namespace scope {

	template<uint32_t, uint32_t> class PixelmapperFrameSaw;
	template<uint32_t, uint32_t> class PixelmapperFrameBiDi;
	template<uint32_t, uint32_t> class PixelmapperFrameResonanceHW;
	template<uint32_t, uint32_t> class PixelmapperFrameResonanceSW;

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
	template<uint32_t NCHANNELS = 2, uint32_t NAREAS = 1>
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
			static std::unique_ptr<PixelmapperBasic<NCHANNELS, NAREAS>> Factory(const config::ScannerEnum _scanner, const ScannerVectorType& _type) {
				switch ( _scanner ) {
					case config::ScannerEnum::RegularGalvo:
						switch ( (ScannerVectorTypeHelper::Mode)_type ) {
							case ScannerVectorTypeHelper::Bidirectional:
								return std::make_unique<PixelmapperFrameBiDi<NCHANNELS, NAREAS>>();
							// Not yet implemented
							//case ScannerVectorTypeHelper::Planehopper:
							//	return std::unique_ptr<PixelmapperBasic>(new PixelmapperBasic());
						
							default:
								return std::make_unique<PixelmapperFrameSaw<NCHANNELS, NAREAS>>();
						}
						break;
					case config::ScannerEnum::ResonantGalvo:
						switch ( (ScannerVectorTypeHelper::Mode)_type ) {
							case ScannerVectorTypeHelper::ResonanceBiDi:
								return std::make_unique<PixelmapperFrameResonanceHW<NCHANNELS, NAREAS>>();
						}
						break;
				}
			}

			/** Sets pointer to the current frame to be mapped into. */
			virtual void SetCurrentFrame(const uint32_t& _area, ScopeMultiImagePtr const _current_frame) {
				current_frames[_area] = _current_frame;
			}

			virtual void SetCurrentFrames(std::vector<ScopeMultiImagePtr> const _current_frames) {
				assert(_current_frames.size() == NAREAS);
				for (uint32_t a = 0; a < NAREAS; a++)
					current_frames[a] = _current_frames[a];
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
			virtual PixelmapperResult LookupChunk(DaqMultiChunk<NCHANNELS, NAREAS, uint16_t>& _chunk, const uint16_t& _currentavgcount) {
				PixelmapperResult result(Nothing);
				return result;
			}

			/* @} */
			
	};

	/** Maps acquired pixels into an image, takes care of return fractions
	* I: real image, forth scan, J: real image, back scan, R: return
	* Chunk has all samples C&I&R, but we only want to use I samples (and the J samples in reverse order as they come)
	*
	* RRRRRRRRR
	* RIIIIIIIR
	* RJJJJJJJR
	* RIIIIIIIR
	* RJJJJJJJR
	* */
	template<uint32_t NCHANNELS = 2, uint32_t NAREAS = 1>
	class PixelmapperFrameBiDi : public PixelmapperBasic<NCHANNELS, NAREAS> {

	public:
		PixelmapperFrameBiDi()
			: PixelmapperBasic(ScannerTypeHelper::Regular, ScannerVectorTypeHelper::Bidirectional) {

		}

		/** Maps a chunk via the lookup vector */
		PixelmapperResult LookupChunk(DaqMultiChunk<NCHANNELS, NAREAS, uint16_t>& _chunk, const uint16_t& _currentavgcount) {
			PixelmapperResult result(Nothing);
			uint32_t n = 1;
			const uint32_t multiplier = _currentavgcount;								// currentavgcount = 0: first frame, multiply by 0 -> overwrite last image pixel (for running update of old pixels), see PipelineController
			const uint32_t divisor = std::max<uint32_t>(1, _currentavgcount + 1);		// currentavgcount = 1: second frame, multiply by 1, divide by 2
			const uint32_t halfdivisor = std::max<uint32_t>(1, divisor >> 2);			// etc etc

			auto lookit = lastlookup;
			DaqMultiChunk<NCHANNELS, NAREAS, uint16_t>::dataiterator channelend;
			DaqMultiChunk<NCHANNELS, NAREAS, uint16_t>::dataiterator chunkit;
			// Go through all channels
			for (uint32_t c = 0; c < NCHANNELS; c++) {
				// Get the data
				ScopeImageAccessU16 imagedata(*current_frames[0]->GetChannel(c));
				std::vector<uint16_t>* const dataptr(imagedata.GetData());
				// Which sample did we map last in this chunk (initially std::begin)
				chunkit = _chunk.lastmapped[0][c];
				// where does this channel end in the chunk's data vector
				channelend = std::begin(_chunk.data) + (c + 1)*_chunk.PerChannel();
				// Which pixel did we last look up in the lookup vector
				lookit = lastlookup;
				// Advance iterators in chunk and lookup vector in parallel
				auto lookupend = std::end(*lookup);
				for (; (lookit != lookupend) && (chunkit != channelend); lookit++, chunkit++) {
					// Do a little calculation for the online averaging
					n = (static_cast<uint32_t>(dataptr->operator[](*lookit)) * multiplier) + static_cast<uint32_t>(*chunkit);
					dataptr->operator[](*lookit) = static_cast<uint16_t>(n / divisor + ((n%divisor)>halfdivisor ? 1u : 0u));
				}
				// save in the chunk which sample was last mapped
				_chunk.lastmapped[0][c] = chunkit;
			}
			// save which pixel we last looked up
			if (lookit == std::end(*lookup)) {
				lastlookup = std::begin(*lookup);
				result = PixelmapperResult(result | FrameComplete);
			}
			else
				lastlookup = lookit;

			if (chunkit == channelend)
				result = PixelmapperResult(result | EndOfChunk);

			return result;
		}
	};

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
		PixelmapperResult LookupChunk(DaqMultiChunk<NCHANNELS, NAREAS, uint16_t> const _chunk, const uint16_t* _currentavgcount) {
			PixelmapperResult result(Nothing);
			uint32_t n = 1;
			const uint32_t multiplier = _currentavgcount;								// currentavgcount = 0: first frame, multiply by 0 -> overwrite last image pixel (for running update of old pixels), see PipelineController
			const uint32_t divisor = std::max<uint32_t>(1, _currentavgcount + 1);		// currentavgcount = 1: second frame, multiply by 1, divide by 2
			const uint32_t halfdivisor = std::max<uint32_t>(1, divisor >> 2);			// etc etc

			auto lookit = lastlookup;
			std::array<DaqChunk<uint16_t>::iterator, NAREAS> channelend;
			std::array<DaqChunk<uint16_t>::iterator, NAREAS> chunkit;
			// Go through all channels
			for (uint32_t c = 0; c < NCHANNELS; c++) {
				std::vector<ScopeImageAccessU16> imagedata;
				std::vector<std::vector<uint16_t>* const> dataptr;
				for (uint32_t a = 0; a < NAREAS; a++) {
					// Get the data
					imagedata.push_back(ScopeImageAccessU16(*current_frames[a]->GetChannel(c)));
					dataptr.push_back(imagedata[a].GetData());

					// Which sample did we map last in this chunk (initially st::begin)
					chunkit[a] = _chunk.lastmapped[a][c];
					// where does this channel end in the chunk's data vector
					channelend[a] = _chunk.GetDataStart(a) + (c + 1)*_chunk.PerChannel();
				}

				// Which pixel did we last look up in the lookup vector
				lookit = lastlookup;
				// Advance iterators in chunk and lookup vector in parallel
				auto lookupend = std::end(*lookup);
				for (; (lookit != lookupend) && (chunkit[0] != channelend[0]); lookit++) {
					for (uint32_t a = 0; a < NAREAS; a++) {
						// Do a little calculation for the online averaging
						n = (static_cast<uint32_t>(dataptr[a]->operator[](*lookit)) * multiplier) + static_cast<uint32_t>(*chunkit[a]);
						dataptr[a]->operator[](*lookit) = static_cast<uint16_t>(n / divisor + ((n%divisor)>halfdivisor ? 1u : 0u));
						chunkit[a]++;
					}
				}
				// save in the chunk which sample was last mapped
				for (uint32_t a = 0; a < NAREAS; a++)
					_chunk->lastmapped[a][c] = chunkit[a];
			}
			// save which pixel we last looked up
			if (lookit == lookup->end()) {
				lastlookup = std::begin(*lookup);
				result = PixelmapperResult(result | FrameComplete);
			}
			else
				lastlookup = lookit;

			if (chunkit[0] == channelend[0])
				result = PixelmapperResult(result | EndOfChunk);

			return result;
		}
	};

	/** Maps acquired pixels into an image analysing the resonance scanner sync signal, takes care of return fractions, forth/back lines etc
	* @tparam NAREAS defines how many areas are mapped in parallel (e.g. multiarea configuration with only one scanner-pair) */
	template<uint32_t NCHANNELS = 2, uint32_t NAREAS = 1>
	class PixelmapperFrameResonanceHW
		: public PixelmapperBasic<NCHANNELS, NAREAS> {
	public:
		PixelmapperFrameResonanceHW()
			: PixelmapperBasic(ScannerTypeHelper::Resonance, ScannerVectorTypeHelper::ResonanceBiDi) {
		}

		/** Maps a chunk */
		PixelmapperResult LookupChunk(DaqMultiChunk<NCHANNELS, NAREAS, uint16_t>& _chunk, const uint16_t& _currentavgcount) {
			PixelmapperResult result(Nothing);

			return result;
		}
	};

	/** Maps acquired pixels into an image analysing the resonance scanner sync signal, takes care of return fractions, forth/back lines etc
	* @tparam NAREAS defines how many areas are mapped in parallel (e.g. multiarea configuration with only one scanner-pair) */
	template<uint32_t NCHANNELS = 2, uint32_t NAREAS = 1>
	class PixelmapperFrameResonanceSW
		: public PixelmapperBasic<NCHANNELS, NAREAS> {

	protected:
		/** pointer to the current frame to be mapped into (for SW resonance mapping, different type than the one declared in PixelmapperBasic). */
		ScopeMultiImageResonanceSWPtr current_frame;

		/** pointer to the current averaged frame to be mapped into */
		ScopeMultiImageResonanceSWPtr current_averaged_frame;

		/** true only for mapping first chunk */
		bool firstchunk;

	public:
		PixelmapperFrameResonanceSW()
			: PixelmapperBasic(ScannerTypeHelper::Resonance, ScannerVectorTypeHelper::ResonanceBiDi)
			, firstchunk(true) {
		}

		/** Maps a chunk */
		PixelmapperResult LookupChunk(DaqMultiChunkResonance<NCHANNELS, NAREAS, uint16_t>& _chunk, const uint16_t& _currentavgcount) {
			PixelmapperResult result(Nothing);
			uint32_t n = 1;
			const uint32_t multiplier = _currentavgcount;								// currentavgcount = 0: first frame, multiply by 0 -> overwrite last image pixel (for running update of old pixels), see PipelineController
			const uint32_t divisor = std::max<uint32_t>(1, _currentavgcount + 1);		// currentavgcount = 1: second frame, multiply by 1, divide by 2
			const uint32_t halfdivisor = std::max<uint32_t>(1, divisor >> 2);			// etc etc

			parameters::ScannerVectorFrameResonance* tmp = dynamic_cast<parameters::ScannerVectorFrameResonance*>(svparameters);

			const uint32_t xturnpixelsleft = tmp->XTurnPixelsLeft();
			const uint32_t xturnpixelsright = tmp->XTurnPixelsRight();
			const uint32_t ximagepixels = tmp->XImagePixels();
			const uint32_t cutofflines = tmp->YCutoffLines();
			const uint32_t scanlines = tmp->YScanLines();
			const uint32_t ytotallines = tmp->YTotalLines();
			const uint32_t xtotalpixels = tmp->XTotalPixels();
			const uint32_t totalpixels = tmp->TotalPixels();
			const uint32_t totalimagepixels = tmp->XImagePixels()*tmp->YImageLines();

			uint32_t imagepos;
			bool forthline;
			uint32_t l;
			uint32_t i;
			uint32_t lx;
			DaqMultiChunkResonance<NCHANNELS, NAREAS>::iterator channelend;
			DaqMultiChunkResonance<NCHANNELS, NAREAS>::iterator chunkit;
			DaqMultiChunkResonance<NCHANNELS, NAREAS>::iteratorSync syncit;

			const uint32_t area = current_frame->Area();
			const size_t num_planes = tmp->planes.size() ? tmp->planes.size() : 1;
			const uint32_t maxlines = ytotallines * num_planes;
			const uint32_t maximagepixels = totalimagepixels * num_planes; // 65536*np

			 // Go through all channels
			for (uint32_t c = 0; c < _chunk->NChannels(); c++) {

				// Loop over the volume scanning planes
				for (size_t np = 0; np < num_planes; np++) {
					size_t cyy = ytotallines * np;

					// Get the data
					ScopeImageAccessU16 imagedata(*current_frame->GetChannel(c)); //GetChannel : returns pointer to one channel image
					std::vector<uint16_t>* const dataptr(imagedata.GetData()); //dataptr gets the Pointer to imagedata(ScopeImage)'s data vector

					ScopeImageAccessU16 averagedimagedata(*current_averaged_frame->GetChannel(c));
					std::vector<uint16_t>* const averageddataptr(averagedimagedata.GetData());

					// Which sample did we map last in this chunk (initially std::begin) and at what position of the sync signal
					chunkit = _chunk->lastmapped[c];
					syncit = _chunk->lastsyncsig[c];
					// where does this channel end in the chunk's data vector
					channelend = std::begin(_chunk->data) + (c + 1)*_chunk->PerChannel();
					// Which pixel did we last map into the image
					imagepos = current_frame->LastImagePos(c);
					forthline = current_frame->LastForthline(c);

					// find beginning of the first line of the first chunk
					if (firstchunk) {
						firstchunk = false;
						do {
							chunkit++;
							syncit++;
						} while ((chunkit + 1 != channelend) && !(*syncit == 1 && *(syncit + 1) == 0));
						// To ensure that the display starts from the top frame, move back the pointers
						if (num_planes > 1)
						{
							chunkit = chunkit + (num_planes - 2)*totalimagepixels;
							syncit = syncit + (num_planes - 2)*totalimagepixels;
						}
					}

					// always aligning on the left
					// y scanner is based on given number of lines
					//// FOR L = loop over lines
					for (l = current_frame->LastL(c); l < cyy + ytotallines; l++) {
						// x (resonant) scanner is based on the synchronization signal and waits until it switches from 1 to 0

						if (forthline) {
							// Loop over X-position
							for (i = current_frame->LastX(c); (chunkit != channelend) && (i < (xtotalpixels - xturnpixelsright)); i++, syncit++, chunkit++) {
								if ((l >= cyy + cutofflines) && (l < cyy + scanlines) && (i >= xturnpixelsleft)) {
									//if ( (l >= cutofflines) && (l < scanlines) && (i >= xturnpixelsleft) ) {
									// write into the frame that is not averaged
									dataptr->operator[](imagepos) = *chunkit;

									// write into the averaged frame
									n = (static_cast<uint32_t>(averageddataptr->operator[](imagepos)) * multiplier) + static_cast<uint32_t>(*chunkit);
									averageddataptr->operator[](imagepos) = static_cast<uint16_t>(n / divisor + ((n%divisor)>halfdivisor ? 1u : 0u));

									imagepos++;
								}
								lx = i;
							}
							current_frame->SetLastX(c, 0);
						}
						else {
							for (i = current_frame->LastX(c); chunkit != channelend; i++, syncit++, chunkit++) {
								// save intermediately in a vector
								current_frame->CurrentLineData()->at(c).at(i) = *chunkit;
								if ((chunkit + 1 != channelend) && (*syncit == 1 && *(syncit + 1) == 0)) {
									// fill in beginning at the end of the intermediate vector
									for (uint32_t x = 0; x < ((i >= xtotalpixels - xturnpixelsright) ? (xtotalpixels - xturnpixelsright) : i); x++) {
										if ((l >= cyy + cutofflines) && (l < cyy + scanlines) && (x >= xturnpixelsleft)) {
											//if ( (l >= cutofflines) && (l < scanlines) && (x >= xturnpixelsleft) ) {
											imagepos--;

											// write into the frame that is not averaged
											dataptr->operator[](imagepos) = current_frame->CurrentLineData()->at(c).at(i - x - 1);

											// write into the averaged frame
											n = (static_cast<uint32_t>(averageddataptr->operator[](imagepos)) * multiplier) + static_cast<uint32_t>(current_frame->CurrentLineData()->at(c).at(i - x - 1));
											averageddataptr->operator[](imagepos) = static_cast<uint16_t>(n / divisor + ((n%divisor)>halfdivisor ? 1u : 0u));
										}
									}
									syncit++;
									chunkit++;
									current_frame->SetLastX(c, 0);
									break;
								}
								lx = i;
							}
						}

						if (chunkit == channelend) {
							current_frame->SetLastX(c, lx);
							result = PixelmapperResult(result | EndOfChunk);
							break;
						}
						if ((l >= cyy + cutofflines) && (l < cyy + scanlines)) {
							if (forthline)
								imagepos = ((l - (cyy + cutofflines) + 2) * ximagepixels) + np * totalimagepixels;
							else
								imagepos = ((l - (cyy + cutofflines) + 1) * ximagepixels) + np * totalimagepixels;
						}
						forthline = !forthline;
					}
					//// END FOR L

					// Save in the chunk which sample was last mapped and the position of the sync signal
					_chunk->SetLastMapped(c, chunkit);
					_chunk->SetLastSyncSig(c, syncit);

					// save which pixel we last looked up
					if (l >= maxlines) {
						current_frame->SetLastImagepos(c, 0);
						current_frame->SetLastL(c, 0);
						current_frame->SetLastForthline(c, true);
						current_frame->SetLastX(c, 0);

						result = PixelmapperResult(result | FrameComplete);
					}
					else {
						current_frame->SetLastImagepos(c, imagepos);
						current_frame->SetLastL(c, l);
					}
					current_frame->SetLastForthline(c, forthline);

				} // End of loop for num_planes
			} // End of loop for c = no. of channels

			return result;

		}

		/** Sets pointer to the current averaged (displayed) frame to be mapped into. */
		void SetCurrentAveragedFrame(ScopeMultiImageResonanceSWPtr const _current_averaged_frame) {
			current_averaged_frame = _current_averaged_frame;
		}
	};

}