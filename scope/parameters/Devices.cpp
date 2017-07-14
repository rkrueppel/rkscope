#include "stdafx.h"
#include "Devices.h"
#include "Scope.h"

namespace scope {
	
namespace parameters {

// Save some typing here...
using namespace boost::property_tree;

XYZControl::XYZControl()
	: xpos(0, -10000, 10000, L"XPosition_um")
	, ypos(0, -10000, 10000, L"YPosition_um")
	, zpos(0, -10000, 10000, L"ZPosition_um")
	, pollinterval(1000, 0, 10000, L"PollInterval_ms") {
}

void XYZControl::Load(const wptree& pt) {
	xpos.SetFromPropertyTree(pt);
	ypos.SetFromPropertyTree(pt);
	zpos.SetFromPropertyTree(pt);
	pollinterval.SetFromPropertyTree(pt);
}

void XYZControl::Save(wptree& pt) const {
	xpos.AddToPropertyTree(pt);
	ypos.AddToPropertyTree(pt);
	zpos.AddToPropertyTree(pt);
	pollinterval.AddToPropertyTree(pt);
}

XYZControlGalil::XYZControlGalil()
	: comstring(L"COM7 19200", L"RS232Connection")
	, xcountspermicron(2, 1.0, 100, L"XEncoderCountsPerMicrometer")
	, ycountspermicron(2, 1.0, 100.0, L"YEncoderCountsPerMicrometer")
	, zcountspermicron(4, 1.0, 100.0, L"ZEncoderCountsPerMicrometer"){
}

void XYZControlGalil::Load(const wptree& pt) {
	XYZControl::Load(pt);
	comstring.SetFromPropertyTree(pt);
	xcountspermicron.SetFromPropertyTree(pt);
	ycountspermicron.SetFromPropertyTree(pt);
	zcountspermicron.SetFromPropertyTree(pt);
}

void XYZControlGalil::Save(wptree& pt) const {
	XYZControl::Save(pt);
	comstring.AddToPropertyTree(pt);
	xcountspermicron.AddToPropertyTree(pt);
	ycountspermicron.AddToPropertyTree(pt);
	zcountspermicron.AddToPropertyTree(pt);
}

XYZControlSutter::XYZControlSutter()
	: comstring(L"COM10", L"RS232COMPORT")
	, microstepspermicron(1.0, 1.0, 2000.0, L"MicrostepsPerMicrometer") {
}

void XYZControlSutter::Load(const wptree& pt) {
	XYZControl::Load(pt);
	comstring.SetFromPropertyTree(pt);
	microstepspermicron.SetFromPropertyTree(pt);
}

void XYZControlSutter::Save(wptree& pt) const {
	XYZControl::Save(pt);
	comstring.AddToPropertyTree(pt);
	microstepspermicron.AddToPropertyTree(pt);
}

FastZControl::FastZControl()
	: minoutput(0, -10, 10, L"MinOutput_V")
	, maxoutput(4, -10, 10, L"MaxOutput_V")
	, calibrationfile(L"None", L"CalibrationFile") {
	calibration.emplace(std::pair<double, double>(0.0, 0.0));
}

bool FastZControl::LoadCalibration(const std::wstring& _filepath) {
	std::ifstream file(_filepath);
	if ( !file.good() ) {
		DBOUT(L"FastZControl::LoadCalibration file is not good, load aborted");
		return false;
	}
	calibration.clear();
	double key = 0.0;
	double value = 0.0;
	while ( file.good() ) {
		file >> key >> value;
		calibration.emplace(std::pair<double, double>(key, value));
	}
	calibrationfile = _filepath;
	DBOUT(L"FastZControl::LoadCalibration loaded file " << _filepath);
	return true;
}

double FastZControl::PositionToVoltage(const double& _positionum) {
	double zvolt = 0;
	auto hit = calibration.find(_positionum);
	if ( hit != calibration.end() ) {
		zvolt = hit->second;
		return zvolt;
	}
	auto lower = calibration.upper_bound(_positionum);			// upper_bound returns element with key that is greater than _position
	if ( lower == calibration.begin() )	 {						// if we are to low we use the lowest value in calibration
		zvolt = lower->second;
		return zvolt;
	}
	if ( lower == calibration.end() )							// be careful when no calibration is loaded or we are at the end
		lower--;
	auto upper =  lower == (calibration.begin())?lower:lower--;
	
	// do linear interpolation
	double divider = (upper->first - lower->first);
	divider = (divider==0)?1:divider;							// avoid division by zero
	zvolt = ((upper->second - lower->second) / divider ) * (_positionum - lower->first) + lower->second;
	DBOUT(L"New ETL voltage: " << zvolt);
	return zvolt;
}

void FastZControl::Load(const wptree& pt) {
	minoutput.SetFromPropertyTree(pt);
	maxoutput.SetFromPropertyTree(pt);
	calibrationfile.SetFromPropertyTree(pt);
	if ( (calibrationfile() != L"") && (calibrationfile() != L"None") )
		LoadCalibration(calibrationfile());
}

void FastZControl::Save(wptree& pt) const {
	minoutput.AddToPropertyTree(pt);
	maxoutput.AddToPropertyTree(pt);
	calibrationfile.AddToPropertyTree(pt);
}


FastZControlETL::FastZControlETL() {
}

void FastZControlETL::Load(const wptree& pt) {
	FastZControl::Load(pt);
}

void FastZControlETL::Save(wptree& pt) const {
	FastZControl::Save(pt);
}

XYControl::XYControl()
	: xpos(0, -100000, 100000, L"XPosition_um")
	, ypos(0, -100000, 100000, L"YPosition_um")
	, pollinterval(1000, 0, 10000, L"PollInterval_ms")
	, buttonstepsize(10, 0, 1000, L"StepSizeForButtons_um")
	, stageinfo(L"", L"StageInformation") {
}

void XYControl::Load(const wptree& pt) {
	xpos.SetFromPropertyTree(pt);
	ypos.SetFromPropertyTree(pt);
	pollinterval.SetFromPropertyTree(pt);
	buttonstepsize.SetFromPropertyTree(pt);
}

void XYControl::Save(wptree& pt) const {
	xpos.AddToPropertyTree(pt);
	ypos.AddToPropertyTree(pt);
	pollinterval.AddToPropertyTree(pt);
	buttonstepsize.AddToPropertyTree(pt);
}

XYControlGalil::XYControlGalil()
	: comstring(L"COM5 19200", L"RS232Connection")
	, countspermicron(1024.0, 1.0, 10000.0, L"EncoderCountsPerMicrometer") {
}

void XYControlGalil::Load(const wptree& pt) {
	XYControl::Load(pt);
	comstring.SetFromPropertyTree(pt);
}

void XYControlGalil::Save(wptree& pt) const {
	XYControl::Save(pt);
	comstring.AddToPropertyTree(pt);
}

XYControlStanda::XYControlStanda()
	: devicex(0, 0, 10, L"DeviceNumberX")
	, devicey(1, 0, 10, L"DeviceNumberY")
	, speed(10, 0, 500, L"MovementSpeed_stepspersec")
	, microstepspermicron(3.226, 0.1, 100.0, L"MicrostepsPerMicrometer") {
}

void XYControlStanda::Load(const wptree& pt) {
	XYControl::Load(pt);
	devicex.SetFromPropertyTree(pt);
	devicey.SetFromPropertyTree(pt);
	speed.SetFromPropertyTree(pt);
	microstepspermicron.SetFromPropertyTree(pt);
}

void XYControlStanda::Save(wptree& pt) const {
	XYControl::Save(pt);
	devicex.AddToPropertyTree(pt);
	devicey.AddToPropertyTree(pt);
	speed.AddToPropertyTree(pt);
	microstepspermicron.AddToPropertyTree(pt);
}

}

}