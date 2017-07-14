#include "stdafx.h"
#include "ScannerVectorFrameResonanceHopper.h"
#include "controllers\ScopeController.h"

namespace scope {

ScannerVectorFrameResonanceHopper::ScannerVectorFrameResonanceHopper(const ScannerVectorFillType& _filltype)
	: ScannerVectorFrameBasic(ScannerVectorTypeHelper::ResonanceHopper, _filltype) {
}


void ScannerVectorFrameResonanceHopper::UpdateVector() {
	ScopeController scope_controller;
	uint32_t area = (filltype == ScannerVectorFillTypeHelper::FullframeXYZP)?0:1;
	size_t num_planes = (scope_controller.GuiParameters.areas[area]->FrameResonance().planes.size())?scope_controller.GuiParameters.areas[area]->FrameResonance().planes.size():1;	
	switch ( filltype ) {
	case ScannerVectorFillTypeHelper::FullframeXYZP:
		// interleaved samples for x,y,z,pockels
		vecptr->resize(4*num_planes*svparameters->YTotalLines());
		FillY();
		FillZ();
		FillP();
		break;
	case ScannerVectorFillTypeHelper::LineXPColumnYZ:
		break;
	case ScannerVectorFillTypeHelper::LineZP:
		vecptr->resize(2*num_planes*svparameters->YTotalLines() / 2);
		FillZ();
		FillP();
		break;
	default:
		throw ScopeException("ScannerVectorFillType not yet implemented");
	}

	lookup->resize(num_planes * svparameters->YTotalLines() / 2);
}

void ScannerVectorFrameResonanceHopper::FillY() {
	uint32_t area = (filltype == ScannerVectorFillTypeHelper::FullframeXYZP)?0:1;
	//size_t num_planes = (scope_controller.GuiParameters.areas[area]->frameresonance.planes.size())?scope_controller.GuiParameters.areas[area]->frameresonance.planes.size():1;	

	parameters::ScannerVectorFrameResonance* tmp = dynamic_cast<parameters::ScannerVectorFrameResonance*>(svparameters);
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());		// convert full device range to full range of int16_t
	//const Scaler<int16_t> scaletodevice(daqparameters->outputs->minoutputscanner(), daqparameters->outputs->maxoutputscanner());
	const double zoom = tmp->zoom();
	const double range = daqparameters->outputs->maxoutputscanner() - daqparameters->outputs->minoutputscanner();
	const double rangezoomed = range / zoom;
	
	// choose the maximum scanner amplitude for the larger frame side
	double yrangezoomed;
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

	size_t cy = 0;
/*	for(size_t n = 1; n  <= num_planes; n++)
	{
		size_t cyy = ytotallines*(n-1);
		// fill in YScanLines() with increasing values (scanning) in Y
		uint32_t y = 0;
		for (size_t i = cy + cyy ; i < cyy + yscanlines ; i+= 4, y+= 2 )
			vecptr->at(i) = scaletodevice(yminzoomed + y * yscanslope);
		y = 0;	
		// fill in YRetraceLines() with decreasing values (retracing) in Y
		for (size_t i = cy + cyy + yscanlines ; i < cyy + ytotallines ; i+= 4, y+= 2 )
			vecptr->at(i) = scaletodevice(ymaxzoomed + y * yretraceslope);
	}
	*/
}

void ScannerVectorFrameResonanceHopper::FillZ() {
	uint32_t area = (filltype == ScannerVectorFillTypeHelper::FullframeXYZP)?0:1;
	//size_t num_planes = (scope_controller.GuiParameters.areas[area]->frameresonance.planes.size())?scope_controller.GuiParameters.areas[area]->frameresonance.planes.size():1;	

	parameters::ScannerVectorFrameResonance* tmp = dynamic_cast<parameters::ScannerVectorFrameResonance*>(svparameters);
	const uint32_t cutofflines = tmp->YCutoffLines()/2;
	const uint32_t scanlines = tmp->YScanLines()/2;
	const uint32_t totallines = tmp->YTotalLines()/2;
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());
	const int16_t fastzoutdev = scaletodevice(zparameters->PositionToVoltage(svparameters->fastz()));		// get the voltage corresponding to current ETL position in micron and scale to device
	double z1, z2,zretraceslope;
	size_t cz, step_size; 
	if (filltype == ScannerVectorFillTypeHelper::FullframeXYZP) {
		cz = 1;	step_size = 4;
	}
	if (filltype == ScannerVectorFillTypeHelper::LineZP) {
		cz = 0;	step_size = 2;
	}

	/*if(num_planes == 1)
	{
		z1 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[0].position());
		for ( size_t i = cz ; i < cz + step_size*totallines ; i += step_size )
			vecptr->at(i) = scaletodevice(z1);
	}
	else*/
	/*
	for(size_t n = 1; n  <= num_planes; n++)
	{
		size_t czz = step_size*totallines*(n-1);
		z1 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[n-1].position());
		if(n == num_planes) // If last plane, start from beginning
			z2 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[0].position());
		else 
			z2 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[n].position());

		//if(num_planes == 1)
		//	z2 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[n-1].position());


		zretraceslope = (z2 - z1)/(step_size*totallines - step_size*scanlines);
		size_t slope_step = (step_size*totallines - step_size*scanlines)/10; // number of steps for slope increment or decrement

		uint32_t z = step_size/2;
		for ( size_t i = cz + czz ; i < cz + czz + step_size*scanlines ; i += step_size )
			vecptr->at(i) = scaletodevice(z1);
		for ( size_t i = cz + czz + step_size*scanlines ; i < cz + czz + step_size*totallines ; i += step_size, z+= slope_step)//step_size/2 )
			vecptr->at(i) = scaletodevice(z1 + (zretraceslope*z));
		}
	}
	*/
}

