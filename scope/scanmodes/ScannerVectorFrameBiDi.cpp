#include "stdafx.h"
#include "ScannerVectorFrameBiDi.h"

namespace scope {
ScannerVectorFrameBiDi::ScannerVectorFrameBiDi(const ScannerVectorFillType& _filltype)
	: ScannerVectorFrameBasic(ScannerVectorTypeHelper::Bidirectional, _filltype) {
}

ScannerVectorFrameBiDi::~ScannerVectorFrameBiDi() {

}

void ScannerVectorFrameBiDi::UpdateVector() {
	// samples for x,y,z,pockels
	vecptr->resize(4 * svparameters->TotalPixels());
	lookup->resize(svparameters->TotalPixels());

	FillX();
	FillY();
	FillZ();
	FillP();
	FillLookup();
}

void ScannerVectorFrameBiDi::FillLookup() {
	parameters::ScannerVectorFrameBiDi* tmp = dynamic_cast<parameters::ScannerVectorFrameBiDi*>(svparameters);
	const uint32_t xturnpixels = tmp->XTurnPixels();
	const uint32_t cutofflines = tmp->YCutoffLines();
	const uint32_t scanlines = tmp->YScanLines();
	const uint32_t ytotallines = tmp->YTotalLines();
	const uint32_t xtotalpixels = tmp->XTotalPixels();
	uint32_t datapos = 0;
	uint32_t imagepos = 0;
	bool forthline = true;
	// advance datapos on every sampled pixel, advance imagepos only on pixels that are inside the image -> build up the lookup vector
	for ( uint32_t l = 0 ; l < ytotallines ; l++ ) {
		for ( uint32_t x = 0 ; x < xtotalpixels ; x++ ) {
			// only pixels after y cutoff and before y retrace, and after the first 0.5*xturnpixels and before the last 0.5*xturnpixels (?)
			if ( (l >= cutofflines) && (l < scanlines) && (x >= xturnpixels)) {
				if ( forthline )
					lookup->at(datapos) = imagepos++;
				else
					lookup->at(datapos) = --imagepos;		// backline is opposite direction
			}
			else				// Discard y cutoff and retrace pixels, and x turnpixels
				lookup->at(datapos) = 0;

			datapos++;
		}
		if ( (l >= cutofflines) && (l < scanlines) )
			imagepos += xtotalpixels - xturnpixels;
		forthline = !forthline;
	}
	// Adjust for the scannerdelay by rotating the lookup vector (do not respect oversampling, since lookup is done on downsampled data
	lookup_rotation = daqparameters->ScannerDelaySamples(false);
	if ( lookup_rotation > 0 )
		std::rotate(std::begin(*lookup), std::begin(*lookup)+lookup_rotation, std::end(*lookup));
	if ( lookup_rotation < 0 )
		std::rotate(std::begin(*lookup), std::end(*lookup)+lookup_rotation, std::end(*lookup));
}

void ScannerVectorFrameBiDi::FillX() {
	parameters::ScannerVectorFrameBiDi* tmp = dynamic_cast<parameters::ScannerVectorFrameBiDi*>(svparameters);
	const uint32_t framesamples(svparameters->TotalPixels());										// fill y forth and back with the same x stuff
	const uint32_t linesamples(svparameters->XTotalPixels());
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t
	const double zoom = svparameters->zoom();
	const double range = daqparameters->outputs->maxoutputscanner() - daqparameters->outputs->minoutputscanner();
	const double rangezoomed = range / zoom;

	// choose the maximum scanner amplitude for the larger frame side
	double xrangezoomed;
	if(tmp->xaspectratio >= tmp->yaspectratio) {
		xrangezoomed = rangezoomed;
	}
	else {
		xrangezoomed = static_cast<double>(tmp->xaspectratio) / static_cast<double>(tmp->yaspectratio) * rangezoomed;
	}
	const double devicecenter = (daqparameters->outputs->minoutputscanner() + daqparameters->outputs->maxoutputscanner()) * 0.5;	// center of the device range (e.g. scanner takes +-3V -> center is 0V)
	double center = devicecenter + svparameters->xoffset()*0.5*range;										// center of the frame, offset +-1 means maximum offset
	if ( center - 0.5*xrangezoomed < daqparameters->outputs->minoutputscanner() )
		center = daqparameters->outputs->minoutputscanner()+0.5*xrangezoomed;
	if ( center + 0.5*xrangezoomed > daqparameters->outputs->maxoutputscanner() )
		center = daqparameters->outputs->maxoutputscanner()-0.5*xrangezoomed;
	const double xminzoomed = center - 0.5*xrangezoomed;								// minimum x value after zoom
	const double xmaxzoomed = center + 0.5*xrangezoomed;								// maximum x value after zoom
	const double xslope = xrangezoomed / static_cast<double>(svparameters->XTotalPixels());
	
	uint32_t x = 0;
	size_t cx = 0;

	do {
		// fill in XTotalPixels() with increasing values (scanning forth) in X
		x = 0;
		for ( size_t i = cx ; i < cx+4*svparameters->XTotalPixels() ; i += 4, x++ )
			vecptr->at(i) = scaletodevice(xminzoomed + x * xslope);

		cx += 4*linesamples;
		if ( cx >= 4*framesamples)			// for odd total number of lines, break do not scan back (although bad for scanner
			break;

		// fill in XTotalPixels() with decreasing values (scanning back) in X
		for ( size_t i = cx ; i < cx+4*svparameters->XTotalPixels() ; i += 4, x-- )
			vecptr->at(i) = scaletodevice(xminzoomed + x * xslope);

		cx += 4*linesamples;
	} while ( cx < 4*framesamples );
}


void ScannerVectorFrameBiDi::FillY() {
	parameters::ScannerVectorFrameBiDi* tmp = dynamic_cast<parameters::ScannerVectorFrameBiDi*>(svparameters);
	const uint32_t framesamples(tmp->TotalPixels());
	const uint32_t linesamples(tmp->XTotalPixels());
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t
	const double zoom = tmp->zoom();
	const double range = daqparameters->outputs->maxoutputscanner() - daqparameters->outputs->minoutputscanner();
	const double rangezoomed = range / zoom;
	
	// choose the maximum scanner amplitude for the larger frame side
	double yrangezoomed;
	if(tmp->xaspectratio >= tmp->yaspectratio) {
		yrangezoomed = static_cast<double>(tmp->yaspectratio) / static_cast<double>(tmp->xaspectratio) * rangezoomed;
	}
	else {
		yrangezoomed = rangezoomed;
	}
	const double devicecenter = (daqparameters->outputs->minoutputscanner() + daqparameters->outputs->maxoutputscanner()) * 0.5;	// center of the device range (e.g. scanner takes +-3V -> center is 0V)
	double center = devicecenter + tmp->yoffset()*0.5*range;										// center of the frame, offset +-1 means maximum offset
	if ( center - 0.5*yrangezoomed < daqparameters->outputs->minoutputscanner() )
		center = daqparameters->outputs->minoutputscanner()+0.5*yrangezoomed;
	if ( center + 0.5*yrangezoomed > daqparameters->outputs->maxoutputscanner() )
		center = daqparameters->outputs->maxoutputscanner()-0.5*yrangezoomed;
	const double yminzoomed = center - 0.5*yrangezoomed;								// minimum y value after zoom
	const double ymaxzoomed = center + 0.5*yrangezoomed;								// maximum y value after zoom
	const double yscanslope = yrangezoomed / static_cast<double>(tmp->YScanLines());
	const double yretraceslope = -yrangezoomed / static_cast<double>(tmp->YRetraceLines() * tmp->XTotalPixels());
	const uint32_t yscanlinesamples = 4*tmp->YScanLines()*linesamples;
	uint32_t yscan = 0;
	uint32_t yretrace = 0;
	size_t cy = 1;																			// y starts at sample 1

	do {
		// fill in linesamples with the same value for scanning
		if ( cy < (1 + yscanlinesamples) ) {
			for ( size_t i = cy ; i < cy+4*linesamples ; i += 4 )
				vecptr->at(i) = scaletodevice(yminzoomed + yscan * yscanslope);
			yscan++;
		}
		else {
			// do a smoother retracing
			for ( size_t i = cy ; i < cy+4*linesamples ; i += 4 ) {
				vecptr->at(i) = scaletodevice(ymaxzoomed + yretrace * yretraceslope);
				yretrace++;
			}
		}
		cy += 4*linesamples;
	} while ( cy < (4*framesamples + 1) );
}

void ScannerVectorFrameBiDi::FillZ() {
	//parameters::ScannerVectorFrameBiDi* tmp = dynamic_cast<parameters::ScannerVectorFrameBiDi*>(svparameters);
	const uint32_t framesamples(svparameters->TotalPixels());
	const uint32_t linesamples(svparameters->XTotalPixels());
	// convert full device range to full range of int16_t
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());
	// get the voltage corresponding to current ETL position in micron and scale to device
	const int16_t fastzoutdev = scaletodevice(zparameters->PositionToVoltage(svparameters->fastz()));
	// z starts at sample 2
	size_t cz = 2;

