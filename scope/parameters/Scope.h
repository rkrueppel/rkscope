#pragma once

#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"
#include "Area.h"
#include "Stimulation.h"
#include "Plane.h"
#include "Storage.h"
#include "Runstates.h"
#include "Windows.h"

namespace scope {

	/** all parameters live in this namespace */
	namespace parameters {

		using boost::property_tree::wptree;

		/** The master parameters class
		* @ingroup ScopeParameters */
		class Scope
			: public Base {

		protected:
			/** Updates durations of timeseries from choosen frames and framerate */
			virtual void UpdateTotaltimeFromFrames();

			/** Updates number of frames from choosen duration and framerate */
			virtual void UpdateFramesFromTotaltime();

		public:
			ScopeValue<uint32_t> nareas;

			uint32_t nmasters;
			uint32_t nslaves;

			/** current date */
			ScopeString date;
	
			/** current time */
			ScopeString time;

			/** Current version (git commit hash) of Scope */
			ScopeString scopecommit;
	
			/** a comment, e.g. user name or config description */
			ScopeString comment;

			/** Vector with all areas. Ordering is: master 0, its slaves, master 1, its slaves etc. */
			std::vector<std::unique_ptr<BaseArea>> allareas;
	
			/** the StorageParameters */
			Storage storage;
	
			/** the StackParameters */
			Stack stack;
	
			/** the TimeseriesParameters */
			Timeseries timeseries;

			/** the BehaviorParameters */
			Behavior behavior;
	
			/** the parameters for the xyz stage (set type in ScopeDefines.h) */
			config::XYZStageParametersType stage;
	
			/** the StimulationParameters */
			Stimulation stimulation;

			/** The parameters for windows on the screen */
			WindowCollection frames;

			/** true: start inputs first, then outputs with output of area 0 as last, so it (e.g. /ao/StartTrigger) can serve as common master trigger for everything else
			* false: start outputs first, then inputs (area 0 last) so an FPGA doing the input can generate the sample clock for the outputs */
			ScopeNumber<bool> startinputsfirst;

			/** The trigger channel which is the internal common master trigger for all devices.
			* In a configuration with InputsDAQmx and OutputsDAQmx this should be e.g. /PXI-6259_0/ao/StartTrigger and startinputsfirst should be true;
			* or /PXI-6259_0/ai/StartTrigger and startinputsfirst false.\n
			* In a configuration with InputsFPGA and OutputsDAQmx this should be e.g. /PXI-6259_0/ao/StartTrigger and startinputsfirst should be false */
			ScopeString commontrigger;

			/** Size of the maximally reachable field of view. In a single-area system this is just that area's basemicronperpixelx * 256.
			* In a multi-area nbeam system this is
			* - if you configured that area's daq output minoutputscanner/maxoutputscanner to reach the maximal FOV: that area's basemicronperpixelx * 256
			* - if you configured that area's daq output minoutputscanner/maxoutputscanner to reach only a smaller FOV: put in here whatever you can reach
			* and reasonably image by moving the FPUs
			* In a multi-area multi-scanner system this is the maximum FOV you can reasonably image by moving the FPUs */
			ScopeNumber<double> masterfovsizex;

			/** The same for the y direction */
			ScopeNumber<double> masterfovsizey;

			/** current RunState */
			ScopeValue<RunState> run_state;
	
			/** requested acquisition mode (see DaqModeHelper) */
			ScopeValue<DaqMode> requested_mode;

		public:
			Scope(const uint32_t& _nmasters, const uint32_t& _nslaves);

			/** Supply copy constructor because of unique_ptr<Area> (does deep copy of Areas) */
			Scope(const Scope& _scope);

			/** Supply assignment operator because of unique_ptr<Area> (does deep copy of Areas) */
			Scope& operator=(const Scope& _scope);

			/** Load all from file */
			void Load(const std::wstring& filename);

			/** Save all to file */
			void Save(const std::wstring& filename) const;

			void SetReadOnlyWhileScanning(const RunState& _runstate) override;
		};


	}

}