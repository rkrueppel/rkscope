#pragma once
#include "helpers/ScopeDatatypes.h"
#include "helpers\ScopeNumber.h"

namespace scope {
	
	/** Keeps the counters used by TheScope together. */
	template<uint32_t NAREAS>
	struct ScopeCounters {
		/** Updated from PipelineController::Run */
		std::array<ScopeNumber<double>, NAREAS> singleframeprogress;

		/** Updated from ScopeController::RunStack, connected to progress indicator in CStackSettingsPage */
		ScopeNumber<double> planecounter;

		/** Updated from PipelineController::Run, connected to progress indicator in CTimeSeriesSettingsPage or edit control in CBehaviorSettingsPage */
		std::array<ScopeNumber<double>, NAREAS> framecounter;

		/** Updated from ScopeController::RunTimeseries, connected to progress indicator in CTimeSeriesSettingsPage */
		ScopeNumber<double> repeatcounter;

		/** Updated from ScopeController::RunBehavior, connected to edit control in CBehaviorSettingsPage */
		ScopeNumber<double> trialcounter;

		/** Updated from e.g. ScopeController::RunBehavior, connected to time indicator in CBehaviorSettingsPage */
		ScopeNumber<double> totaltime;
		
		ScopeCounters()
			: planecounter(0.0, 0.0, 100.0, L"PlaneCounter")
			, repeatcounter(0.0, 0.0, 1000.0, L"RepeatCounter")
			, trialcounter(0.0, 0.0, 10000.0, L"TotalTime")
			, totaltime(0.0, 0.0, 10000.0, L"TrialCounter")
		{
			
		}
	};

}