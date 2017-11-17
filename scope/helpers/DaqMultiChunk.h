#pragma once

#include "../ScopeDefines.h"
#include "helpers/helpers.h"

namespace scope {

	/** A DaqMultiChunk contains data from all areas and all channels sequentially.
	* First area 0 with first perchannel samples of channel 0, then perchannel samples of channel 1 etc
	* Then area 1 with etc.
	* @tparam NAREAS is the number of areas
	* @tparam DATA_T is the datatype of the chunk data.*/
	template<uint32_t NAREAS = 1, class DATA_T = uint16_t>
	class DaqMultiChunk {

		protected:
			/** Number of channels in each area */
			const uint32_t nchannels;

			/** Number of samples per channel */
			uint32_t perchannel;

		public:
			/** Iterator over the data vector */
			typedef typename std::vector<DATA_T>::iterator iterator;

			/** The data vector */
			std::vector<DATA_T> data;

			/** Iterators to positions that was last mapped. Outer array is areas, inner vector is channels. */
			std::array<std::vector<iterator>, NAREAS> lastmapped;

			/** @param[in] _perchannel number of samples the chunk should contain per channel
			* @param[in] _nchannels number of channels in the chunk (total number of samples is thus NAREAS*_perchannel*_nchannels) */
			DaqMultiChunk(const uint32_t& _perchannel, const uint32_t& _nchannels)
				: nchannels(_nchannels)
				, perchannel(_perchannel)
				, data(NAREAS * _perchannel * _nchannels, (DATA_T)0)
				, lastmapped(std::vector(_nchannels)) 
			{
				// Reset lastmapped
				for (uint32_t a = 0 ; a < NAREAS ; a++) {
					for (uint32_t c = 0 ; < nchannels ; c++) {
						lastmapped[a][c] = std::begin(data) + a*(nchannels*perchannel) + c*perchannel;
					}
				}
			}

			/** Virtual destructor here, to be prepared for derived classes */
			virtual ~DaqMultiChunk() { };

			/** @name Several accessor methods
			* @{ */
			uint32_t NAreas() const { return NAREAS; }
			uint32_t NChannels() const { return nchannels; }
			uint32_t PerChannel() const { return perchannel; }
			iterator GetLastMapped(const uint32_t& _area, const uint32_t& _channel) const { return lastmapped[_area][_channel]; }
			iterator GetDataStart(const uint32_t& _area) { return data.begin() + _area * nchannels * perchannel; }
			/** @} */

			/** @name Mutator methods
			* @{ */
			void SetLastMapped(const uint32_t& _area, const uint32_t& _channel, const iterator& _last) { lastmapped[_area][_channel] = _last; }
			/** @} */
			
			/** Downsamples by averaging _factor samples. Replaces every _factor samples by their average.
			* The chunk does shrink that way and perchannel is adjusted.
			* @param[in] _factor downsampling factor */
			void Downsample(const uint32_t& _factor) {
				assert( (perchannel % _factor) == 0 );		// make sure we have _factor*perchannel=pixelsperchannel
			
				if ( _factor == 1 )
					return;
				uint32_t acc = 0;								// accumulate as 32bit to avoid overflow
				uint32_t halffactor = _factor>>2;
				auto pixelend = std::begin(data);
				
				// We do not have to care about area or channel borders here.
				// Outer loop increases iterator for downsampled pixels (overwrites old pixels)
				for ( auto it = std::begin(data), downit = std::begin(data); it != std::end(data) ; ++downit ) {
					acc = 0;
					pixelend = it + _factor;
					
					// inner loop increases iterator for this pixels and accumulates
					for ( ; it != pixelend ; ++it)
						acc = acc + static_cast<uint32_t>(*it);
					
					*downit = static_cast<uint16_t>(acc / _factor) + ((acc%_factor)>halffactor?1u:0u);		// quick rounding trick for integer multiplication
				}
				
				// since we have overwritten part of the vector with the downsampled pixels, we resize it now
				perchannel /= _factor;
				data.resize(NAREAS*perchannel*nchannels);
				
				// Reset lastmapped
				for (uint32_t a = 0 ; a < NAREAS ; a++) {
					for (uint32_t c = 0 ; < nchannels ; c++) {
						lastmapped[a][c] = std::begin(data) + a*(nchannels*perchannel) + c*perchannel;
					}
				}
			}

			/** Multiplies every sample by _factor */
			virtual void Scale(const double& _factor) {
				assert(_factor > 0 );
				std::transform(std::begin(data), std::end(data), std::begin(data), [&_factor](uint16_t& elem) {
					return round2ui16(_factor * static_cast<double>(elem)); } );
			}
	};

	/** A shared pointer to a DaqMultiChunk */
	template<uint32_t NAREAS = 1, class DATA_T = uint16_t> using DaqMultiChunkPtr = std::shared_ptr<DaqMultiChunk<NAREAS, DATA_T>>;

}