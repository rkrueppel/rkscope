#include "stdafx.h"
#include "parameters/Scope.h"
#include "helpers/ScopeException.h"

namespace scope {

namespace parameters {

// Save some typing here...
using namespace boost::property_tree;

Stimulation::Stimulation()
	: channel(L"PXI-6259_0/port0/line0", L"Channel")
	, timingsource(L"OnboardClock", L"Timingsource")
	, onset(0, 0, 1000, L"Onset_s")
	, duration(0, 1, 1000, L"Duration_s")
	, ontime(0.5, 0.001, 1000, L"OnTime_s")
	, offtime(0.5, 0.001, 1000, L"OffTime_s")
	, enable(false, false, true, L"Enable") {
}

void Stimulation::Load(const wptree& pt) {
	channel.SetFromPropertyTree(pt);
	timingsource.SetFromPropertyTree(pt);
	duration.SetFromPropertyTree(pt);
	enable.SetFromPropertyTree(pt);
	offtime.SetFromPropertyTree(pt);
	onset.SetFromPropertyTree(pt);
	ontime.SetFromPropertyTree(pt);
}

void Stimulation::Save(wptree& pt) const {
	channel.AddToPropertyTree(pt);
	timingsource.AddToPropertyTree(pt);
	duration.AddToPropertyTree(pt);
	enable.AddToPropertyTree(pt);
	offtime.AddToPropertyTree(pt);
	onset.AddToPropertyTree(pt);
	ontime.AddToPropertyTree(pt);
}

void Stimulation::SetReadOnlyWhileScanning(const RunState& _runstate) {
	const bool enabler = (_runstate.t==RunStateHelper::Mode::Stopped)?true:false;
	duration.SetRWState(enabler);
	enable.SetRWState(enabler);
	offtime.SetRWState(enabler);
	onset.SetRWState(enabler);
	ontime.SetRWState(enabler);
}

Area::Area(const uint32_t& _area, const bool& _isslave, Area * const _masterarea)
	: area(_area, 0, 16, L"Area")
	, histrange(100, 0, 65535, L"HistRange")
	, isslave(_isslave, false, true, L"IsSlaveArea")
	, daq(_isslave)
	, masterarea(_masterarea)
	, linerate(1, 0, 100000, L"Linerate_Hz")
	, framerate(1, 0, 1000, L"Framerate_Hz")
	, frametime(1, 0, 100000, L"Frametime_s")
	, scanmode(ScannerVectorTypeHelper::Sawtooth, L"ScanMode")
	, basemicronperpixelx(0.1, 1E-6, 100, L"BaseMicronPerPixelX")
	, basemicronperpixely(0.1, 1E-6, 100, L"BaseMicronPerPixelY")
	, micronperpixelx(0.1, 1E-6, 100, L"MicronPerPixelX")
	, micronperpixely(0.1, 1E-6, 100, L"MicronPerPixelY") {

	// Make sure the parameter (isslave) equals the compiled logic (ThisIsSlaveArea, which uses the macro definitions of SCOPE_NAREAS etc)
	assert( isslave() == ThisIsSlaveArea(area) );
	
	// Make sure we set the pointer to the masterarea for slave areas
	assert( (isslave()==true && (masterarea!=nullptr)) || (isslave()==false) );
	
	// Insert all possible scannervectors
	scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Sawtooth, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Sawtooth)); 
	scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Bidirectional, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Bidirectional));
	scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Planehopper, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Planehopper));
	scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::ResonanceBiDi, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::ResonanceBiDi));
	scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::ResonanceHopper, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::ResonanceHopper));
	scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::LineStraight, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::LineStraight));

	InitializeConnections();
}

Area::Area(const Area& _a)
	: area(_a.area)
	, isslave(_a.isslave)
	, masterarea(_a.masterarea)
	, daq(_a.daq)
	, fpuxystage(_a.fpuxystage)
	, fpuzstage(_a.fpuzstage)
	, scannervectorframesmap()
	, scanmode(_a.scanmode(), L"ScanMode")
	, basemicronperpixelx(_a.basemicronperpixelx)
	, basemicronperpixely(_a.basemicronperpixely)
	, micronperpixelx(_a.micronperpixelx)
	, micronperpixely(_a.micronperpixely)
	, linerate(_a.linerate)
	, framerate(_a.framerate)
	, frametime(_a.frametime) {

	for ( auto& sv : _a.scannervectorframesmap )
		scannervectorframesmap.emplace(std::make_pair(sv.first, ScannerVectorFrameBasic::Factory(sv.first, sv.second.get())));

	InitializeConnections();
}

