#pragma once

#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"
#include "Plane.h"

namespace scope {

	/** all parameters live in this namespace */
	namespace parameters {

		using boost::property_tree::wptree;

		/** Parameters for stack acquisition
		* @ingroup ScopeParameters */
		class Stack : public Base {
			public:
				uint32_t nareas;

				/** Start plane, measured with the zdevicetype. 0 are masters, 1 are slaves */
				std::vector<PlaneProperties> startat;
				
				/** Stop plane, measured with the zdevicetype. 0 are masters, 1 are slaves. */
				std::vector<PlaneProperties> stopat;
				
				/** Spacing between planes in microns */
				ScopeNumber<double> spacing;

				/** Vector with properties for every plane. Outer vector is planes, middle array 0 are masters, 1 slaves, inner vector is areas */
				std::vector<std::vector<PlaneProperties>> planes;
				
				/** type of z device to use */
				ScopeValue<ZDevice> zdevicetype;

				/** time in seconds for all timeseries */
				ScopeNumber<double> overalltime;

				Stack(const uint32_t& _nareas);

				/** total span of z stack */
				virtual double Range(const uint32_t& _area);
				
				/** distance between planes */
				virtual double Increment(const uint32_t& _area);

				/** space constant for exponential pockels interpolation */
				virtual double Lambda(const uint32_t& _area);

				void Load(const wptree& pt) override;
				void Save(wptree& pt) const override;
				void SetReadOnlyWhileScanning(const RunState& _runstate) override;

			protected:
				/** Updates planes with positions and pockels adjustment etc. */
				virtual void UpdatePlanes();

				/** Resets start/stop and all planes. Called when changing zdevicetype. */
				virtual void ResetPlanes();
		};

		/** Parameters for timeseries acquisition
		* @ingroup ScopeParameters */
		class Timeseries : public Base {
			public:
				Timeseries(const uint32_t& _nareas);
				
				uint32_t nareas;

				/** number of frames in timeseries for each area */
				std::vector<ScopeNumber<uint32_t>> frames;
				
				/** total acquisition time for each area */
				std::vector<ScopeNumber<double>> totaltimes;
				
				/** channel name for trigger input */
				ScopeString triggerchannel;
				
				/** the current/first repeat is triggered on triggerchannel */
				ScopeNumber<bool> triggered;

				/** every repeat is triggered */
				ScopeNumber<bool> alltriggered;

				/** How often should the timeseries be repeated */
				ScopeNumber<uint32_t> repeats;

				/** Time in seconds between the beginning of one and beginning of the next timeseries */
				ScopeNumber<double> betweenrepeats;

				/** time in seconds for all timeseries */
				ScopeNumber<double> overalltime;

				/** vector with properties for all planes, for alternating planes on repeating timeseries, inner vector are areas */
				std::vector<std::vector<PlaneProperties>> planes;

				void Load(const wptree& pt) override;
				void Save(wptree& pt) const override;
				void SetReadOnlyWhileScanning(const RunState& _runstate) override;

			protected:
				/** Check and correct times (e.g. betweenrepats must be bigger than duration of one timeseries etc) */
				void CheckTimes();
		};

		/** Parameters for behavior triggered acquisition
		* @ingroup ScopeParameters */
		class Behavior : public Base {
			public:
				Behavior(const uint32_t& _nareas);
				
				uint32_t nareas;

				/** Which mode to use */
				ScopeValue<BehaviorMode> mode;

				/** Should acquisition go on until Stop pressed (true) or until repeats done (false)? */
				ScopeNumber<bool> unlimited_repeats;

				/** How many repeats should be acquired if not unlimited_repeats? */
				ScopeNumber<uint32_t> repeats;

				/** vector with properties for all planes, for alternating planes on alternating repeats, one for each area */
				std::vector<std::vector<PlaneProperties>> planes;

				/** The %DAQmx digital line for gating. Acquisition as long as this line is high, stops on low, restarts on next high etc */
				ScopeString gateline;

				void Load(const wptree& pt) override;
				void Save(wptree& pt) const override;
				void SetReadOnlyWhileScanning(const RunState& _runstate) override;
		};

	}
}