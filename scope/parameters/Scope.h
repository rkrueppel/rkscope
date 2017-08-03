#pragma once

#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"
#include "Plane.h"
#include "Storage.h"
#include "Runstates.h"
#include "Framescan.h"
#include "Devices.h"
#include "IO.h"
#include "Windows.h"

namespace scope {

/** all parameters live in this namespace */
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


/** Parameters for a whole area (includes a daq and a fpu)
* @ingroup ScopeParameters */
class Area
	: public Base {

public:
	/** the number of this area */
	ScopeNumber<uint32_t> area;

	/** true if this area is a slave area (for an n-beam system) */
	ScopeNumber<bool> isslave;

	/** const pointer to the master area parameters (provided in the constructor through parametes::Scope) */
	Area* masterarea;
	
	/** the DaqParameters for this area */
	Daq daq;

	/** the xy stage parameters for this FPU (set type in ScopeDefines.h) */
	SCOPE_FPUXYCONTROL_T fpuxystage;
	
	/** the fast z stage parameters for this FPU (set type in ScopeDefines.h) */
	SCOPE_FPUZCONTROL_T fpuzstage;
	
	/** the choosen scanner vector type */
	ScopeValue<ScannerVectorType> scanmode;

	/** This map contains all ScannerVectors that are supported by the kind of scanner */
	std::map<ScannerVectorTypeHelper::Mode, std::unique_ptr<ScannerVectorFrameBasic>> scannervectorframesmap;

	/** @name Base scale for 256x256 pixels at zoom 1 and the (maxoutput-minoutput) range set in Daq
	* @{ */
	/** Base scale in x direction for 256x256 pixels at zoom 1 and the (maxoutput-minoutput) range set in Daq */
	ScopeNumber<double> basemicronperpixelx;
	/** Base scale in y direction for 256x256 pixels at zoom 1 and the (maxoutput-minoutput) range set in Daq */
	ScopeNumber<double> basemicronperpixely;
	/** @} */

	/** @name Current scale for current resolution and zoom
	* @{ */
	ScopeNumber<double> micronperpixelx;
	ScopeNumber<double> micronperpixely;
	/** @} */

	/** Line repetition rate in Hertz */
	ScopeNumber<double> linerate;
	
	/** Frame repetition rate in Hertz */
	ScopeNumber<double> framerate;

	/** Time per frame in seconds */
	ScopeNumber<double> frametime;

	/** Histogram range for the areas */
	ScopeNumber<uint32_t> histrange;

	/** @param[in] _area the area number
	* @param[in] _isslave is this a slave area?
	* @param[in] _masterarea pointer to the masterarea parameters, or nullptr */
	Area(const uint32_t& _area = 0, const bool& _isslave = false, Area * const _masterarea = nullptr);

	/** Copy constructor (deep copy because of the pointers in the map) */
	Area(const Area& _other);

	/** Assignment (deep copy because of the pointers in the map) */
	Area& operator=(const Area& v);

	/** Virtual destructor, just in case we derive a something from Area somtime */
	virtual ~Area() { }

	/** (Re)set the pointer to the master area e.g. after a copy */
	void SetMasterArea(Area* const _masterarea);

	/** Copies parts of the ScannerVectorParameters from the master area's. Keeps Pockels and fast z */
	void CopyFromMasterArea();

	/** pointer to the current parameters::frame. */
	ScannerVectorFrameBasic& Currentframe() const;

	/** pointer to the ScannerVectorFrameSaw if implemented or throwing an exception! */
	ScannerVectorFrameSaw& FrameSaw() const;

	/** pointer to the ScannerVectorFrameResonance if implemented or throwing an exception! */
	ScannerVectorFrameResonance& FrameResonance() const;

	/** pointer to the ScannerVectorFrameBiDi if implemented or throwing an exception! */
	ScannerVectorFrameBiDi& FrameBiDi() const;

	/** pointer to the ScannerVectorFramePlaneHopper if implemented or throwing an exception! */
	ScannerVectorFramePlaneHopper& FrameHopper() const;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

	/** Save to preset in currentframe */
	virtual void SaveToPreset(const std::wstring& _name);

	/** Load from preset in currentframe */
	virtual void LoadFromPreset(const std::wstring& _name);

	/** Delete preset in currentframe */
	virtual void DeletePreset(const std::wstring& _name);

	/** Time per frame in seconds*/
	virtual double FrameTime() const;

	/** Time per line in seconds */
	virtual double LineTime() const;

	/** total number of pixels summed over all channels	 */
	virtual uint32_t TotalPixelsAllChannels() const;

	/** Gives the current framescan X offset in micrometers */
	virtual double XOffsetInMicron() const;

	/** Gives the current framescan Y offset in micrometers */
	virtual double YOffsetInMicron() const;

protected:
	/** Helper for constructors and assignment to connect internal ScopeValues (as the connections in ScopeValues do not
	* get copied automatically */
	virtual void InitializeConnections();

	/** Force update of rates etc. after change of scanmode. scanmode is connected to this. */
	virtual void ChangeScanMode();

	/** Updates framerate, frametime, and linerate */
	virtual void UpdateRates();

	/** Updates fast z boundaries on changed fast Z (ETL) calibration file */
	virtual void UpdateFastZCalibration();

	/** Current scale calculated from micronperpixelx with the resolution set in currentframe */
	virtual void CalculateMicronPerPixel();

	/** Calculates the x resolution from the x aspect ratio and the y resolution from the y aspect ratio */
	virtual void CalculateResolution();
};

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
	/** current date */
	ScopeString date;
	
	/** current time */
	ScopeString time;

	/** Current version (git commit hash) of Scope */
	ScopeString scopecommit;
	
	/** a comment, e.g. user name or config description */
	ScopeString comment;

	/** Type of scanner in the microscope */
	ScopeValue<ScannerType> scannertype;
	
	/** holds AreaParameters for all areas. Pointers, since we have to supply constructor parameters (e.g. isslave?), plain array needs default constructible. */
	std::array<std::unique_ptr<Area>, SCOPE_NAREAS> areas;
	
	/** the StorageParameters */
	Storage storage;
	
	/** the StackParameters */
	Stack stack;
	
	/** the TimeseriesParameters */
	Timeseries timeseries;

	/** the BehaviorParameters */
	Behavior behavior;
	
	/** the parameters for the xyz stage (set type in ScopeDefines.h) */
	SCOPE_XYZCONTROL_T stage;
	
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
	Scope();

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