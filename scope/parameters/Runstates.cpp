#include "stdafx.h"
#include "parameters/Runstates.h"
#include "helpers/ScopeException.h"

namespace scope {

namespace parameters {

		// Save some typing here...
		using namespace boost::property_tree;

		Stack::Stack(const uint32_t& _nareas)
			: nareas(_nareas)
			, startat(_nareas)
			, stopat(_nareas)
			, spacing(1, 0.1, 50, L"Spacing_um")
			, zdevicetype(ZDeviceHelper::ZStage, L"ZDeviceType")
			, overalltime(1, 1, 2000, L"OverallTime_s")
		{
			spacing.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
			zdevicetype.ConnectOther(std::bind(&Stack::ResetPlanes, this));
			for ( auto& s : startat ) {
				s.position.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
				s.pockels.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
			}
			for ( auto& s : stopat ) {
				s.position.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
				s.pockels.ConnectOther(std::bind(&Stack::UpdatePlanes, this));
			}
		}

		void Stack::UpdatePlanes() {
			// Find max number of planes in all areas
			uint32_t maxplanes = 0;
			for ( uint32_t a = 0 ; a < nareas ; a++ ) {
				// +1 since stop plane also counts
				uint32_t areaplanes = static_cast<uint32_t>(floor(Range(a) / spacing()) + 1);
				maxplanes = (areaplanes > maxplanes)?areaplanes:maxplanes;
			}

			/*planes___.clear();
			planes___.resize(maxplanes, std::vector<PlaneProperties>(nareas));*/
			planes___.assign(maxplanes, std::vector<PlaneProperties>(nareas));

			for ( uint32_t a = 0 ; a < nareas ; a++ ) {
				double l = Lambda(a);
				double c = stopat[a].pockels()*exp(-l*stopat[a].position());
				// +1 since stop plane also counts
				uint32_t areaplanes = static_cast<uint32_t>(floor(Range(a) / spacing()) + 1);
				DBOUT(L"Stack::UpdatePlanes area " << a << L" lambda: " << Lambda(a) << L" c: " << c);
				
				uint32_t p = 0;
				// Planes in the defined range for that area are defined with increasing position and adjusted Pockels power
				for	(  ; p < areaplanes ; p++ ) {
					planes___[p][a].position = startat[a].position() + p * Increment(a);
					planes___[p][a].pockels = c * exp(l * planes___[p][a].position());
				}
				// If another area has more planes, set this area's planes with the last position and closed Pockels cell
				for ( ; p < maxplanes ; p++ ) {
					planes___[p][a].position = planes___[areaplanes-1][a].position();
					planes___[p][a].pockels = 0;
				}
			}
		}

		void Stack::ResetPlanes() {
			planes___.clear();
			for ( uint32_t a = 0 ; a < nareas ; a++ ) {
				// Set to zero and do not call other signal (i.e. UpdatePlanes) right now
				stopat[a].position.Set(0, true, false);		
				stopat[a].pockels.Set(0, true, false);
				startat[a].position.Set(0, true, false);
				startat[a].pockels.Set(0, true, false);
			}
			UpdatePlanes();
		}

		double Stack::Range(const uint32_t& _area) {
			return abs(stopat[_area].position() - startat[_area].position());
		}

		double Stack::Increment(const uint32_t& _area) {
			return (stopat[_area].position()>startat[_area].position())?spacing():-1.0*spacing();
		}

		double Stack::Lambda(const uint32_t& _area) {
			if ( (stopat[_area].pockels() == 0) || (startat[_area].pockels() == 0) )
				return 0.0;
			if ( startat[_area].position() == stopat[_area].position() )
				return 0.0;
			return log(startat[_area].pockels()/stopat[_area].pockels()) / (startat[_area].position()-stopat[_area].position());
		}

		void Stack::Load(const wptree& pt) {
			// Catch possible exceptions from Load, continue unharmed after
			try {
				for ( uint32_t a = 0 ; a < nareas ; a++ ) {
					startat[a].Load(pt.get_child(boost::str(boost::wformat(L"startat%d") % a)));
					stopat[a].Load(pt.get_child(boost::str(boost::wformat(L"stopat%d") % a)));
				}
			}
			catch (...) { ScopeExceptionHandler(__FUNCTION__, true, true); }
			spacing.SetFromPropertyTree(pt);
			zdevicetype.SetFromPropertyTree(pt);
			overalltime.SetFromPropertyTree(pt);
		}

