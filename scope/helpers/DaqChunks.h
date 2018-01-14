#pragma once
#include "helpers/helpers.h"

namespace scope {

	template<class DATA_T = uint16_t>
	class DaqChunk {
		public:
			/** Iterator over the data vector */
			typedef typename std::vector<DATA_T>::iterator dataiterator;

			/** The data vector */
			std::vector<DATA_T> data;

			/** Iterators to positions that were last mapped. Order could be area first, channel second A0C0A0C1A1C0A1C1 and so on. */
			std::vector<dataiterator> lastmapped;

			DaqChunk(const uint32_t& _datasize, const uint32_t& _lastmappedsize)
				: data(_datasize, (DATA_T)0)
				, lastmapped(_lastmappedsize, std::begin(data)) {}

			virtual ~DaqChunk() {}
	};

	/** A DaqMultiChunk contains data from all areas and all channels sequentially.
	* First area 0 with first perchannel samples of channel 0, then perchannel samples of channel 1 etc
	* Then area 1 with etc.
	* @tparam NCHANNELS is the number of channels
	* @tparam NAREAS is the number of areas
	* @tparam DATA_T is the datatype of the chunk data.*/
	template<uint32_t NCHANNELS = 1, uint32_t NAREAS = 1, class DATA_T = uint16_t>
	class DaqMultiChunk
		: public DaqChunk<DATA_T> {

		protected:
			/** Number of samples per channel */
			uint32_t perchannel;

		public:
			/** Iterators to positions that was last mapped. Outer array is areas, inner vector is channels. */
			std::array<std::vector<dataiterator>, NAREAS> lastmapped;

			/** @param[in] _perchannel number of samples the chunk should contain per channel */
			DaqMultiChunk(const uint32_t& _perchannel)
				: DaqChunk<DATA_T>(NAREAS * _perchannel * NCHANNELS, NAREAS*NCHANNELS)
				, perchannel(_perchannel)
			{
				// Reset lastmapped
				lastmapped.fill(std::vector<dataiterator>(NCHANNELS));
				for (uint32_t a = 0; a < NAREAS; a++) {
					for (uint32_t c = 0; c < NCHANNELS; c++) {
						lastmapped[a][c] = std::begin(data) + a * (NCHANNELS*perchannel) + c * perchannel;
					}
				}
			}

			/** @name Several accessor methods
			* @{ */
			uint32_t PerChannel() const { return perchannel; }
			dataiterator GetLastMapped(const uint32_t& _area, const uint32_t& _channel) const { return lastmapped[_area][_channel]; }
			/* @ret Where does _area start in the data vector */
			dataiterator GetDataStart(const uint32_t& _area) { return data.begin() + _area * NCHANNELS * perchannel; }
			/** @} */
			
			/** @name Mutator methods
			* @{ */
			void SetLastMapped(const uint32_t& _area, const uint32_t& _channel, const dataiterator& _last) { lastmapped[_area][_channel] = _last; }
			/** @} */

			/** Downsamples by averaging _factor samples. Replaces every _factor samples by their average.
			* The chunk does shrink that way and perchannel is adjusted.
			* @param[in] _factor downsampling factor */
			void Downsample(const uint32_t& _factor) {
				assert((perchannel % _factor) == 0);		// make sure we have _factor*perchannel=pixelsperchannel

				if (_factor == 1)
					return;
				uint32_t acc = 0;								// accumulate as 32bit to avoid overflow
				uint32_t halffactor = _factor >> 2;
				auto pixelend = std::begin(data);

				// We do not have to care about area or channel borders here.
				// Outer loop increases iterator for downsampled pixels (overwrites old pixels)
				for (auto it = std::begin(data), downit = std::begin(data); it != std::end(data); ++downit) {
					acc = 0;
					pixelend = it + _factor;

					// inner loop increases iterator for this pixels and accumulates
					for (; it != pixelend; ++it)
						acc = acc + static_cast<uint32_t>(*it);

					*downit = static_cast<uint16_t>(acc / _factor) + ((acc%_factor)>halffactor ? 1u : 0u);		// quick rounding trick for integer multiplication
				}

				// since we have overwritten part of the vector with the downsampled pixels, we resize it now
				perchannel /= _factor;
				data.resize(NAREAS*perchannel*NCHANNELS);

				// Reset lastmapped
				for (uint32_t a = 0; a < NAREAS; a++) {
					for (uint32_t c = 0; c < NCHANNELS; c++) {
						lastmapped[a][c] = std::begin(data) + a * (NCHANNELS*perchannel) + c * perchannel;
					}
				}
			}

			/** Multiplies every sample by _factor */
			virtual void Scale(const double& _factor) {
				assert(_factor > 0);
				std::transform(std::begin(data), std::end(data), std::begin(data), [&_factor](uint16_t& elem) {
					return round2ui16(_factor * static_cast<double>(elem)); });
			}
	};

	/** A DaqChunk contains data from all areas and channels sequentially and additionally a bool vector for the resonance scanner sync signal.
	* First area 0 with first perchannel samples of channel 0, then perchannel samples of channel 1 etc
	* Then area 1 with etc.
	* NOT thread-safe!
	* @tparam NCHANNELS is the number of channels
	* @tparam NAREAS is the number of areas
	* @tparam DATA_T is the datatype of the chunk data.*/
	template<uint32_t NCHANNELS = 1, uint32_t NAREAS = 1, class DATA_T = uint16_t>
	class DaqMultiChunkResonance
		: public DaqMultiChunk<NCHANNELS, NAREAS, DATA_T> {

		public:
			/** Iterator over the sync vector */
			typedef std::vector<bool>::iterator iteratorSync;

			/** The vector with booleans for the synchronization signal for the resonance scanner */
			std::vector<bool> resSync;

			/** Iterators to positions of the last used sync signal point. One for every channel. */
			std::vector<iteratorSync> lastsyncsig;

			/** @param[in] _perchannel number of samples the chunk should contain per channel
			* @param[in] _nchannels number of channels in the chunk (total number of samples is thus NAREAS*_perchannel*_nchannels) */
			DaqMultiChunkResonance(const uint32_t& _perchannel)
				: DaqMultiChunk(_perchannel)
				, resSync(_perchannel, 0)
				, lastsyncsig(NCHANNELS, std::begin(resSync))
			{}

			/** @name Mutator/accessor for the resonance sync stuff
			* @{ */
			iteratorSync GetLastSyncSig(const uint32_t& _channel) const { return lastsyncsig[_channel]; }
			void SetLastSyncSig(const uint32_t& _channel, const iteratorSync& _last) { lastsyncsig[_channel] = _last; }
			/** @} */
	};


}
