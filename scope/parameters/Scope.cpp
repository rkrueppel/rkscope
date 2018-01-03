#include "stdafx.h"
#include "parameters/Scope.h"
#include "helpers/ScopeException.h"

namespace scope {

namespace parameters {

		// Save some typing here...
		using namespace boost::property_tree;


		Scope::Scope(const uint32_t& _nmasters, const uint32_t& _nslaves, std::vector<uint32_t> _masterofslaves)
			: nmasters(_nmasters, L"NoMasters")
			, nslaves(_nslaves, L"NoSlaves")
			, masterofslaves(_masterofslaves)
			, date(L"", L"Date")
			, time(L"", L"Time")
			, scopecommit(std::wstring(CA2W(STR(LASTGITCOMMIT))), L"ScopeCommit")
			, comment(L"", L"Comment")
			, masterareas(0)
			, slaveareas(0)
			, allareas(0)
			, stack(nmasters, nslaves)
			, timeseries(nmasters, nslaves)
			, behavior(nmasters, nslaves)
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
			std::generate_n(std::back_inserter(masterareas), nmasters, [&a, this]() {
				parameters::MasterArea A(a);
				a++;
				return A;
			});

			a = 0;
			std::generate_n(std::back_inserter(slaveareas), nslaves, [&a, this]() {
				parameters::SlaveArea A(a, &masterareas[masterofslaves[a]]);
				a++;
				return A;
			});
			a = 0;
			std::generate_n(std::back_inserter(allareas), nmasters, [&a, this]() {
				return &masterareas[a];
			});
			a = 0;
			std::generate_n(std::back_inserter(allareas), nslaves, [&a, this]() {
				return &slaveareas[a];
			});
	
			for(auto& ar : masterareas) {
				ar.framerate.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
				ar.daq.averages.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
				ar.histrange.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
			}

			for (auto& ar : slaveareas) {
				ar.framerate.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
				ar.daq.averages.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
				ar.histrange.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
			}
	
			for (auto& at : timeseries.frames )
				for (auto& fr : at)
					fr.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
	
			for (auto& at : timeseries.totaltimes )
				for (auto& tt : at )
					tt.ConnectOther(std::bind(&Scope::UpdateFramesFromTotaltime, this));

			timeseries.repeats.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
			timeseries.betweenrepeats.ConnectOther(std::bind(&Scope::UpdateTotaltimeFromFrames, this));
			UpdateTotaltimeFromFrames();
		}

		Scope::Scope(const Scope& _scope)
			: nmasters(_scope.nmasters)
			, nslaves(_scope.nslaves)
			, masterofslaves(_scope.masterofslaves)
			, date(_scope.date)
			, time(_scope.time)
			, scopecommit(_scope.scopecommit)
			, comment(_scope.comment)
			, masterareas(_scope.masterareas)
			, slaveareas(_scope.slaveareas)
			, allareas(0)
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

			uint32_t a = 0;
			std::generate_n(std::back_inserter(allareas), nmasters, [&a, this]() {
				return &masterareas[a];
			});
			a = 0;
			std::generate_n(std::back_inserter(allareas), nslaves, [&a, this]() {
				return &slaveareas[a];
			});

