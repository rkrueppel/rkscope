#include "StdAfx.h"
#include "ScannerVectorFrameSaw.h"
#include "controllers/ScopeLogger.h"

namespace scope {

ScannerVectorFrameSaw::ScannerVectorFrameSaw(const ScannerVectorFillType& _filltype)
	: ScannerVectorFrameBasic(ScannerVectorTypeHelper::Sawtooth, _filltype) {
}

ScannerVectorFrameSaw::~ScannerVectorFrameSaw() {

}

void ScannerVectorFrameSaw::UpdateVector() {
#ifdef _DEBUG
	LARGE_INTEGER countspre;
	LARGE_INTEGER countspost;
	LARGE_INTEGER countfreq;
	QueryPerformanceCounter(&countspre);
#endif

	switch ( filltype ) {
	case ScannerVectorFillTypeHelper::FullframeXYZP:
		// interleaved samples for x,y,z,pockels
		vecptr->resize(4*svparameters->TotalPixels());
		FillX();
		FillY();
		FillZ();
		FillP();
		break;
	case ScannerVectorFillTypeHelper::LineXPColumnYZ:
		// interleaved samples for x,p + interleaved samples for y,z
		vecptr->resize(2*svparameters->XTotalPixels() + 2*svparameters->YTotalLines());
		FillXP();
		FillYZ();
		break;
	case ScannerVectorFillTypeHelper::LineZP:
		vecptr->resize(2*svparameters->TotalPixels());
		FillZ();
		FillP();
		break;
	default:
		throw ScopeException("ScannerVectorFillType not yet implemented");
	}

	lookup->resize(svparameters->TotalPixels());
	FillLookup();

#ifdef _DEBUG
	QueryPerformanceCounter(&countspost);
	QueryPerformanceFrequency(&countfreq);
	double time = static_cast<double>((countspost.QuadPart-countspre.QuadPart))/static_cast<double>(countfreq.QuadPart) * 1000;
	DBOUT(L"Time for updating vector " << time << L" ms");
#endif
}

void ScannerVectorFrameSaw::FillLookup() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
	const uint32_t cutofflines = tmp->YCutoffLines();
	const uint32_t scanlines = tmp->YScanLines();
	const uint32_t cutoffpixels = tmp->XCutoffPixels();
	const uint32_t scanpixels = tmp->XScanPixels();
	const uint32_t ytotallines = tmp->YTotalLines();
	const uint32_t xtotalpixels = tmp->XTotalPixels();
	uint32_t datapos = 0;
	uint32_t imagepos = 0;
	// advance datapos on every sampled pixel, advance imagepos only on pixels that are inside the image -> build up the lookup vector
	for ( uint32_t l = 0 ; l < ytotallines ; l++ ) {
		for ( uint32_t x = 0 ; x < xtotalpixels ; x++ ) {
			if ( (l >= cutofflines) && (l < scanlines) && (x >= cutoffpixels) && (x < scanpixels) )
				lookup->at(datapos) = imagepos++;
			else
				lookup->at(datapos) = 0;
			datapos++;
		}
	}

	// Adjust for the scannerdelay by rotating the lookup vector (do not respect oversampling, since lookup is done on downsampled data
	lookup_rotation = daqparameters->ScannerDelaySamples(false);
	if ( lookup_rotation > 0 )
		std::rotate(std::begin(*lookup), std::begin(*lookup)+lookup_rotation, std::end(*lookup));
	if ( lookup_rotation < 0 )
		std::rotate(std::begin(*lookup), std::end(*lookup)+lookup_rotation, std::end(*lookup));
}

void ScannerVectorFrameSaw::FillX() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
	const uint32_t framesamples(tmp->TotalPixels());
	const uint32_t linesamples(tmp->XTotalPixels());
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t
	const double zoom = tmp->zoom();
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
	double center = devicecenter + tmp->xoffset()*0.5*range;										// center of the frame, offset +-1 means maximum offset
	if ( center - 0.5*xrangezoomed < daqparameters->outputs->minoutputscanner() )
		center = daqparameters->outputs->minoutputscanner()+0.5*xrangezoomed;
	if ( center + 0.5*xrangezoomed > daqparameters->outputs->maxoutputscanner() )
		center = daqparameters->outputs->maxoutputscanner()-0.5*xrangezoomed;
	const double xminzoomed = center - 0.5*xrangezoomed;								// minimum x value after zoom
	const double xmaxzoomed = center + 0.5*xrangezoomed;								// maximum x value after zoom
	const double xscanslope = xrangezoomed / static_cast<double>(tmp->XScanPixels());
	const double xretraceslope = -xrangezoomed / static_cast<double>(tmp->XRetracePixels());
	const uint32_t xscansamples = 4*tmp->XScanPixels();
	const uint32_t xtotalsamples = 4*tmp->XTotalPixels();

	uint32_t x = 0;
	size_t cx = 0;

	do {
		// fill in XCutoffSamples()+XImageSamples() with increasing values (scanning) in X
		x = 0;
		for ( size_t i = cx ; i < cx+xscansamples ; i += 4, x++ )
			vecptr->at(i) = scaletodevice(xminzoomed + x * xscanslope);
		
		// fill in XRetraceSamples() with decreasing values (retracing) in X
		x = 0;
		for ( size_t i = cx+xscansamples ; i < cx+xtotalsamples ; i += 4, x++ )
			vecptr->at(i) = scaletodevice(xmaxzoomed + x * xretraceslope);

		cx += 4*linesamples;
	} while ( cx < 4*framesamples );
}

