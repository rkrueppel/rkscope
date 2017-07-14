#pragma once

#ifdef SCOPE_USE_SUTTER_XYZSTAGE

#include "SerialConnection.h"

namespace scope {

/** Encapsulates the RS232 connection (Win32 API serial connection) to the Sutter controller.
* Uses singleton pattern to make sure, that there is only on connection to the controller, although several different classes can control different axes.
* A static mutex is used to protect hardware access.
* @ingroup ScopeComponentsHardware */
class SutterController {

public:
	/** store comstring for CreateInstance */
	SutterController(const std::wstring& _comstring);

	/** Send command to the Sutter controller, and get a vector with _answerbytes back */
	std::vector<BYTE> Command(const std::string& _cmd, const uint32_t& _answerbytes);

protected:
	/** @return reference to the static instance (local variable inside CreateInstance) */
	SerialConnection& Ctrl();

	/** @param[in] _comstring Connection string (ala "COM10") that is used for the serial connection
	* @return reference to the static instance inside this function
	* @warning The trick of supplying parameters to the static object creation obviously only works one time only (the first time Ctrl() is ever called) */
	static SerialConnection& CreateInstance(const std::wstring& _comstring);

	/** For protecting concurrent hardware access */
	static std::mutex mutex;

	/** Check if last received byte is "\r", otherwise throw */
	void CheckError(const std::vector<BYTE>& _received);

protected:
	/** string for the RS232 connection */
	std::wstring comstring;

};

}

#endif