			// Fix the pointer to the master area!!!!!
			for(uint32_t a = 0; a < nslaves ; a++ )
				slaveareas[a].SetMasterArea(&masterareas[masterofslaves[a]]);
		}

		Scope& Scope::operator=(const Scope& _scope) {
			nmasters = _scope.nmasters();
			nslaves = _scope.nslaves();
			masterofslaves = _scope.masterofslaves;
			date = _scope.date();
			time = _scope.time();
			scopecommit = _scope.scopecommit();
			comment = _scope.comment();

			masterareas = _scope.masterareas;
			slaveareas = _scope.slaveareas;
			allareas.clear();
			uint32_t a = 0;
			std::generate_n(std::back_inserter(allareas), nmasters, [&a, this]() {
				return &masterareas[a];
			});
			a = 0;
			std::generate_n(std::back_inserter(allareas), nslaves, [&a, this]() {
				return &slaveareas[a];
			});

			// Fix the pointer to the master area!!!!!
			for (uint32_t a = 0; a < nslaves; a++)
				slaveareas[a].SetMasterArea(&masterareas[masterofslaves[a]]);

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
			for ( uint32_t a = 0 ; a < nmasters ; a++ )
				timeseries.totaltimes[ATMaster][a].Set(1/masterareas[a].framerate()*masterareas[a].daq.averages()*timeseries.frames[ATMaster][a](), true, false);

			for (uint32_t a = 0; a < nslaves; a++)
				timeseries.totaltimes[ATSlave][a].Set(1 / slaveareas[a].framerate()*slaveareas[a].daq.averages()*timeseries.frames[ATSlave][a](), true, false);

			// Time between repeats (start to start) can be minimally duration of one timeseries (+0.1s for overhead)
			double maxduration = *std::max_element(std::begin(timeseries.totaltimes[ATMaster]), std::end(timeseries.totaltimes[ATMaster]));
			timeseries.betweenrepeats.SetWithLimits(timeseries.betweenrepeats(), maxduration + 0.1, timeseries.betweenrepeats.ul());

			timeseries.overalltime = timeseries.totaltimes[ATMaster][0]() * timeseries.repeats() + (timeseries.repeats()-1) * timeseries.betweenrepeats();
		}

		void Scope::UpdateFramesFromTotaltime() {
			for ( uint32_t a = 0 ; a < nmasters ; a++ )
				timeseries.frames[ATMaster][a].Set(round2ui32(timeseries.totaltimes[ATMaster][a]()*masterareas[a].framerate()/masterareas[a].daq.averages()));
			for (uint32_t a = 0; a < nslaves; a++)
				timeseries.frames[ATSlave][a].Set(round2ui32(timeseries.totaltimes[ATSlave][a]()*slaveareas[a].framerate() / slaveareas[a].daq.averages()));
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
				nmasters.SetFromPropertyTree(pt.get_child(L"scope"));
				nslaves.SetFromPropertyTree(pt.get_child(L"scope"));
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
				for (auto& ma : masterareas)
					ma.Load(pt.get_child(boost::str(boost::wformat(L"scope.masterarea%d") % i++)));

				i = 0;
				for (auto& sa : slaveareas)
					sa.Load(pt.get_child(boost::str(boost::wformat(L"scope.slavearea%d") % i++)));
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
		}

		void Scope::Save(const std::wstring& filename) const {
			wptree pt;
			wptree ptroot;
			std::vector<wptree> ptmasterareas(masterareas.size());
			std::vector<wptree> ptslaveareas(slaveareas.size());
			wptree ptstorage;
			wptree ptstack;
			wptree pttimeseries;
			wptree ptbehavior;
			wptree ptzcontrol;
			wptree ptstage;
			wptree ptstimulation;
			wptree ptframes;

			try {
				nmasters.AddToPropertyTree(ptroot);
				nslaves.AddToPropertyTree(ptroot);
				date.AddToPropertyTree(ptroot);
				time.AddToPropertyTree(ptroot);
				scopecommit.AddToPropertyTree(ptroot);
				comment.AddToPropertyTree(ptroot);
				startinputsfirst.AddToPropertyTree(ptroot);
				commontrigger.AddToPropertyTree(ptroot);
				masterfovsizex.AddToPropertyTree(ptroot);
				masterfovsizey.AddToPropertyTree(ptroot);
				pt.add_child(L"scope", ptroot);
				for ( uint32_t a = 0 ; a < masterareas.size() ; a++ ) {
					masterareas[a].Save(ptmasterareas[a]);
					pt.add_child(boost::str(boost::wformat(L"scope.masterarea%d") % a), ptmasterareas[a]);
				}
				for (uint32_t a = 0; a < slaveareas.size(); a++) {
					slaveareas[a].Save(ptslaveareas[a]);
					pt.add_child(boost::str(boost::wformat(L"scope.slavearea%d") % a), ptslaveareas[a]);
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
			for ( auto& ar : masterareas )
				ar.SetReadOnlyWhileScanning(_runstate);
			for (auto& ar : slaveareas)
				ar.SetReadOnlyWhileScanning(_runstate);
			storage.SetReadOnlyWhileScanning(_runstate);
			stack.SetReadOnlyWhileScanning(_runstate);
			timeseries.SetReadOnlyWhileScanning(_runstate);
			stage.SetReadOnlyWhileScanning(_runstate);
			stimulation.SetReadOnlyWhileScanning(_runstate);
		}


	}

}