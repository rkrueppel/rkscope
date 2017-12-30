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
			, areas(0)
			, stack(_nareas)
			, timeseries(_nareas)
			, behavior(_nareas)
			, startinputsfirst(false, false, true, L"StartInputsFirst")
			, commontrigger(L"/PXI-6259_0/ao/StartTrigger", L"CommonMasterTrigger")
			, masterfovsizex(3000.0, 1.0, 20000.0, L"MasterFOVSizeX_um")
			, masterfovsizey(3000.0, 1.0, 20000.0, L"MasterFOVSizeY_um")
			, run_state(RunStateHelper::Stopped, L"RunState")
			, requested_mode(DaqModeHelper::continuous, L"RequestedMode")
		{
			date.Set(GetCurrentDateString());
			time.Set(GetCurrentTimeString());
	
			uint32_t a = 0;
			std::generate_n(std::back_inserter(areas), nareas, [&a, this]() {
				parameters::Area A(a, false, config::slavearea[a]?&areas[0]:nullptr);
				a++;
				return A;
			});
	
			for(auto& ar : areas) {
				ar.framerate.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
				ar.daq.averages.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
				ar.histrange.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
			}
	
			for (auto& fr : timeseries.frames )
				fr.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
	
			for (auto& tt : timeseries.totaltimes )
				tt.ConnectOther(std::bind(&Scope::UpdateFramesFromTotaltime, this));

			timeseries.repeats.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
			timeseries.betweenrepeats.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
			UpdateTotaltimeFromFrames();
		}

		Scope::Scope(const Scope& _scope)
			: nareas(_scope.nareas)
			, date(_scope.date)
			, time(_scope.time)
			, scopecommit(_scope.scopecommit)
			, comment(_scope.comment)
			, areas(_scope.areas)
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

			// Fix the pointer to the master area!!!!!
			uint32_t i = 0;
			for (auto& ar : areas)
				ar.SetMasterArea(config::slavearea[i++]?&areas[0]:nullptr);
		}

		Scope& Scope::operator=(const Scope& _scope) {
			nareas = _scope.nareas();
			date = _scope.date();
			time = _scope.time();
			scopecommit = _scope.scopecommit();
			comment = _scope.comment();

			areas = _scope.areas;
			// Fix the pointer to the master area!!!!!
			uint32_t i = 0;
			for (auto& ar : areas)
				ar.SetMasterArea(config::slavearea[i++]?&areas[0]:nullptr);

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
			for ( uint32_t a = 0 ; a < nareas ; a++ )
				timeseries.totaltimes[a].Set(1/areas[a].framerate()*areas[config::MyMaster(a)].daq.averages()*timeseries.frames[a](), true, false);

			// Time between repeats (start to start) can be minimally duration of one timeseries (+0.1s for overhead)
			double maxduration = *std::max_element(std::begin(timeseries.totaltimes), std::end(timeseries.totaltimes));
			timeseries.betweenrepeats.SetWithLimits(timeseries.betweenrepeats(), maxduration + 0.1, timeseries.betweenrepeats.ul());

			timeseries.overalltime = timeseries.totaltimes[0]() * timeseries.repeats() + (timeseries.repeats()-1) * timeseries.betweenrepeats();
		}

		void Scope::UpdateFramesFromTotaltime() {
			for ( uint32_t a = 0 ; a < nareas ; a++ )
				timeseries.frames[a].Set(round2ui32(timeseries.totaltimes[a]()*areas[a].framerate()/areas[config::MyMaster(a)].daq.averages()));
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
				nareas.SetFromPropertyTree(pt.get_child(L"scope"));
				date.SetFromPropertyTree(pt.get_child(L"scope"));
				time.SetFromPropertyTree(pt.get_child(L"scope"));
				// Do not load scopecommit
				comment.SetFromPropertyTree(pt.get_child(L"scope"));

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
				uint32_t i = 0;
				for (auto& ar : areas)
					ar.Load(pt.get_child(boost::str(boost::wformat(L"scope.area%d") % i++)));
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
		}

		void Scope::Save(const std::wstring& filename) const {
			wptree pt;
			wptree ptroot;
			std::vector<wptree> ptareas(areas.size());
			wptree ptstorage;
			wptree ptstack;
			wptree pttimeseries;
			wptree ptbehavior;
			wptree ptzcontrol;
			wptree ptstage;
			wptree ptstimulation;
			wptree ptframes;

			try {
				nareas.AddToPropertyTree(ptroot);
				date.AddToPropertyTree(ptroot);
				time.AddToPropertyTree(ptroot);
				scopecommit.AddToPropertyTree(ptroot);
				comment.AddToPropertyTree(ptroot);
				startinputsfirst.AddToPropertyTree(ptroot);
				commontrigger.AddToPropertyTree(ptroot);
				masterfovsizex.AddToPropertyTree(ptroot);
				masterfovsizey.AddToPropertyTree(ptroot);
				pt.add_child(L"scope", ptroot);
				for ( uint32_t a = 0 ; a < areas.size() ; a++ ) {
					areas[a].Save(ptareas[a]);
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
			for ( auto& ar : areas )
				ar.SetReadOnlyWhileScanning(_runstate);
			storage.SetReadOnlyWhileScanning(_runstate);
			stack.SetReadOnlyWhileScanning(_runstate);
			timeseries.SetReadOnlyWhileScanning(_runstate);
			stage.SetReadOnlyWhileScanning(_runstate);
			stimulation.SetReadOnlyWhileScanning(_runstate);
		}


	}

}