void ScannerVectorFrameSaw::FillY() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
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

void ScannerVectorFrameSaw::FillZ() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
	const uint32_t framesamples(tmp->TotalPixels());
	const uint32_t linesamples(tmp->XTotalPixels());
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());	// convert full device range to full range of int16_t
	const int16_t fastzoutdev = scaletodevice(zparameters->PositionToVoltage(svparameters->fastz()));					// get the voltage corresponding to current ETL position in micron and scale to device
	size_t cz, cz_init, step_size;
	// Decide vector storage locations and step_size based on Master/Slave
	if (filltype == ScannerVectorFillTypeHelper::FullframeXYZP) {
		cz = 2;	cz_init = 2; step_size = 4; 
	}
	if (filltype == ScannerVectorFillTypeHelper::LineZP) {
		cz = 0;	cz_init = 0; step_size = 2; 
	}

	do {
		// Fast z position stays constant during normal frame scanning	
		for ( size_t i = cz ; i < cz + step_size*linesamples ; i += step_size )
			vecptr->at(i) = fastzoutdev;
		cz += step_size*linesamples;
	} while ( cz < (step_size*framesamples + cz_init) );
}

void ScannerVectorFrameSaw::FillP() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
	const uint32_t framesamples(tmp->TotalPixels());
	const uint32_t linesamples(tmp->XTotalPixels());
	const uint32_t cutofflines = tmp->YCutoffLines();
	const uint32_t scanlines = tmp->YScanLines();
	const uint32_t cutoffpixels = tmp->XCutoffPixels();
	const uint32_t scanpixels = tmp->XScanPixels();
	const double pockelsoutval = (tmp->pockels()-tmp->pockels.ll())/(tmp->pockels.ul()-tmp->pockels.ll())
		*daqparameters->outputs->maxoutputpockels()+daqparameters->outputs->minoutputpockels();							// scale pockels value from displayed value (e.g. 0..1) to device value (e.g. 0..2)
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t
	size_t cp, cp_init, step_size;
	// Decide vector storage locations and step_size based on Master/Slave
	if (filltype == ScannerVectorFillTypeHelper::FullframeXYZP) {
		cp = 3;	cp_init = 3; step_size = 4; 
	}
	if (filltype == ScannerVectorFillTypeHelper::LineZP) {
		cp = 1;	cp_init = 1; step_size = 2; 
	}

	do {
		// Pockels blanking during Y cutoff and retrace
		if ( (cp < (cp_init + step_size*cutofflines*linesamples))
			|| (cp > (cp_init + step_size*scanlines*linesamples)) ) {
			for ( size_t i = cp ; i < cp + step_size*linesamples ; i += step_size )
				vecptr->at(i) = scaletodevice(0.0);
		}
		else {
			// Pockels blanking during X cutoff and X retrace, pockels during image
			for ( size_t i = cp ; i < cp+step_size*cutoffpixels ; i += step_size )
				vecptr->at(i) = scaletodevice(0.0);
			for ( size_t i = cp+step_size*cutoffpixels ; i < cp+step_size*scanpixels ; i += step_size )
				vecptr->at(i) = scaletodevice(pockelsoutval);
			for ( size_t i = cp+step_size*scanpixels ; i < cp+4*linesamples ; i += step_size )
				vecptr->at(i) = scaletodevice(0.0);
		}
		cp += step_size*linesamples;
	} while ( cp < (step_size*framesamples + cp_init) );

	// Adjust for the scannerdelay by rotating the pockels vector (do not respect oversampling, since pockels vector is with pixels, not oversampled input samples)
	const int32_t pockels_rotation = daqparameters->ScannerDelaySamples(false);
	if ( pockels_rotation > 0 ) {
		auto length = vecptr->size() / step_size;
		size_t p = 0;
		size_t pr = p + pockels_rotation;
		for ( uint32_t i = 0 ; i < length ; ++i ) {
			std::swap(vecptr->at(cp_init+step_size*p), vecptr->at(cp_init+step_size*pr));
			// Modulo does the turnaround
			p = (p+1)%length;
			pr = (pr+1)%length;
		}	
	}
	if ( pockels_rotation < 0 ) {
		auto length = vecptr->size() / step_size;
		size_t p = 0;					// from reverse p starts at 1
		size_t pr = p - pockels_rotation;
		for ( uint32_t i = 0 ; i < length ; ++i) {
			std::swap(vecptr->at(step_size*(length-p)-1), vecptr->at(step_size*(length-pr)-1));
			// Modulo does the turnaround
			p = (p+1)%length;
			pr = (pr+1)%length;
		}
	}
}