void ScannerVectorFrameResonanceHopper::FillP() {

	uint32_t area = (filltype == ScannerVectorFillTypeHelper::FullframeXYZP)?0:1;
	//size_t num_planes = (scope_controller.GuiParameters.areas[area]->frameresonance.planes.size())?scope_controller.GuiParameters.areas[area]->frameresonance.planes.size():1;	
	parameters::ScannerVectorFrameResonance* tmp = dynamic_cast<parameters::ScannerVectorFrameResonance*>(svparameters);
	const uint32_t cutofflines = tmp->YCutoffLines()/2;
	const uint32_t scanlines = tmp->YScanLines()/2;
	const uint32_t totallines = tmp->YTotalLines()/2;
	const Scaler<int16_t> scaletodevice(-daqparameters->outputs->range(), daqparameters->outputs->range());
	const int16_t fastzoutdev = scaletodevice(zparameters->PositionToVoltage(svparameters->fastz()));		// get the voltage corresponding to current ETL position in micron and scale to device
	const int16_t pockelsdev = scaletodevice(tmp->pockels());

	double z1, z2,zretraceslope;
	size_t cz, step_size; 
	if (filltype == ScannerVectorFillTypeHelper::FullframeXYZP) {
		cz = 1;	step_size = 4;
	}
	if (filltype == ScannerVectorFillTypeHelper::LineZP) {
		cz = 0;	step_size = 2;
	}

	/*if(num_planes == 1)
	{
		z1 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[0].position());
		for ( size_t i = cz ; i < cz + step_size*totallines ; i += step_size )
			vecptr->at(i) = scaletodevice(z1);
	}
	else*/
	/*for(size_t n = 1; n  <= num_planes; n++)
	{
		size_t czz = step_size*totallines*(n-1);
		z1 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[n-1].position());
		if(n == num_planes) // If last plane, start from beginning
			z2 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[0].position());
		else 
			z2 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[n].position());

		//if(num_planes == 1)
		//	z2 = zparameters->PositionToVoltage(scope_controller.GuiParameters.areas[area]->frameresonance.planes[n-1].position());


		zretraceslope = (z2 - z1)/(step_size*totallines - step_size*scanlines);
		size_t slope_step = (step_size*totallines - step_size*scanlines)/10; // number of steps for slope increment or decrement

		uint32_t z = step_size/2;
		for ( size_t i = cz + czz ; i < cz + czz + step_size*scanlines ; i += step_size )
			vecptr->at(i) = scaletodevice(z1);
		for ( size_t i = cz + czz + step_size*scanlines ; i < cz + czz + step_size*totallines ; i += step_size, z+= slope_step)//step_size/2 )
			vecptr->at(i) = scaletodevice(z1 + (zretraceslope*z));
		}
	}
	*/
	//size_t num_planes = (scope_controller.GuiParameters.areas[area]->frameresonance.planes.size())?scope_controller.GuiParameters.areas[area]->frameresonance.planes.size():1;	

	
	

	// Decide vector storage locations and step_size based on Master/Slave
	/*size_t cp, step_size; 
	if (filltype == ScannerVectorFillTypeHelper::FullframeXYZP) {
		cp = 3;	step_size = 4;
	}
	if (filltype == ScannerVectorFillTypeHelper::LineZP) {
		cp = 1;	step_size = 2;
	}*/

	/*for(size_t n = 1; n  <= num_planes; n++)
	{
		size_t cpp = step_size*totallines*(n-1);
		double p = scope_controller.GuiParameters.areas[area]->frameresonance.planes[n-1].pockels();
		for ( size_t i = cp + cpp; i < cp + cpp + step_size*cutofflines ; i += step_size )
			vecptr->at(i) = scaletodevice(0.0);
		for ( size_t i = cp + cpp + step_size*cutofflines ; i < cp + cpp + step_size*scanlines ; i += step_size )
			vecptr->at(i) = scaletodevice(p);//pockelsdev;
		for ( size_t i = cp + cpp + step_size*scanlines ; i < cp + cpp + step_size*totallines ; i += step_size )
			vecptr->at(i) = scaletodevice(0.0);
	}
	*/
}



}