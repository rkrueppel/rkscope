#include "stdafx.h"
#include "Stimulation.h"

namespace scope {
	namespace parameters {

		using boost::property_tree::wptree;

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
			const bool enabler = (_runstate.t == RunStateHelper::Mode::Stopped) ? true : false;
			duration.SetRWState(enabler);
			enable.SetRWState(enabler);
			offtime.SetRWState(enabler);
			onset.SetRWState(enabler);
			ontime.SetRWState(enabler);
		}


	}
}