Area& Area::operator=(const Area& _a) {
	if ( this != &_a ) {
		area = _a.area;
		isslave = _a.isslave;
		masterarea = _a.masterarea;
		daq = _a.daq;
		fpuxystage = _a.fpuxystage;
		fpuzstage = _a.fpuzstage;
		scanmode = _a.scanmode();
		basemicronperpixelx = _a.basemicronperpixelx;
		basemicronperpixely = _a.basemicronperpixely;
		micronperpixelx = _a.micronperpixelx;
		micronperpixely = _a.micronperpixely;
		linerate = _a.linerate;
		framerate = _a.framerate;
		frametime = _a.frametime;

		for ( auto& sv : _a.scannervectorframesmap )
			scannervectorframesmap.emplace(std::make_pair(sv.first, ScannerVectorFrameBasic::Factory(sv.first, sv.second.get())));

	}

	// Not needed here?!
	//InitializeConnections();
		
	return *this;
}

void Area::SetMasterArea(Area* const _masterarea) {
	assert( (isslave()==true && (_masterarea!=nullptr)) || (isslave()==false) );
	masterarea = _masterarea;
	if ( isslave() ) 
		CopyFromMasterArea();
	ChangeScanMode();
	UpdateRates();
}

void Area::CopyFromMasterArea() {
	// Iterate over scannervectors
	for (auto& sv : scannervectorframesmap ) {
		*sv.second = *masterarea->scannervectorframesmap.at(sv.first);
	}

	// Copy master scanmode onto slave, so that the correct scanner vector is generated (Karlis)
	scanmode = masterarea->scanmode;
}

ScannerVectorFrameBasic& Area::Currentframe() const {
	return *scannervectorframesmap.at(scanmode.Value()).get();
}

ScannerVectorFrameSaw& Area::FrameSaw() const {
	try {
		return dynamic_cast<ScannerVectorFrameSaw&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::Sawtooth).get());
	} catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameSaw", true, true, true, true); }
}

ScannerVectorFrameResonance& Area::FrameResonance() const {
	try {
		return dynamic_cast<ScannerVectorFrameResonance&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::ResonanceBiDi));
	} catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameResonance", true, true, true, true); }
}

ScannerVectorFrameBiDi& Area::FrameBiDi() const {
	try {
		return dynamic_cast<ScannerVectorFrameBiDi&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::Bidirectional));
	} catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameBiDi", true, true, true, true); }
}

ScannerVectorFramePlaneHopper& Area::FrameHopper() const {
	try {
		return dynamic_cast<ScannerVectorFramePlaneHopper&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::Planehopper));
	} catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameHopper", true, true, true, true); }
}

void Area::Load(const wptree& pt) {
	area.SetFromPropertyTree(pt);
	histrange.SetFromPropertyTree(pt);
	isslave.SetFromPropertyTree(pt);
	linerate.SetFromPropertyTree(pt);
	framerate.SetFromPropertyTree(pt);
	frametime.SetFromPropertyTree(pt);
	scanmode.SetFromPropertyTree(pt);
	basemicronperpixelx.SetFromPropertyTree(pt);
	basemicronperpixely.SetFromPropertyTree(pt);
	micronperpixelx.SetFromPropertyTree(pt);
	micronperpixely.SetFromPropertyTree(pt);
	daq.Load(pt.get_child(L"daq"));

	for ( auto& sv : scannervectorframesmap ) {
		sv.second->Load(pt.get_child(ScannerVectorTypeHelper::NameOf(sv.first)));
	}

	fpuzstage.Load(pt.get_child(L"fpuzstage"));
	fpuxystage.Load(pt.get_child(L"fpuxystage"));
	ChangeScanMode();
	UpdateRates();
	UpdateFastZCalibration();
}

