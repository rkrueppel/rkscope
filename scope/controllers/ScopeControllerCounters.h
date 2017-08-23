#pragma once
#include "helpers/ScopeDatatypes.h"

namespace scope {
	
	/** Keeps the counters used by the ScopeController together. */
	struct ScopeControllerCounters {
		/** Updated from PipelineController::Run */
		std::vector<ScopeNumber<double>> singleframeprogress;

		/** Updated from ScopeController::RunStack, connected to progress indicator in CStackSettingsPage */
		ScopeNumber<double> planecounter;

		/** Updated from PipelineController::Run, connected to progress indicator in CTimeSeriesSettingsPage or edit control in CBehaviorSettingsPage */
		std::vector<ScopeNumber<double>> framecounter;

		/** Updated from ScopeController::RunTimeseries, connected to progress indicator in CTimeSeriesSettingsPage */
		ScopeNumber<double> repeatcounter;

		/** Updated from ScopeController::RunBehavior, connected to edit control in CBehaviorSettingsPage */
		ScopeNumber<double> trialcounter;

		/** Updated from e.g. ScopeController::RunBehavior, connected to time indicator in CBehaviorSettingsPage */
		ScopeNumber<double> totaltime;
		
		ScopeControllerCounters(const uint32_t& _nareas)
			: singleframeprogress(_nareas, 0)
			, planecounter(0.0, 0.0, 100.0, L"PlaneCounter")
			, framecounter(_nareas, 0)
			, repeatcounter(0.0, 0.0, 1000.0, L"RepeatCounter")
			, trialcounter(0.0, 0.0, 10000.0, L"TotalTime")
			, totaltime(0.0, 0.0, 10000.0, L"TrialCounter")
		{}
	};

}