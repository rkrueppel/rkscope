#include "stdafx.h"
#include "ScannerVectorFrameResonanceBiDi.h"

namespace scope {

ScannerVectorFrameResonanceBiDi::ScannerVectorFrameResonanceBiDi(const ScannerVectorFillType& _filltype)
	: ScannerVectorFrameBasic(ScannerVectorTypeHelper::ResonanceBiDi, _filltype) {
}

void ScannerVectorFrameResonanceBiDi::UpdateVector() {
	switch ( filltype ) {
	case ScannerVectorFillTypeHelper::FullframeXYZP:
		// interleaved samples for x,y,z,pockels
		// shorter vector than in other framescan modes, because the clock is the resonance scanner sync signal here
		vecptr->resize(4*svparameters->YTotalLines() / 2);
		FillY();
		FillZ();
		FillP();
		break;
	case ScannerVectorFillTypeHelper::LineXPColumnYZ:
		break;
	case ScannerVectorFillTypeHelper::LineZP:
		vecptr->resize(2*svparameters->YTotalLines() / 2);
		FillZ();
		FillP();
		break;
	default:
		throw ScopeException("ScannerVectorFillType not yet implemented");
	}

	lookup->resize(svparameters->YTotalLines() / 2);
}

void ScannerVectorFrameResonanceBiDi::FillY() {
	parameters::ScannerVectorFrameResonance* tmp = dynamic_cast<parameters::ScannerVectorFrameResonance*>(svparameters);
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t

	const double zoom = tmp->zoom();
	const double range = daqparameters->outputs->maxoutputscanner() - daqparameters->outputs->minoutputscanner();
	const double rangezoomed = range / zoom;
	
	// choose the maximum scanner amplitude for the larger frame side
	double yrangezoomed(1.0);
	if(tmp->xaspectratio >= tmp->yaspectratio) {
		yrangezoomed = static_cast<double>(tmp->yaspectratio) / static_cast<double>(tmp->xaspectratio) * rangezoomed;
	}
	else {
		yrangezoomed = zoom * tmp->yaspectratio  / (tmp->xaspectratio * static_cast<double>(round2ui32(tmp->yaspectratio / tmp->xaspectratio * zoom))) * rangezoomed;
	}

	const double devicecenter = (daqparameters->outputs->minoutputscanner() + daqparameters->outputs->maxoutputscanner()) * 0.5;	// center of the device range (e.g. scanner takes +-3V -> center is 0V)
	double center = devicecenter + tmp->yoffset()*0.5*range;										// center of the frame, offset +-1 means maximum offset
	if ( center - 0.5*yrangezoomed < daqparameters->outputs->minoutputscanner() )
		center = daqparameters->outputs->minoutputscanner()+0.5*yrangezoomed;
	if ( center + 0.5*yrangezoomed > daqparameters->outputs->maxoutputscanner() )
		center = daqparameters->outputs->maxoutputscanner()-0.5*yrangezoomed;
	const double yminzoomed = center - 0.5*yrangezoomed;								// minimum y value after zoom
	const double ymaxzoomed = center + 0.5*yrangezoomed;								// maximum y value after zoom

	// The slope is related to the number of lines and not to the number of pixels, as in the other framescan modes
	// (because the sync signal of the resonance scanner indicates a new line and it triggers the y scanner signal)
	const double yscanslope = yrangezoomed / static_cast<double>(tmp->YScanLines());
	const double yretraceslope = -yrangezoomed / static_cast<double>(tmp->YRetraceLines());
	const uint32_t yscanlines = 4*tmp->YScanLines()/2;
	const uint32_t ytotallines = 4*tmp->YTotalLines()/2;

	// fill in YScanLines() with increasing values (scanning) in Y
	size_t cy = 0;	
	uint32_t y = 0;
	for (size_t i = cy; i < yscanlines ; i+= 4, y+= 2 )
		vecptr->at(i) = scaletodevice(yminzoomed + y * yscanslope);
	y = 0;
	// fill in YRetraceLines() with decreasing values (retracing) in Y
	for (size_t i = cy + yscanlines ; i < ytotallines ; i+= 4, y+= 2 )
		vecptr->at(i) = scaletodevice(ymaxzoomed + y * yretraceslope);
}

void ScannerVectorFrameResonanceBiDi::FillZ() {
	parameters::ScannerVectorFrameResonance* tmp = dynamic_cast<parameters::ScannerVectorFrameResonance*>(svparameters);
	const uint32_t cutofflines = tmp->YCutoffLines()/2;
	const uint32_t scanlines = tmp->YScanLines()/2;
	const uint32_t totallines = tmp->YTotalLines()/2;
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());
	const int16_t fastzoutdev = scaletodevice(zparameters->PositionToVoltage(svparameters->fastz()));		// get the voltage corresponding to current ETL position in micron and scale to device

	// Decide vector storage locations and step_size based on Master/Slave (i.e. Fullframe or LineZP)
	size_t cz(1);
	size_t step_size(2);

	if (filltype == ScannerVectorFillTypeHelper::FullframeXYZP) {
		cz = 1;	step_size = 4;
	}
	if (filltype == ScannerVectorFillTypeHelper::LineZP) {
		cz = 0;	step_size = 2;
	}
	
	double z1(zparameters->PositionToVoltage(tmp->fastz()));
	for ( size_t i = cz ; i < cz + step_size*totallines ; i += step_size )
		vecptr->at(i) = scaletodevice(z1);
	
}

void ScannerVectorFrameResonanceBiDi::FillP() {
	parameters::ScannerVectorFrameResonance* tmp = dynamic_cast<parameters::ScannerVectorFrameResonance*>(svparameters);
	const uint32_t cutofflines = tmp->YCutoffLines() / 2;
	const uint32_t scanlines = tmp->YScanLines() / 2;
	const uint32_t totallines = tmp->YTotalLines() / 2;
	// convert full device range to full range of int16_t
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());
	// get the voltage corresponding to current etl position in micron and scale to device
	const int16_t pockelsdev = scaletodevice(tmp->pockels());

	// Decide vector storage locations and step_size based on Master/Slave (i.e. Fullframe or LineZP)
	size_t cp(1);
	size_t cp_init(1);
	size_t step_size(2);

	if (filltype == ScannerVectorFillTypeHelper::FullframeXYZP) {
		cp = 1;	cp_init = 1; step_size = 4; 
	}
	if (filltype == ScannerVectorFillTypeHelper::LineZP) {
		cp = 1;	cp_init = 1; step_size = 2; 
	}

	for ( size_t i = cp ; i < cp + step_size*cutofflines ; i += step_size )
		vecptr->at(i) = scaletodevice(0.0);
	for ( size_t i = cp + step_size*cutofflines ; i < cp + step_size*scanlines ; i += step_size )
		vecptr->at(i) = pockelsdev;
	for ( size_t i = cp + step_size*scanlines ; i < cp + step_size*totallines ; i += step_size )
		vecptr->at(i) = scaletodevice(0.0);
}



}