void Area::Save(wptree& pt) const {
	area.AddToPropertyTree(pt);
	histrange.AddToPropertyTree(pt);
	isslave.AddToPropertyTree(pt);
	linerate.AddToPropertyTree(pt);
	framerate.AddToPropertyTree(pt);
	frametime.AddToPropertyTree(pt);
	scanmode.AddToPropertyTree(pt);
	basemicronperpixelx.AddToPropertyTree(pt);
	basemicronperpixely.AddToPropertyTree(pt);
	micronperpixelx.AddToPropertyTree(pt);
	micronperpixely.AddToPropertyTree(pt);
	wptree ptdaq;
	wptree ptpresets;
	wptree ptfpuzstage;
	wptree ptfpuxystage;

	daq.Save(ptdaq);
	pt.add_child(L"daq", ptdaq);

	for ( const auto& sv : scannervectorframesmap ) {
		wptree ptsv;
		sv.second->Save(ptsv);
		pt.add_child(ScannerVectorTypeHelper::NameOf(sv.first), ptsv);
	}

	fpuzstage.Save(ptfpuzstage);
	pt.add_child(L"fpuzstage", ptfpuzstage);
	fpuxystage.Save(ptfpuxystage);
	pt.add_child(L"fpuxystage", ptfpuxystage);
}

void Area::SetReadOnlyWhileScanning(const RunState& _runstate) {
	const bool enabler = (_runstate.t==RunStateHelper::Mode::Stopped)?true:false;
	scanmode.SetRWState(enabler);
	fpuxystage.SetReadOnlyWhileScanning(_runstate);
	fpuzstage.SetReadOnlyWhileScanning(_runstate);
	daq.SetReadOnlyWhileScanning(_runstate);
	for ( auto& sv : scannervectorframesmap )
		sv.second->SetReadOnlyWhileScanning(_runstate);
}

void Area::SaveToPreset(const std::wstring& _name) {
	scannervectorframesmap.at(scanmode.Value())->SaveToPreset(_name, daq);
}
 
void Area::LoadFromPreset(const std::wstring& _name) {
	scannervectorframesmap.at(scanmode.Value())->LoadFromPreset(_name, daq);
}

void Area::DeletePreset(const std::wstring& _name) {
	scannervectorframesmap.at(scanmode.Value())->DeletePreset(_name);
}

double Area::FrameTime() const {
	return scannervectorframesmap.at(scanmode.Value())->TotalPixels() * daq.pixeltime() * 1E-6;
}

double Area::LineTime() const {
	return scannervectorframesmap.at(scanmode.Value())->XTotalPixels() * daq.pixeltime() * 1E-6;
}

uint32_t Area::TotalPixelsAllChannels() const {
	return scannervectorframesmap.at(scanmode.Value())->TotalPixels()*daq.inputs->channels();
}

double Area::XOffsetInMicron() const {
	// xoffset goes from -1..+1 (maxima of the zoom 1 field of view), thus:
	return scannervectorframesmap.at(scanmode.Value())->xoffset() * basemicronperpixelx() * 256.0 * 0.5;
}

double Area::YOffsetInMicron() const {
	// yoffset goes from -1..+1 (maxima of the zoom 1 field of view), thus:
	return scannervectorframesmap.at(scanmode.Value())->yoffset() * basemicronperpixely() * 256.0 * 0.5;
}

void Area::InitializeConnections() {
	scanmode.ConnectOther(std::bind(&Area::ChangeScanMode, this));
	if ( isslave() ) {
		// A slave area gets these from the master area. These connection go in the collector so they get disconnected when this
		// area is destructed
		// Iterate over all scannervectorsframe
		for ( auto& masv : masterarea->scannervectorframesmap ) {
			auto conns = masv.second->ConnectCopyTrigger(std::bind(&Area::CopyFromMasterArea, this));
			connection_collector.insert(connection_collector.end(), conns.begin(), conns.end());
		}
		CopyFromMasterArea();
	}
	else {
		// Iterate over all scannervectorsframe and connect changes of their parameters to calculating functions in Area
		for ( auto& sv : scannervectorframesmap ) {
			sv.second->ConnectRateUpdate(std::bind(&Area::UpdateRates, this));
			sv.second->ConnectResolutionUpdate(std::bind(&Area::CalculateResolution, this));
			sv.second->ConnectMicronPerPixelUpdate(std::bind(&Area::CalculateMicronPerPixel, this));
		}
	}
	daq.pixeltime.ConnectOther(std::bind(&Area::UpdateRates, this));

	ChangeScanMode();
	UpdateRates();
	UpdateFastZCalibration();
}

