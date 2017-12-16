#pragma once

// Only use this code if we need it
#ifdef SCOPE_USE_GALIL_XYZSTAGE

#include "XYZControl.h"

// Forward declaration
namespace scope{
	class GalilController;
	namespace parameters {
		class XYZControlGalil;
	}
}

namespace scope {

/** Wraps an xyz stage controlled by a Galil controller (e.g. from Feinmess).
* @ingroup ScopeComponentsHardware */
class XYZControlGalil :
	public XYZControl {

protected:
	/** GalilController is a singleton */
	std::unique_ptr<GalilController> gc;

	/** conversion factor between encoder counts and micrometers for x axis*/
	double xcountspermicron;

	/** conversion factor between encoder counts and micrometers for y axis*/
	double ycountspermicron;

	/** conversion factor between encoder counts and micrometers for z axis*/
	double zcountspermicron;

public:
	XYZControlGalil();
	~XYZControlGalil();

	/** Initialize with parameters. Not an override of base clase method, because different parameter type! */
	void Initialize(parameters::XYZControlGalil& _params);

	void UpdatePositionValues() override;

	double CurrentXPosition() override;

	double CurrentYPosition() override;

	double CurrentZPosition() override;

	void SetZeroXAxis() override;

	void SetZeroYAxis() override;

	void SetZeroZAxis() override;

	void MoveRelative(const double& _xrel, const double& _yrel, const double& _zrel) override;

	void MoveAbsolute(const double& _xabs, const double& _yabs, const double& _zabs) override;
};

}

#endif