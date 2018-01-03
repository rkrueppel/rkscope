#include "stdafx.h"
#include "parameters/Runstates.h"
#include "helpers/ScopeException.h"

namespace scope {

	namespace parameters {

		// Save some typing here...
		using namespace boost::property_tree;

		Stack::Stack(const uint32_t& _nmasters, const uint32_t& _nslaves)
			: nmasters(_nmasters)
			, nslaves(_nslaves)
			, startat{std::vector<PlaneProperties>(nmasters),std::vector<PlaneProperties>(nslaves) }
			, stopat{std::vector<PlaneProperties>(nmasters), std::vector<PlaneProperties>(nslaves) }
			, spacing(1, 0.1, 50, L"Spacing_um")
			, zdevicetype(ZDeviceHelper::ZStage, L"ZDeviceType")
			, overalltime(1, 1, 2000, L"OverallTime_s")
		{
			spacing.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
			zdevicetype.ConnectOther(std::bind(&Stack::ResetPlanes, this));
			for (auto& at : startat) {
				for (auto& p : at) {
					p.position.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
					p.pockels.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
				}
			}
			for ( auto& at : stopat ) {
				for (auto& p : at) {
					p.position.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
					p.pockels.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
				}
			}
		}

		void Stack::UpdatePlanes() {
			// Find max number of planes in all areas
			uint32_t maxplanes = 0;
			
			for ( uint32_t a = 0 ; a < nmasters ; a++ ) {
				// +1 since stop plane also counts
				uint32_t areaplanes = static_cast<uint32_t>(floor(Range(ATMaster, a) / spacing()) + 1);
				maxplanes = (areaplanes > maxplanes)?areaplanes:maxplanes;
			}
			for (uint32_t a = 0; a < nslaves; a++) {
				// +1 since stop plane also counts
				uint32_t areaplanes = static_cast<uint32_t>(floor(Range(ATSlave, a) / spacing()) + 1);
				maxplanes = (areaplanes > maxplanes) ? areaplanes : maxplanes;
			}

			/*planes___.clear();
			planes___.resize(maxplanes, std::vector<PlaneProperties>(nareas));*/
			planes.assign(maxplanes, { std::vector<PlaneProperties>(nmasters), std::vector<PlaneProperties>(nslaves) } );

			for (uint32_t at = 0; at < NAreaTypes; at++) {
				for (uint32_t a = 0; a < (at==ATMaster)?nmasters:nslaves; a++) {
					double l = Lambda(AreaType(at), a);
					double c = stopat[at][a].pockels()*exp(-l * stopat[at][a].position());
					// +1 since stop plane also counts
					uint32_t areaplanes = static_cast<uint32_t>(floor(Range(AreaType(at),a) / spacing()) + 1);
					DBOUT(L"Stack::UpdatePlanes area " << a << L" lambda: " << Lambda(AreaType(at),a) << L" c: " << c);

					uint32_t p = 0;
					// Planes in the defined range for that area are defined with increasing position and adjusted Pockels power
					for (; p < areaplanes; p++) {
						planes[p][at][a].position = startat[at][a].position() + p * Increment(AreaType(at),a);
						planes[p][at][a].pockels = c * exp(l * planes[p][at][a].position());
					}
					// If another area has more planes, set this area's planes with the last position and closed Pockels cell
					for (; p < maxplanes; p++) {
						planes[p][at][a].position = planes[areaplanes - 1][at][a].position();
						planes[p][at][a].pockels = 0;
					}
				}
			}
		}

		void Stack::ResetPlanes() {
			planes.clear();
			for (uint32_t at = 0; at < NAreaTypes; at++) {
				for (uint32_t a = 0; a < (at==ATMaster)?nmasters:nslaves; a++) {
					// Set to zero and do not call other signal (i.e. UpdatePlanes) right now
					stopat[at][a].position.Set(0, true, false);
					stopat[at][a].pockels.Set(0, true, false);
					startat[at][a].position.Set(0, true, false);
					startat[at][a].pockels.Set(0, true, false);
				}
			}
			UpdatePlanes();
		}

		double Stack::Range(const AreaType& _areatype, const uint32_t& _area) {
			return abs(stopat[_areatype][_area].position() - startat[_areatype][_area].position());
		}

		double Stack::Increment(const AreaType& _areatype, const uint32_t& _area) {
			return (stopat[_areatype][_area].position()>startat[_areatype][_area].position())?spacing():-1.0*spacing();
		}

		double Stack::Lambda(const AreaType& _areatype, const uint32_t& _area) {
			if ( (stopat[_areatype][_area].pockels() == 0) || (startat[_areatype][_area].pockels() == 0) )
				return 0.0;
			if ( startat[_areatype][_area].position() == stopat[_areatype][_area].position() )
				return 0.0;
			return log(startat[_areatype][_area].pockels()/stopat[_areatype][_area].pockels()) / (startat[_areatype][_area].position()-stopat[_areatype][_area].position());
		}