void ScannerVectorFrameSaw::FillXP() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t
	const double zoom = tmp->zoom();
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
	double center = devicecenter + tmp->xoffset()*0.5*range;										// center of the frame, offset +-1 means maximum offset
	if ( center - 0.5*xrangezoomed < daqparameters->outputs->minoutputscanner() )
		center = daqparameters->outputs->minoutputscanner()+0.5*xrangezoomed;
	if ( center + 0.5*xrangezoomed > daqparameters->outputs->maxoutputscanner() )
		center = daqparameters->outputs->maxoutputscanner()-0.5*xrangezoomed;
	const double xminzoomed = center - 0.5*xrangezoomed;								// minimum x value after zoom
	const double xmaxzoomed = center + 0.5*xrangezoomed;								// maximum x value after zoom
	const double xscanslope = xrangezoomed / static_cast<double>(tmp->XScanPixels());
	const double xretraceslope = -xrangezoomed / static_cast<double>(tmp->XRetracePixels());
	// *2 since samples for x and p are interleaved
	const uint32_t xcutoffsamples = 2*tmp->XCutoffPixels();
	const uint32_t ximagesamples = 2*tmp->XImagePixels();
	const uint32_t xtotalsamples = 2*tmp->XTotalPixels();
	// scale pockels value from displayed value (e.g. 0..1) to device value (e.g. 0..2 V)
	const double pockelsoutval = (tmp->pockels()-tmp->pockels.ll())/(tmp->pockels.ul()-tmp->pockels.ll())
		*daqparameters->outputs->maxoutputpockels()+daqparameters->outputs->minoutputpockels();

	uint32_t x = 0;
	const size_t cx = 0;		// xp interleaved starts at index 0

	// fill in XCutoffSamples()+XImageSamples() with increasing values (scanning) in X
	for ( size_t i = cx ; i < cx+xcutoffsamples ; i += 2, ++x ) {
		// x galvo ramps up
		vecptr->at(i) = scaletodevice(xminzoomed + x * xscanslope);
		// Pockels cell is blanked/closed
		vecptr->at(i+1) = 0;
	}
	for ( size_t i = cx+xcutoffsamples ; i < cx+xcutoffsamples+ximagesamples ; i += 2, ++x ) {
		// x galvo ramps further up
		vecptr->at(i) = scaletodevice(xminzoomed + x * xscanslope);
		// Pockels cell is open now
		vecptr->at(i+1) = scaletodevice(pockelsoutval);
	}
	// fill in XRetraceSamples() with decreasing values (retracing) in X
	x = 0;	// restart sloping
	for ( size_t i = cx+xcutoffsamples+ximagesamples ; i < cx+xtotalsamples ; i += 2, ++x ) {
		// x galvo quickly ramps down
		vecptr->at(i) = scaletodevice(xmaxzoomed + x * xretraceslope);
		// Pockels cell blanked/closed again
		vecptr->at(i+1) = 0;
	}

	// Adjust for the scannerdelay by rotating the pockels vector (do not respect oversampling, since pockels vector is with pixels, not oversampled input samples)
	const int32_t pockels_rotation = daqparameters->ScannerDelaySamples(false);
	if ( pockels_rotation > 0 ) {
		auto length = xtotalsamples;
		size_t p = 1;								// pockels starts at 2nd sample
		size_t pr = p + pockels_rotation*2;			// every 2nd sample is pockels
		for ( uint32_t i = 0 ; i < length ; ++i ) {
			std::swap(vecptr->at(p), vecptr->at(pr));
			// Modulo does the turnaround
			p = (p+2)%length;
			pr = (pr+2)%length;
		}	
	}
	if ( pockels_rotation < 0 ) {
		auto length = xtotalsamples;
		size_t p = 0;							// from reverse pockels starts at 0
		size_t pr = p + pockels_rotation*2;		// every 2nd sample is pockels
		for ( uint32_t i = 0 ; i < length ; ++i) {
			std::swap(vecptr->at(length-p), vecptr->at(length-pr));
			// Modulo does the turnaround
			p = (p+2)%length;
			pr = (pr+2)%length;
		}
	}
}