		void Stack::Save(wptree& pt) const {
			std::vector<wptree> ptstartat(nareas);
			std::vector<wptree> ptstopat(nareas);
			for ( uint32_t a = 0 ; a < nareas ; a++ ) {
				startat[a].Save(ptstartat[a]);
				pt.add_child(boost::str(boost::wformat(L"startat%d") % a), ptstartat[a]);
				stopat[a].Save(ptstopat[a]);
				pt.add_child(boost::str(boost::wformat(L"stopat%d") % a), ptstopat[a]);
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
			for ( auto& s : startat )
				s.SetReadOnlyWhileScanning(_runstate);
			for ( auto& s : stopat )
				s.SetReadOnlyWhileScanning(_runstate);
			spacing.SetRWState(enabler);
			zdevicetype.SetRWState(enabler);
		}

		Timeseries::Timeseries(const uint32_t& _nareas)
			: nareas(_nareas)
			, totaltimes(_nareas)
			, triggerchannel(L"/PXI-6259_0/PFI0", L"Triggerchannel")
			, triggered(false, false, true, L"Triggered")
			, alltriggered(false, false, true, L"AllTriggered")
			, repeats(1, 1, 100, L"Repeats")
			, betweenrepeats(5, 1, 600, L"BetweenRepeats_s")
			, overalltime(1, 1, 2000, L"OverallTime_s")
		{
			//  vector elements have to have different names for correct loading/saving
			std::wostringstream stream;
			uint32_t a = 0;
			std::generate_n(std::back_inserter(frames), nareas, [&a, &stream](){stream.str(L""); stream << L"Frames" << a++; return ScopeNumber<uint32_t>(1, 1, 10000, stream.str());});
			a = 0;
			std::generate_n(std::back_inserter(totaltimes), nareas, [&a, &stream](){stream.str(L""); stream << L"Totaltimes_s" << a++; return ScopeNumber<double>(1, 0.5, 10000, stream.str());});
		}

		void Timeseries::Load(const wptree& pt) {
			for ( auto& f : frames )
				f.SetFromPropertyTree(pt);
			for ( auto& t : totaltimes )
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
					std::vector<PlaneProperties> ppvec;
					for ( uint32_t a = 0 ; a < nareas ; a++ ) {
						PlaneProperties pp;
						pp.Load(planetree.get_child(boost::str(boost::wformat(L"area%d") % a)));
						ppvec.push_back(pp);
					}
					planes.push_back(ppvec);
					ip++;
				}
			}
			catch (...) { // Just catch
			}
		}

		void Timeseries::Save(wptree& pt) const {
			for ( const auto& f : frames )
				f.AddToPropertyTree(pt);
			for ( const auto& t : totaltimes )
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
				std::vector<wptree> areatree(nareas);
				// Go through all single area planes
				for ( uint32_t a = 0 ; a < nareas ; a++ ) {
					ppvec[a].Save(areatree[a]);
					planetree.add_child(boost::str(boost::wformat(L"area%d") % a), areatree[a]);
				}
				pt.add_child(boost::str(boost::wformat(L"plane%d") % ip), planetree);
				ip++;
			}
		}

		void Timeseries::SetReadOnlyWhileScanning(const RunState& _runstate) {
			const bool enabler = (_runstate.t==RunStateHelper::Mode::Stopped)?true:false;
			for ( auto& f : frames )
				f.SetRWState(enabler);
			for ( auto& t : totaltimes )
				t.SetRWState(enabler);
			triggered.SetRWState(enabler);
			alltriggered.SetRWState(enabler);
			triggerchannel.SetRWState(enabler);
			repeats.SetRWState(enabler);
			betweenrepeats.SetRWState(enabler);
			overalltime.SetRWState(enabler);
		}

		Behavior::Behavior(const uint32_t& _nareas)
			: nareas(_nareas)
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
					std::vector<PlaneProperties> ppvec;
					for ( uint32_t a = 0 ; a < nareas ; a++ ) {
						PlaneProperties pp;
						pp.Load(planetree.get_child(boost::str(boost::wformat(L"area%d") % a)));
						ppvec.push_back(pp);
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
				std::vector<wptree> areatree(nareas);
				// Go through all single area planes
				for ( uint32_t a = 0 ; a < nareas ; a++ ) {
					ppvec[a].Save(areatree[a]);
					planetree.add_child(boost::str(boost::wformat(L"area%d") % a), areatree[a]);
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