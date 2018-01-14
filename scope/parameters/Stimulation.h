#pragma once

#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"

namespace scope {
	namespace parameters {

		using boost::property_tree::wptree;

		/** Parameters for digital stimulation output
		* @ingroup ScopeParameters */
		class Stimulation
			: public Base {

		public:
			Stimulation();

			/** digital output channel */
			ScopeString channel;

			/** timing source. Be careful NI M-series devices can not divide down their onboard clock to provide a sample clock for
			* digital I/O (see http://www.ni.com/white-paper/3615/en under 4. / Shared sample clock.
			* Thus using "" (and providing a reference clock) or "OnboardClock" does not work here! Put in a sample clock
			* from e.g. the analog output task or provide the sample clock via the PXI_Trig from the FPGA! */
			ScopeString timingsource;

			/** stimulation enabled/disabled */
			ScopeNumber<bool> enable;

			/** onset of stimulation */
			ScopeNumber<double> onset;

			/** total duration of stimulation */
			ScopeNumber<double> duration;

			/** on time of one stimulation pulse */
			ScopeNumber<double> ontime;

			/** off time after stimulation pulse */
			ScopeNumber<double> offtime;

			void Load(const wptree& pt) override;
			void Save(wptree& pt) const override;
			void SetReadOnlyWhileScanning(const RunState& _runstate) override;
		};



	}
}