void ScannerVectorFrameSaw::FillYZ() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
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
	const double yretraceslope = -yrangezoomed / static_cast<double>(tmp->YRetraceLines());
	// *2 since samples for y and z are interleaved
	const uint32_t ycutoffsamples = 2*tmp->YCutoffLines();
	const uint32_t yimagesamples = 2*tmp->YImageLines();
	const uint32_t ytotalsamples = 2*tmp->YTotalLines();

	const Scaler<int16_t> scaletodevicez(-daqparameters->outputs->range(), daqparameters->outputs->range());	// convert full device range to full range of int16_t
	const int16_t fastzoutdev = scaletodevicez(zparameters->PositionToVoltage(svparameters->fastz()));					// get the voltage corresponding to current ETL position in micron an

	uint32_t y = 0;
	const size_t cy = 2*tmp->XTotalPixels();		// yz interleaved starts after 2*XTotalPixels (for xp)

	// fill in YCutoffSamples()+YImageSamples() with increasing values (scanning) in Y
	for ( size_t i = cy ; i < cy+ycutoffsamples+yimagesamples ; i += 2, ++y ) {
		// y galvo ramps up
		vecptr->at(i) = scaletodevice(yminzoomed + y * yscanslope);
		// fast z stays
		vecptr->at(i+1) = fastzoutdev;
	}
	// fill in YRetraceSamples() with decreasing values (retracing) in Y
	y = 0;	// restart sloping
	for ( size_t i = cy+ycutoffsamples+yimagesamples ; i < cy+ytotalsamples ; i += 2, ++y ) {
		// y galvo quickly ramps down
		vecptr->at(i) = scaletodevice(ymaxzoomed + y * yretraceslope);
		// fast z stays
		vecptr->at(i+1) = fastzoutdev;
	}

}

void ScannerVectorFrameSaw::FillZP() {
	parameters::ScannerVectorFrameSaw* tmp = dynamic_cast<parameters::ScannerVectorFrameSaw*>(svparameters);
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t
	const int16_t pockelsoutval = scaletodevice((tmp->pockels()-tmp->pockels.ll())/(tmp->pockels.ul()-tmp->pockels.ll())
		*daqparameters->outputs->maxoutputpockels()+daqparameters->outputs->minoutputpockels());
	const int16_t fastzoutdev = scaletodevice(zparameters->PositionToVoltage(svparameters->fastz()));					// get the voltage corresponding to current ETL position in micron an
	// *2 since samples for z and p are interleaved
	const uint32_t xcutoffsamples = 2*tmp->XCutoffPixels();
	const uint32_t ximagesamples = 2*tmp->XImagePixels();
	const uint32_t xtotalsamples = 2*tmp->XTotalPixels();

	// Fast z is the same the whole line, Pockels is closed/0 during cutoff and retrace
	for ( size_t i = 0 ; i < xcutoffsamples ; i+=2 ) {
		vecptr->operator[](i) = fastzoutdev;
		vecptr->operator[](i+1) = 0;
	}
	for ( size_t i = xcutoffsamples ; i < xcutoffsamples+ximagesamples ; i+=2 ) {
		vecptr->operator[](i) = fastzoutdev;
		vecptr->operator[](i+1) = pockelsoutval;
	}
	for ( size_t i = xcutoffsamples+ximagesamples ; i < xtotalsamples ; i+=2 ) {
		vecptr->operator[](i) = fastzoutdev;
		vecptr->operator[](i+1) = 0;
	}

}

void ScannerVectorFrameSaw::RotateP(const size_t& _pstart, const size_t& _interleavedfactor, const int32_t& _rotateby) {
	const size_t length = vecptr->size();
	if ( _rotateby > 0 ) {
		size_t p = _pstart;
		size_t pr = p + _rotateby*_interleavedfactor;
		for ( uint32_t i = 0 ; i < length ; i += _interleavedfactor ) {
			std::swap(vecptr->at(p), vecptr->at(pr));
			// Modulo does the turnaround
			p = (p+_interleavedfactor)%length;
			pr = (pr+_interleavedfactor)%length;
		}	
	}
	if ( _rotateby < 0 ) {
		size_t p = 0;							// from reverse pockels starts at 0
		size_t pr = p + _rotateby*_interleavedfactor;
		for ( uint32_t i = 0 ; i < vecptr->size() ; i += _interleavedfactor ) {
			std::swap(vecptr->at(length-p), vecptr->at(length-pr));
			// Modulo does the turnaround
			p = (p+_interleavedfactor)%length;
			pr = (pr+_interleavedfactor)%length;
		}
	}
}

}