void Area::ChangeScanMode() {
	UpdateRates();
	CalculateMicronPerPixel();
	if ( isslave() )
		CopyFromMasterArea();
}

void Area::UpdateRates() {
	framerate = 1/FrameTime();
	frametime = FrameTime();
	linerate = 1/LineTime();
}

void Area::UpdateFastZCalibration() {
	DBOUT(L"Area::UpdateFastZCalibration");
	if (!fpuzstage.calibration.empty()) {
		for ( auto& sv : scannervectorframesmap )
			sv.second->fastz.SetLimits(fpuzstage.calibration.begin()->first, (--fpuzstage.calibration.end())->first);
	}
}

void Area::CalculateMicronPerPixel() {
	// basemicronperpixel must be calibrated to zoom 1 256x256 pixel
	micronperpixelx = basemicronperpixelx() / Currentframe().zoom() * (256.0 / static_cast<double>(Currentframe().xres()));
	micronperpixely = basemicronperpixely() / Currentframe().zoom() * (256.0 / static_cast<double>(Currentframe().yres()));
}

void Area::CalculateResolution() {
	scannervectorframesmap.at(scanmode.Value())->xres = static_cast<double>(Currentframe().yres) / Currentframe().yaspectratio * Currentframe().xaspectratio;
	double xaspectratioboundary = Currentframe().yaspectratio / static_cast<double>(Currentframe().yres) * static_cast<double>(Currentframe().xres);
	if(Currentframe().xaspectratio < xaspectratioboundary || Currentframe().xaspectratio > xaspectratioboundary) {
		scannervectorframesmap.at(scanmode.Value())->xaspectratio = xaspectratioboundary;
	}

	scannervectorframesmap.at(scanmode.Value())->yres = static_cast<double>(Currentframe().xres) / Currentframe().xaspectratio * Currentframe().yaspectratio;
	double yaspectratioboundary = Currentframe().xaspectratio / static_cast<double>(Currentframe().xres) * static_cast<double>(Currentframe().yres);
	if(Currentframe().yaspectratio < yaspectratioboundary || Currentframe().yaspectratio > yaspectratioboundary) {
		scannervectorframesmap.at(scanmode.Value())->yaspectratio = yaspectratioboundary;
	}
}

Scope::Scope()
	: date(L"", L"Date")
	, time(L"", L"Time")
	, scopecommit(std::wstring(CA2W(STR(LASTGITCOMMIT))), L"ScopeCommit")
	, comment(L"", L"Comment")
	, scannertype(SCOPE_SCANNERTYPE, L"Scannertype")
	, startinputsfirst(false, false, true, L"StartInputsFirst")
	, commontrigger(L"/PXI-6259_0/ao/StartTrigger", L"CommonMasterTrigger")
	, masterfovsizex(3000.0, 1.0, 20000.0, L"MasterFOVSizeX_um")
	, masterfovsizey(3000.0, 1.0, 20000.0, L"MasterFOVSizeY_um")
	, run_state(RunStateHelper::Stopped, L"RunState")
	, requested_mode(DaqModeHelper::continuous, L"RequestedMode") {
	date.Set(GetCurrentDateString());
	time.Set(GetCurrentTimeString());
	
	scannertype.SetRWState(false);
	
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		// All area masters or first area is master the rest slaves
		if ( ThisIsSlaveArea(a) )
			areas[a].reset(new Area(a, true, areas[0].get()));
		else
			areas[a].reset(new Area(a, false, nullptr));

		areas[a]->framerate.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
		areas[a]->daq.averages.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
		areas[a]->histrange.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
		timeseries.frames[a].ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
		timeseries.totaltimes[a].ConnectOther(std::bind(&Scope::UpdateFramesFromTotaltime, this));
	}

	timeseries.repeats.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
	timeseries.betweenrepeats.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
	UpdateTotaltimeFromFrames();
}

