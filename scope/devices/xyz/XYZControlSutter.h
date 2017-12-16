#pragma once

// Only use this code if we need it
#ifdef SCOPE_USE_SUTTER_XYZSTAGE

#include "XYZControl.h"
#include "SutterController.h"

namespace scope {

/** Wraps a Sutter xyz stage controlled by a Sutter controller.
* @ingroup ScopeComponentsHardware */
class XYZControlSutter 
	: public XYZControl {

protected:
	/** SutterController is a singleton */
	std::unique_ptr<SutterController> sc;

	/** conversion factor between encoder counts (microsteps) and micrometers for all axis.
	* Usually 0.2 (coarse) or 0.04 (fine) um/microstep -> 5 microsteps/um or 25 microsteps/um.
	* See also parameters::XYZControlSutter **/
	ScopeNumber<double> microstepspermicron;

	/** @name device properties read by status query
	* @{ */
	uint16_t stepdiv;
	uint16_t stepmult;
	uint16_t speed;
	uint16_t version;
	/** @} */

	/** Helper to convert byte vector read from controller into integers */
	union SutterValue16 {
		/** integer */
		uint16_t intval;
		/** byte array */
		unsigned char charval[2];
	};

	/** Helper to convert byte vector read from controller into integers */
	union SutterValue32 {
		/** integer */
		uint32_t intval;
		/** byte array */
		unsigned char charval[4];
	};

	/** @name Helper functions to convert between byte vector and integers
	* @{ */
	uint16_t BytesToInt16(const std::vector<BYTE>& _bytevec, const uint32_t& _from);
	std::vector<BYTE> Int16ToBytes(const uint16_t& _i);
	uint32_t BytesToInt32(const std::vector<BYTE>& _bytevec, const uint32_t& _from);
	std::vector<BYTE> Int32ToBytes(const uint32_t& _i);
	/** @} */

	/** Queries the device status and updates data members */
	void UpdateStatus();

	/** Sets movement velocity in micron/s, usually called once from Initialize */
	void SetVelocity(const uint16_t& _vel);

public:
	XYZControlSutter(void);

	/** First stop polling, then shut off axes */
	~XYZControlSutter(void);

	/** Initialize hardware */
	void Initialize(parameters::XYZControlSutter& _params);

	void UpdatePositionValues() override;

	void SetZero() override;

	void MoveRelative(const double& _xrel, const double& _yrel, const double& _zrel) override;

	void MoveAbsolute(const double& _xabs, const double& _yabs, const double& _zabs) override;

};

}

#endif