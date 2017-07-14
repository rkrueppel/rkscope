#include "stdafx.h"
#include "PixelmapperFrameResonanceSW.h"
#include "parameters\Framescan.h"
#include "helpers\DaqChunkResonance.h"
#include "helpers\ScopeImage.h"
#include "helpers\ScopeMultiImageResonanceSW.h"
#include "controllers\ScopeController.h"

namespace scope {

PixelmapperFrameResonanceSW::PixelmapperFrameResonanceSW()
	: PixelmapperBasic(ScannerTypeHelper::Resonance, ScannerVectorTypeHelper::ResonanceBiDi)
	, firstchunk(true) {
}

PixelmapperResult PixelmapperFrameResonanceSW::LookupChunk(DaqChunkResonancePtr const _chunk, const uint16_t& _currentavgcount) {
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
	DaqChunkResonance::iterator channelend;
	DaqChunkResonance::iterator chunkit;
	DaqChunkResonance::iteratorSync syncit;
	
	ScopeController scope_controller;

	const uint32_t area = current_frame->Area();
	const size_t num_planes = scope_controller.GuiParameters.areas[area]->FrameResonance().planes.size()?scope_controller.GuiParameters.areas[area]->FrameResonance().planes.size():1;
	const uint32_t maxlines = ytotallines*num_planes;
	const uint32_t maximagepixels = totalimagepixels*num_planes; // 65536*np

	// Go through all channels
	for ( uint32_t c = 0 ; c < _chunk->NChannels() ; c++ ) {

		// Loop over the volume scanning planes
		for(size_t np = 0; np  < num_planes; np++) {
			size_t cyy = ytotallines*np;

			// Get the data
			ScopeImageAccessU16 imagedata(*current_frame->GetChannel(c)); //GetChannel : returns pointer to one channel image
			std::vector<uint16_t>* const dataptr(imagedata.GetData()); //dataptr gets the Pointer to imagedata(ScopeImage)'s data vector

			ScopeImageAccessU16 averagedimagedata(*current_averaged_frame->GetChannel(c));
			std::vector<uint16_t>* const averageddataptr(averagedimagedata.GetData());

			// Which sample did we map last in this chunk (initially std::begin) and at what position of the sync signal
		chunkit = _chunk->lastmapped[c];
		syncit = _chunk->lastsyncsig[c];
			// where does this channel end in the chunk's data vector
			channelend = std::begin(_chunk->data)+(c+1)*_chunk->PerChannel();
			// Which pixel did we last map into the image
			imagepos = current_frame->LastImagePos(c);
			forthline = current_frame->LastForthline(c);
		
			// find beginning of the first line of the first chunk
		if ( firstchunk ) {
			firstchunk = false;
				do {
					chunkit++;
					syncit++;
				} while ( (chunkit+1 != channelend) && !( *syncit == 1 && *(syncit+1) == 0 ));
				// To ensure that the display starts from the top frame, move back the pointers
				if(num_planes > 1)
				{
					chunkit = chunkit+ (num_planes-2)*totalimagepixels;
					syncit = syncit + (num_planes-2)*totalimagepixels;
				}
			}

			// always aligning on the left
			// y scanner is based on given number of lines
//// FOR L = loop over lines
			for ( l = current_frame->LastL(c) ; l < cyy + ytotallines ; l++ ) { 
				// x (resonant) scanner is based on the synchronization signal and waits until it switches from 1 to 0
				
				if ( forthline ) {
					// Loop over X-position
					for ( i = current_frame->LastX(c); (chunkit != channelend) && (i < (xtotalpixels - xturnpixelsright)); i++, syncit++, chunkit++ ) {
						if ( (l >= cyy + cutofflines) && (l < cyy + scanlines) && (i >= xturnpixelsleft) ) {
						//if ( (l >= cutofflines) && (l < scanlines) && (i >= xturnpixelsleft) ) {
							// write into the frame that is not averaged
							dataptr->operator[](imagepos) = *chunkit;

							// write into the averaged frame
							n = (static_cast<uint32_t>(averageddataptr->operator[](imagepos)) * multiplier) + static_cast<uint32_t>(*chunkit);
							averageddataptr->operator[](imagepos) = static_cast<uint16_t>(  n / divisor + ((n%divisor)>halfdivisor?1u:0u) );

							imagepos++;
						}
						lx = i;
					}
					current_frame->SetLastX(c, 0);
				}
				else {
					for ( i = current_frame->LastX(c); chunkit != channelend; i++, syncit++, chunkit++ ) {
						// save intermediately in a vector
						current_frame->CurrentLineData()->at(c).at(i) = *chunkit;
						if ( (chunkit+1 != channelend) && ( *syncit == 1 && *(syncit+1) == 0 ) ) {
							// fill in beginning at the end of the intermediate vector
							for ( uint32_t x = 0; x < ((i>=xtotalpixels-xturnpixelsright)?(xtotalpixels-xturnpixelsright):i); x++ ) {
								if ( (l >= cyy + cutofflines) && (l < cyy + scanlines) && (x >= xturnpixelsleft) ) {
								//if ( (l >= cutofflines) && (l < scanlines) && (x >= xturnpixelsleft) ) {
									imagepos--;
								
									// write into the frame that is not averaged
									dataptr->operator[](imagepos) = current_frame->CurrentLineData()->at(c).at(i - x - 1);

									// write into the averaged frame
									n = (static_cast<uint32_t>(averageddataptr->operator[](imagepos)) * multiplier) + static_cast<uint32_t>(current_frame->CurrentLineData()->at(c).at(i - x - 1));
									averageddataptr->operator[](imagepos) = static_cast<uint16_t>(  n / divisor + ((n%divisor)>halfdivisor?1u:0u) );
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

				if( chunkit == channelend ) {
					current_frame->SetLastX(c, lx);
					result = PixelmapperResult(result | EndOfChunk);
					break;
				}
				if ( (l >= cyy + cutofflines) && (l < cyy + scanlines) ) {
					if ( forthline )
						imagepos = ((l-(cyy+cutofflines)+2) * ximagepixels) + np*totalimagepixels;
					else
						imagepos = ((l-(cyy+cutofflines)+1) * ximagepixels) + np*totalimagepixels;
				}
				forthline = !forthline;
			}
//// END FOR L

			// Save in the chunk which sample was last mapped and the position of the sync signal
			_chunk->SetLastMapped(c, chunkit);
			_chunk->SetLastSyncSig(c, syncit);

			// save which pixel we last looked up
			if ( l >= maxlines ) {
				current_frame->SetLastImagepos(c, 0);
				current_frame->SetLastL(c, 0);
				current_frame->SetLastForthline(c, true);
				current_frame->SetLastX(c, 0);

				result =  PixelmapperResult(result | FrameComplete);
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

void PixelmapperFrameResonanceSW::SetCurrentAveragedFrame(ScopeMultiImageResonanceSWPtr const _current_averaged_frame) {
	current_averaged_frame = _current_averaged_frame;
}

}
