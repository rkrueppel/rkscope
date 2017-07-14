#pragma once

#include "ScopeDefines.h"

// Only use this code if we need it
#ifdef SCOPE_USE_STANDA_XYFPUSTAGE

#include "XYControl.h"
#include "USMCDLL.h"

// Link to the Standa library. The path to that library has to be in your Scope properties->Linker->General->"Additional library directories" path!
#pragma comment(lib, "USMCDLL.lib")

// Forward declaration
namespace scope{
	namespace parameters {
	class XYControlStanda;
	}
}

namespace scope {

/** Wraps a Standa xy stage controlled by a Standa controller
* @ingroup ScopeComponentsHardware */
class XYControlStanda :
	public XYControl {

protected:
	/** protect hardware access. Do not use a static mutex, since there can be several Standa stages (one for each FPU). */
	std::mutex mutex;

	/** device numbers for x and yaxis */
	std::array<DWORD, 2> dev;

	/** movement speed in steps/sec */
	float speed;

	/** microsteps (1/8 full step) per micrometer travel */
	double microsteps_per_micron;

	/** @name Standa library parameters
	* @{ */
	USMC_Devices devices;
	USMC_State state;
	USMC_StartParameters stprms;
	USMC_Parameters prms;
	USMC_Mode mode;
	USMC_EncoderState enstate;
	/** @} */

protected:
	/** Initialize the axis (turn on power) */
	void InitAxis(const DWORD& _device);

	/** Checks for Standa error and throws exception */
	void CheckError(const DWORD& errorcode);

	/** Turn off the motor on that device and save the current position to flash memory */
	void TurnOffAndSaveToFlash(const DWORD& _device);

public:
	XYControlStanda();

	/** First stop polling, then shut off axes */
	~XYControlStanda();

	/** Initialize hardware */
	virtual void Initialize(parameters::XYControlStanda& _params);

	/** Start the worker function in the pollthread. We have to derive StartPolling here, since we need to lock the mutex */
	virtual void StartPolling() override;

	virtual void UpdatePositionValues() override;

	/** Sets current position as zero */
	virtual void SetZero() override;

	/** Move the stage relative to the current position
	* @param[in] _xrel, _yrel relative amount to move the stage in micrometers */
	virtual void MoveRelative(const double& _xrel, const double& _yrel) override;

	/** Move the stage to an absolute position
	* @param[in] _xabs,_yabs new absolute coordinates for the stage to move to in micrometers */
	virtual void MoveAbsolute(const double& _xabs, const double& _yabs) override;

	/** Get all parameters as nicely formatted string */
	std::wstring GetParameters();

	/** Get all mode information as nicely formatted string */
	std::wstring GetMode();

	/** Get state information as nicely formatted string */
	std::wstring GetState();

	/** Get start parameters as nicely formatted string */
	std::wstring GetStartParameters();
};

}

#endif