Scope::Scope(const Scope& _scope)
	: date(_scope.date)
	, time(_scope.time)
	, scopecommit(_scope.scopecommit)
	, comment(_scope.comment)
	, scannertype(_scope.scannertype)
	, storage(_scope.storage)
	, stack(_scope.stack)
	, timeseries(_scope.timeseries)
	, behavior(_scope.behavior)
	, stage(_scope.stage)
	, stimulation(_scope.stimulation)
	, frames(_scope.frames)
	, startinputsfirst(_scope.startinputsfirst)
	, commontrigger(_scope.commontrigger)
	, masterfovsizex(_scope.masterfovsizex)
	, masterfovsizey(_scope.masterfovsizey)
	, run_state(_scope.run_state)
	, requested_mode(_scope.requested_mode){

	scannertype.SetRWState(false);

	// Deep copy of Areas
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		// First areas are masters the rest slaves (if SCOPE_NBEAM_SLAVES > 0)
		if ( ThisIsSlaveArea(a) ) {
			areas[a].reset(new Area(*_scope.areas[a].get()));
			// Fix the pointer to the master area!!!!!
			areas[a]->SetMasterArea(areas[0].get());
		}
		else {
			areas[a].reset(new Area(*_scope.areas[a].get()));
			areas[a]->SetMasterArea(nullptr);
		}
	}
}

Scope& Scope::operator=(const Scope& _scope) {
	date = _scope.date();
	time = _scope.time();
	scopecommit = _scope.scopecommit();
	comment = _scope.comment();

	ATLASSERT (scannertype() == _scope.scannertype());	// Otherwise something is seriously wrong

	// Deep copy of Areas
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		// First areas are masters the rest slaves (if SCOPE_NBEAM_SLAVES > 0)
		if ( ThisIsSlaveArea(a) ) {
			areas[a].reset(new Area(*_scope.areas[a].get()));
			// Fix the pointer to the master area!!!!!
			areas[a]->SetMasterArea(areas[0].get());
		}
		else {
			areas[a].reset(new Area(*_scope.areas[a].get()));
			areas[a]->SetMasterArea(nullptr);
		}
	}

	storage = _scope.storage;
	stack = _scope.stack;
	timeseries = _scope.timeseries;
	behavior = _scope.behavior;
	stage = _scope.stage;
	stimulation = _scope.stimulation;
	frames = _scope.frames;
	startinputsfirst = _scope.startinputsfirst;
	commontrigger = _scope.commontrigger;
	masterfovsizex = _scope.masterfovsizex;
	masterfovsizey = _scope.masterfovsizey;
	run_state = _scope.run_state;
	requested_mode = _scope.requested_mode;

	return *this;
}

void Scope::UpdateTotaltimeFromFrames() {
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
		timeseries.totaltimes[a].Set(1/areas[a]->framerate()*areas[ThisAreaOrMasterArea(a)]->daq.averages()*timeseries.frames[a](), true, false);

	// Time between repeats (start to start) can be minimally duration of one timeseries (+0.1s for overhead)
	double maxduration = *std::max_element(std::begin(timeseries.totaltimes), std::end(timeseries.totaltimes));
	timeseries.betweenrepeats.SetWithLimits(timeseries.betweenrepeats(), maxduration + 0.1, timeseries.betweenrepeats.ul());

	timeseries.overalltime = timeseries.totaltimes[0]() * timeseries.repeats() + (timeseries.repeats()-1) * timeseries.betweenrepeats();
}

void Scope::UpdateFramesFromTotaltime() {
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
		timeseries.frames[a].Set(round2ui32(timeseries.totaltimes[a]()*areas[a]->framerate()/areas[ThisAreaOrMasterArea(a)]->daq.averages()));
}

