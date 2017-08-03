#pragma once

#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"
#include "Inputs.h"
#include "Outputs.h"

namespace scope {

/** all parameters live in this namespace */
namespace parameters {

using boost::property_tree::wptree;

/** All parameters for scanner data generation and pixel acquisition
* If you add/remove parameters or derive classes from this, do not forget to update the Load and Save methods for your new members!
* @ingroup ScopeParameters */
class Daq
	: public Base {

public:
	/** Constructor */
	Daq(const bool& _isslave);

	/** Supply copy constructor because of the unique_ptrs to base class */
	Daq(const Daq& _daq);

	/** Supply assignment operator because of the unique_ptrs to base class */
	Daq& operator=(const Daq& _daq);

	/** true if belongs to a slave area */
	const bool isslave;

	/** the input parameters */
	std::unique_ptr<Inputs> inputs;

	/** the output parameters */
	std::unique_ptr<Outputs> outputs;
	
	/** digital output line of the shutter */
	ScopeString shutterline;

	/** digital output line turning the resonance scanner on and off */
	ScopeString switchresonanceline;

	/** digital output line of the zoom factor for a resonance scanner*/
	ScopeString zoomresonanceline;
	
	/** pixel dwell time in microseconds, this is also the analog out sampling interval */
	ScopeNumber<double> pixeltime;
	
	/** compensate, by waiting, the time that scanners lag behind the command voltage, in microseconds */
	ScopeNumber<double> scannerdelay;
	
	/** number of images to average */
	ScopeNumber<uint32_t> averages;
	
	/** number of frames to acquire */
	ScopeNumber<uint32_t> requested_frames;

	/** frequency of the resonance scanner used, which is especially the frequency of the synchronization signal */
	ScopeNumber<uint32_t> resonance_frequency;

	/** @return the scanner delay in samples */
	int32_t ScannerDelaySamples(const bool& _respectoversampling) const;
 
	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

protected:
	/** Coerces pixeltime to a value that both inputs and outputs support. */
	void CoercePixeltime();
};


}

}
