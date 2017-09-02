#include "stdafx.h"
#include "parameters/Scope.h"
#include "helpers/ScopeException.h"

namespace scope {

namespace parameters {

// Save some typing here...
using namespace boost::property_tree;


Scope::Scope(const uint32_t& _nareas)
	: nareas(_nareas, 1, 20, L"No. of Areas")
	, date(L"", L"Date")
	, time(L"", L"Time")
	, scopecommit(std::wstring(CA2W(STR(LASTGITCOMMIT))), L"ScopeCommit")
	, comment(L"", L"Comment")
	, scannertype(SCOPE_SCANNERTYPE, L"Scannertype")
	, areas(0)
	, startinputsfirst(false, false, true, L"StartInputsFirst")
	, commontrigger(L"/PXI-6259_0/ao/StartTrigger", L"CommonMasterTrigger")
	, masterfovsizex(3000.0, 1.0, 20000.0, L"MasterFOVSizeX_um")
	, masterfovsizey(3000.0, 1.0, 20000.0, L"MasterFOVSizeY_um")
	, run_state(RunStateHelper::Stopped, L"RunState")
	, requested_mode(DaqModeHelper::continuous, L"RequestedMode") {
	date.Set(GetCurrentDateString());
	time.Set(GetCurrentTimeString());
	
	scannertype.SetRWState(false);
	/*uint32_t a = 0;
	std::generate_n(std::back_inserter(areas), nareas, [&a, this]() {
		parameters::Area A(a, false, ThisIsSlaveArea(a)?nullptr:&areas[0]);
		a++;
		return A;
	});*/

	for ( uint32_t a = 0 ; a < nareas() ; a++ ) {
		// All area masters or first area is master the rest slaves
		if (ThisIsSlaveArea(a))
			areas.emplace_back(parameters::Area(a, true, &areas[0]));
		else
			areas.emplace_back(parameters::Area(a, false, nullptr));

		areas[a].framerate.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
		areas[a].daq.averages.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
		areas[a].histrange.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
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