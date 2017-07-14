#pragma once

namespace scope {

/** Wraps hardware connection for signal output to scanners, fast z control, and pockels cell. */
class Outputs {

protected:
	/** which area this one is for */
	const uint32_t area;

	/** Signal for aborting a write operation (since write is performed in several blocks, abort can happen in between) */
	std::atomic<bool> writeabort;

public:
	/** get the area and set up everything */
	Outputs(const uint32_t& _area);

	virtual ~Outputs(void);

	/** Start task. */
	virtual void Start() { }

	/** Stops task. */
	virtual void Stop() { }

	/** Writes values for one frame of X-Y-fastZ and pockels data to the device buffer.
	* Depending on the implementation in the derived classes, this _xyzp can be a complete frame or e.g. only one x line (for x and p), one y column (for y and fast z)
	* , which are then repeated by the device.
	* @param[in] _xyzp the scannervector with xyzp data to write to device
	* @param[in] _blocks in how many blocks should the scannervector be written (more blocks faster update, since smaller blocksize -> buffer for this is free earlier), see "DAQmx quick buffer update.vi" */
	virtual int32_t Write(const std::vector<int16_t>& _xyzp, const uint32_t& _blocks = 1) { return 0; }

	/** Aborts a running Write by setting writeabort to true (which is checked on every block-write of Write) */
	virtual void AbortWrite();

};

}