void Scope::Load(const std::wstring& filename) {
	wptree pt;

	try {
		// use the current locale, convert from utf-8 (in file) to wchar_t (here), and skip BOM header
		std::locale old_locale;
		std::locale utf8_locale(old_locale, new std::codecvt_utf8<wchar_t, 0x10ffff, std::codecvt_mode::consume_header>); // no leak, std::locale takes responsibility for deleting...
		// sigh, UTF is never easy...

		CW2A tmp(filename.c_str());
		read_xml(std::string(tmp), pt, 0, utf8_locale);
		date.SetFromPropertyTree(pt.get_child(L"scope"));
		time.SetFromPropertyTree(pt.get_child(L"scope"));
		// Do not load scopecommit
		comment.SetFromPropertyTree(pt.get_child(L"scope"));

		// Make sure the loaded parameters are for the same scannertype
		if( pt.get_child(L"scope").get<std::wstring>(L"Scannertype") != scannertype.ToChar() )
			throw ScopeException("Type of scanner in parameter file does not match the one in scope.exe");

		startinputsfirst.SetFromPropertyTree(pt.get_child(L"scope"));
		commontrigger.SetFromPropertyTree(pt.get_child(L"scope"));
		masterfovsizex.SetFromPropertyTree(pt.get_child(L"scope"));
		masterfovsizey.SetFromPropertyTree(pt.get_child(L"scope"));
		// We do not need to Load requested_mode and runstate
		storage.Load(pt.get_child(L"scope.storage"));
		stack.Load(pt.get_child(L"scope.stack"));
		timeseries.Load(pt.get_child(L"scope.timeseries"));
		behavior.Load(pt.get_child(L"scope.behavior"));
		stage.Load(pt.get_child(L"scope.stage"));
		stimulation.Load(pt.get_child(L"scope.stimulation"));
		frames.Load(pt.get_child(L"scope.frames"));
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
			areas[a]->Load(pt.get_child(boost::str(boost::wformat(L"scope.area%d") % a)));
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
}

void Scope::Save(const std::wstring& filename) const {
	wptree pt;
	wptree ptroot;
	std::array<wptree, SCOPE_NAREAS> ptareas;
	wptree ptstorage;
	wptree ptstack;
	wptree pttimeseries;
	wptree ptbehavior;
	wptree ptzcontrol;
	wptree ptstage;
	wptree ptstimulation;
	wptree ptframes;

	try {
		date.AddToPropertyTree(ptroot);
		time.AddToPropertyTree(ptroot);
		scopecommit.AddToPropertyTree(ptroot);
		comment.AddToPropertyTree(ptroot);
		scannertype.AddToPropertyTree(ptroot);
		startinputsfirst.AddToPropertyTree(ptroot);
		commontrigger.AddToPropertyTree(ptroot);
		masterfovsizex.AddToPropertyTree(ptroot);
		masterfovsizey.AddToPropertyTree(ptroot);
		pt.add_child(L"scope", ptroot);
		for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
			areas[a]->Save(ptareas[a]);
			pt.add_child(boost::str(boost::wformat(L"scope.area%d") % a), ptareas[a]);
		}
		storage.Save(ptstorage);
		pt.add_child(L"scope.storage", ptstorage);
		stack.Save(ptstack);
		pt.add_child(L"scope.stack", ptstack);
		timeseries.Save(pttimeseries);
		pt.add_child(L"scope.timeseries", pttimeseries);
		behavior.Save(ptbehavior);
		pt.add_child(L"scope.behavior", ptbehavior);
		stage.Save(ptstage);
		pt.add_child(L"scope.stage", ptstage);
		stimulation.Save(ptstimulation);
		pt.add_child(L"scope.stimulation", ptstimulation);
		frames.Save(ptframes);
		pt.add_child(L"scope.frames", ptframes);
		// use the current locale, convert from wchar_t (here) to UTF-8 (in file), and and generate a BOM header
		std::locale old_locale;
		// Attention: the 'new' here is correct that way (weird as it is...)
		std::locale utf8_locale(old_locale
			, new std::codecvt_utf8<wchar_t, 0x10ffff, (std::codecvt_mode)(std::codecvt_mode::consume_header | std::codecvt_mode::generate_header)>); // no leak, std::locale takes responsibility for deleting...
		CW2A tmp(filename.c_str());
		write_xml(std::string(tmp), pt, utf8_locale);
	}
	catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
}

void Scope::SetReadOnlyWhileScanning(const RunState& _runstate) {
	for ( auto& a : areas )
		a->SetReadOnlyWhileScanning(_runstate);
	storage.SetReadOnlyWhileScanning(_runstate);
	stack.SetReadOnlyWhileScanning(_runstate);
	timeseries.SetReadOnlyWhileScanning(_runstate);
	stage.SetReadOnlyWhileScanning(_runstate);
	stimulation.SetReadOnlyWhileScanning(_runstate);
}


}

}