	do {
		// Fast z position stays constant during normal frame scanning	
		for ( size_t i = cz ; i < cz + 4*linesamples ; i += 4 )
			vecptr->at(i) = fastzoutdev;
		cz += 4*linesamples;
	} while ( cz < (4*framesamples + 2) );
}

void ScannerVectorFrameBiDi::FillP() {
	parameters::ScannerVectorFrameBiDi* tmp = dynamic_cast<parameters::ScannerVectorFrameBiDi*>(svparameters);
	const uint32_t framesamples(svparameters->TotalPixels());
	const uint32_t linesamples(svparameters->XTotalPixels());
	const uint32_t cutofflines = tmp->YCutoffLines();
	const uint32_t scanlines = tmp->YScanLines();
	// convert full device range to full range of int16_t
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());
	// get the voltage corresponding to current ETL position in micron and scale to device
	const int16_t pockelsdev = scaletodevice(tmp->pockels());
	// pockels starts at sample 3
	size_t cp = 3;

	do {
		// Pockels blanking during Y cutoff and retrace
		if ( (cp < (3 + 4*cutofflines*linesamples))
			|| (cp > (3 + 4*scanlines*linesamples)) ) {
			for ( size_t i = cp ; i < cp + 4*linesamples ; i += 4 )
				vecptr->at(i) = scaletodevice(0.0);
		}
		else {
			// Pockels stays constant during scan (no x-flyback blanking needed)
			for ( size_t i = cp ; i < cp + 4*linesamples ; i += 4 )
				vecptr->at(i) = pockelsdev;
		}
		cp += 4*linesamples;
	} while ( cp < (4*framesamples + 2) );
}


}
