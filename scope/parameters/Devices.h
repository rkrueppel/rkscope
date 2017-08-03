#pragma once

#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"

namespace scope {
	
namespace parameters {

using boost::property_tree::wptree;

/** Parameters for a general XYZ stage
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class XYZControl
	: public Base {
public:
	XYZControl();

	/** position in micron of x axis */
	ScopeNumber<double> xpos;

	/** position in micron of y axis */
	ScopeNumber<double> ypos;

	/** position in micron of z axis */
	ScopeNumber<double> zpos;

	/** interval in milliseconds to poll device, be careful: long intervals slow quitting of Scope (since to stop the polling thread Scope has to wait until the polling thread does something). */
	ScopeNumber<double> pollinterval;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

/** Parameters for a Galil XYZ stage (e.g. from Feinmess)
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class XYZControlGalil
	: public XYZControl {
public:
	XYZControlGalil();

	/** connection string for Galil controller */
	ScopeString comstring;

	/** conversion factor between encoder counts and micrometers for x axis*/
	ScopeNumber<double> xcountspermicron;

	/** conversion factor between encoder counts and micrometers for y axis*/
	ScopeNumber<double> ycountspermicron;

	/** conversion factor between encoder counts and micrometers for z axis*/
	ScopeNumber<double> zcountspermicron;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

/** Parameters for a Sutter XYZ stage
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class XYZControlSutter
	: public XYZControl {
public:
	XYZControlSutter();

	/** connection string for Sutter controller */
	ScopeString comstring;

	/** conversion factor between encoder counts (microsteps) and micrometers for x axis
	* usually 0.2 (coarse) or 0.04 (fine) um/microstep -> 5 microsteps/um or 25 microsteps/um **/
	ScopeNumber<double> microstepspermicron;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

/** Parameters for a fast Z drive that is controlled by a voltage. Voltage is only generated during scanning in DaqControllerImpl
* via OutputsDAQmx. No device initialization etc needed. Parameters handle transformation from micrometer to voltage by a calibration
* map that can be loaded etc. 
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class FastZControl
	: public Base {
public:
	FastZControl();

	/** calibration map, key is absolute position in microns, value is corresponding output voltage */
	std::map<double, double> calibration;

	/** Minimum output voltage to fast z, for ETL usually 0V */
	ScopeNumber<double> minoutput;

	/** Maximum output voltage to fast z, for ETL usually 4V */
	ScopeNumber<double> maxoutput;

	/** calibration file name */
	ScopeString calibrationfile;

public:
	/** Loads calibration data from a file, overwrites old calibration map. Data structure has to be multiple lines with "position voltage" in ANSI format.
	* Updates the limits of currentposition to the max and min values read from the calibration file.
	* @param[in] _filepath path and filename
	* @return true if successful Load */
	virtual bool LoadCalibration(const std::wstring& _filepath);

	/** Converts a position in micron to voltage */
	virtual double PositionToVoltage(const double& _positionum);

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

/** Parameters for an ETL
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class FastZControlETL
	: public FastZControl {
public:
	FastZControlETL();												

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

/** Parameters for a general xy stage
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class XYControl
	: public Base {
public:
	XYControl();

	/** x position of xy device */
	ScopeNumber<double> xpos;

	/** y position of xy device */
	ScopeNumber<double> ypos;

	/** interval in milliseconds to poll device, be careful: long intervals slow quitting of Scope (since to stop the polling thread Scope has to wait until the polling thread does something). */
	ScopeNumber<double> pollinterval;

	/** size of the step for up/down/left/right buttons */
	ScopeNumber<double> buttonstepsize;

	/** string to put information about the stage in (do no Load or Save this) */
	ScopeString stageinfo;														

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

/** Parameters for a Galil controller xy stage (e.g. by Feinmess)
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class XYControlGalil
	: public XYControl {
public:
	XYControlGalil();

	/** connection string for Galil controller */
	ScopeString comstring;

	/** conversion factor between encoder counts and micrometers */
	ScopeNumber<double> countspermicron;										

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};

/** Parameters for a Standa xy stage
* @ingroup ScopeComponentsHardware
* @ingroup ScopeParameters */
class XYControlStanda
	: public XYControl {
public:
	XYControlStanda();

	/** Standa device number for x axis */
	ScopeNumber<uint32_t> devicex;

	/** Standa device number for y axis */
	ScopeNumber<uint32_t> devicey;

	/** movement speed in steps/sec */
	ScopeNumber<double> speed;

	/** number of microsteps (1/8 full step) per micrometer, 3.226 microsteps per micrometer from Standa 8MTF documentation */
	ScopeNumber<double> microstepspermicron;									

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
};


}

}