		void Stack::Load(const wptree& pt) {
			// Catch possible exceptions from Load, continue unharmed after
			try {
				for (uint32_t at = 0; at < NAreaTypes; at++) {
					for (uint32_t a = 0; a < startat[at].size(); a++) {
						startat[at][a].Load(pt.get_child(boost::str(boost::wformat(L"startat%s%d") % AreaTypeString[at] % a)));
						stopat[at][a].Load(pt.get_child(boost::str(boost::wformat(L"stopat%s%d") % AreaTypeString[at] % a)));
					}
				}
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
			spacing.SetFromPropertyTree(pt);
			zdevicetype.SetFromPropertyTree(pt);
			overalltime.SetFromPropertyTree(pt);
		}

		void Stack::Save(wptree& pt) const {
			std::array<std::vector<wptree>, NAreaTypes> ptstartat({ std::vector<wptree>(nmasters), std::vector<wptree>(nslaves) });
			std::array<std::vector<wptree>, NAreaTypes> ptstopat({ std::vector<wptree>(nmasters), std::vector<wptree>(nslaves) });
			for (uint32_t at = 0; at < NAreaTypes; at++) {
				for (uint32_t a = 0; a < startat[at].size(); a++) {
					startat[at][a].Save(ptstartat[at][a]);
					pt.add_child(boost::str(boost::wformat(L"startat%s%d") % AreaTypeString[at] % a), ptstartat[at][a]);
					stopat[at][a].Save(ptstopat[at][a]);
					pt.add_child(boost::str(boost::wformat(L"stopat%s%d") % AreaTypeString[at] % a), ptstopat[at][a]);
				}
			}
			spacing.AddToPropertyTree(pt);
			zdevicetype.AddToPropertyTree(pt);
			overalltime.AddToPropertyTree(pt);
		}

		void Stack::SetReadOnlyWhileScanning(const RunState& _runstate) {
			bool enabler = false;
			// Stack properties are changeable when not scanning or during continuous/live scanning
			if ( (_runstate.t == RunStateHelper::Mode::Stopped) || (_runstate.t == RunStateHelper::Mode::RunningContinuous) )
				enabler = true;
			for ( auto& at : startat )
				for ( auto& a : at)
					a.SetReadOnlyWhileScanning(_runstate);
			for ( auto& at : stopat )
				for ( auto& a : at )
					a.SetReadOnlyWhileScanning(_runstate);
			spacing.SetRWState(enabler);
			zdevicetype.SetRWState(enabler);
		}

		Timeseries::Timeseries(const uint32_t& _nmasters, const uint32_t& _nslaves)
			: nmasters(_nmasters)
			, nslaves(_nslaves)
			, totaltimes({ std::vector<ScopeNumber<double>>(nmasters), std::vector<ScopeNumber<double>>(nslaves) })
			, triggerchannel(L"/PXI-6259_0/PFI0", L"Triggerchannel")
			, triggered(false, false, true, L"Triggered")
			, alltriggered(false, false, true, L"AllTriggered")
			, repeats(1, 1, 100, L"Repeats")
			, betweenrepeats(5, 1, 600, L"BetweenRepeats_s")
			, overalltime(1, 1, 2000, L"OverallTime_s")
		{
			//  vector elements have to have different names for correct loading/saving
			std::wostringstream stream;
			for (uint32_t at = 0; at < NAreaTypes; at++) {
				uint32_t a = 0;
				std::generate_n(std::back_inserter(frames[at]), (at==0)?nmasters:nslaves, [&at, &a, &stream]() {
					stream.str(L""); stream << L"Frames" << AreaTypeString[at] << a++; return ScopeNumber<uint32_t>(1, 1, 10000, stream.str());
				});
				a = 0;
				std::generate_n(std::back_inserter(totaltimes[at]), (at==0)?nmasters:nslaves, [&at, &a, &stream]() {
					stream.str(L""); stream << L"Totaltimes_s" << AreaTypeString[at] << a++; return ScopeNumber<double>(1, 0.5, 10000, stream.str());
				});
			}
		}

		void Timeseries::Load(const wptree& pt) {
			for ( auto& at : frames)
				for ( auto& f : at )
					f.SetFromPropertyTree(pt);
			for ( auto& at : totaltimes)
				for ( auto& t : at )
					t.SetFromPropertyTree(pt);
			triggered.SetFromPropertyTree(pt);
			alltriggered.SetFromPropertyTree(pt);
			triggerchannel.SetFromPropertyTree(pt);
			repeats.SetFromPropertyTree(pt);
			betweenrepeats.SetFromPropertyTree(pt);
			overalltime.SetFromPropertyTree(pt);
			// Catch possible exceptions from Load, continue unharmed after
			try {
				uint32_t ip = 0;
				planes.clear();
				// Load complete planes until no more found (which causes an exception)
				while (true) {
					const wptree planetree = pt.get_child(boost::str(boost::wformat(L"plane%d") % ip));
					// Load single area planes
					std::array<std::vector<PlaneProperties>, NAreaTypes> ppvec;
					for (uint32_t at = 0; at < NAreaTypes; at++) {
						for (uint32_t a = 0; a < (at==ATMaster)?nmasters:nslaves; a++) {
							PlaneProperties pp;
							pp.Load(planetree.get_child(boost::str(boost::wformat(L"area%s%d") % AreaTypeString[at] % a)));
							ppvec[at].push_back(pp);
						}
					}
					planes.push_back(ppvec);
					ip++;
				}
			}
			catch (...) { // Just catch
			}
		}

		void Timeseries::Save(wptree& pt) const {
			for(const auto& at : frames)
				for ( const auto& f : at )
					f.AddToPropertyTree(pt);
			for (const auto& at : frames)
				for ( const auto& t : at )
					t.AddToPropertyTree(pt);

			triggered.AddToPropertyTree(pt);
			alltriggered.AddToPropertyTree(pt);
			triggerchannel.AddToPropertyTree(pt);
			repeats.AddToPropertyTree(pt);
			betweenrepeats.AddToPropertyTree(pt);
			overalltime.AddToPropertyTree(pt);
			uint32_t ip = 0;
			// Go through all complete planes
			for ( const auto& ppvec : planes ) {
				wptree planetree;
				for (uint32_t at = 0; at < NAreaTypes; at++) {
					std::vector<wptree> areatree((at==ATMaster)?nmasters:nslaves);
					// Go through all single area planes
					for (uint32_t a = 0; a < (at == ATMaster) ? nmasters : nslaves; a++) {
						ppvec[at][a].Save(areatree[a]);
						planetree.add_child(boost::str(boost::wformat(L"area%s%d") % AreaTypeString[at] % a), areatree[a]);
					}
				}
				pt.add_child(boost::str(boost::wformat(L"plane%d") % ip), planetree);
				ip++;
			}
		}

		void Timeseries::SetReadOnlyWhileScanning(const RunState& _runstate) {
			const bool enabler = (_runstate.t==RunStateHelper::Mode::Stopped)?true:false;
			for ( auto& at : frames)
				for ( auto& f : at )
					f.SetRWState(enabler);
			for (auto& at : frames)
				for ( auto& t : at )
					t.SetRWState(enabler);
			triggered.SetRWState(enabler);
			alltriggered.SetRWState(enabler);
			triggerchannel.SetRWState(enabler);
			repeats.SetRWState(enabler);
			betweenrepeats.SetRWState(enabler);
			overalltime.SetRWState(enabler);
		}

		Behavior::Behavior(const uint32_t& _nmasters, const uint32_t& _nslaves)
			: nmasters(_nmasters)
			, nslaves(_nslaves)
			, mode(BehaviorModeHelper::Gated, L"BehaviorMode")
			, unlimited_repeats(true, false, true, L"UnlimitedRepeats")
			, repeats(1, 1, 10000, L"Repeats")
			, gateline(L"PXI-6229/port0/line0", L"GateLine") {
		}

		void Behavior::Load(const wptree& pt) {
			mode.SetFromPropertyTree(pt);
			unlimited_repeats.SetFromPropertyTree(pt);
			repeats.SetFromPropertyTree(pt);
			gateline.SetFromPropertyTree(pt);
			// Catch possible exceptions from Load, continue unharmed after
			try {
				uint32_t ip = 0;
				planes.clear();
				// Load complete planes until no more found (which causes an exception)
				while (true) {
					const wptree planetree = pt.get_child(boost::str(boost::wformat(L"plane%d") % ip));
					// Load single area planes
					std::array<std::vector<PlaneProperties>, NAreaTypes> ppvec;
					for (uint32_t at = 0; at < NAreaTypes; at++) {
						for (uint32_t a = 0; a < (at==ATMaster)?nmasters:nslaves; a++) {
							PlaneProperties pp;
							pp.Load(planetree.get_child(boost::str(boost::wformat(L"area%s%d") % AreaTypeString[at] % a)));
							ppvec[at].push_back(pp);
						}
					}
					planes.push_back(ppvec);
					ip++;
				}
			}
			catch (...) { // Just catch
			}
		}

		void Behavior::Save(wptree& pt) const {
			mode.AddToPropertyTree(pt);
			unlimited_repeats.AddToPropertyTree(pt);
			repeats.AddToPropertyTree(pt);
			gateline.AddToPropertyTree(pt);
			uint32_t ip = 0;
			// Go through all complete planes
			for ( const auto& ppvec : planes ) {
				wptree planetree;
				for (uint32_t at = 0; at < NAreaTypes; at++) {
					std::vector<wptree> areatree((at==ATMaster)?nmasters:nslaves);
					// Go through all single area planes
					for (uint32_t a = 0; a < (at == ATMaster) ? nmasters : nslaves; a++) {
						ppvec[at][a].Save(areatree[a]);
						planetree.add_child(boost::str(boost::wformat(L"area%s%d") % AreaTypeString[at] % a), areatree[a]);
					}
				}
				pt.add_child(boost::str(boost::wformat(L"plane%d") % ip), planetree);
				ip++;
			}
		}

		void Behavior::SetReadOnlyWhileScanning(const RunState& _runstate) {
			const bool enabler = (_runstate.t==RunStateHelper::Mode::Stopped)?true:false;
			mode.SetRWState(enabler);
			unlimited_repeats.SetRWState(enabler);
			repeats.SetRWState(enabler);
		}

	}
}
