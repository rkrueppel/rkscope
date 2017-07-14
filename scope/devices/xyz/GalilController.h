#pragma once

#include "ScopeDefines.h"

// Only use this code if we need it
#ifdef SCOPE_USE_GALIL_XYZSTAGE

// Link the correct library file depending on debug/release and x32/x64
#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "devices/xyz/galil/x64/debug/galil2.lib")
	#else
		#pragma comment(lib, "devices/xyz/galil/debug/galil2.lib")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "devices/xyz/galil/x64/release/galil2.lib")
	#else
		#pragma comment(lib, "devices/xyz/galil/release/galil2.lib")
	#endif
#endif

#include "galil/Galil.h"

namespace scope {

/** Encapsulates the RS232 connection (via Galil's library) to the Galil controller of Feinmess stages.
* Uses singleton pattern to make sure, that there is only on connection to the controller, although several different classes can control different axes.
* E.g. XYControlGalil control XY and ZControlGalil control Z, both through the same physical controller connection by using GalilController singleton. A static mutex
* is used to protect hardware access.
* @warning Also link to Galil.lib in the project properties (to debug or release, depending on configuration)!
* @ingroup ScopeComponentsHardware */
class GalilController {

public:
	/** Store _comstring for CreateInstance */
	GalilController(const std::wstring& _comstring);

	/** @return the library version */
	std::wstring LibraryVersion();

	/** @return connection details */
	std::wstring Connection();
	
	/** Sets the timeout for the current connection */
	void SetTimeout(const int32_t& _milliseconds);

	/** Send command to the Galil controller */
	std::wstring Command(const std::wstring& _cmd);

	/** Send command to the Galil controller and get a number back */
	double CommandValue(const std::wstring& _cmd);

protected:
	/** @return reference to the static instance (local variable inside CreateInstance) */
	Galil& Ctrl();

	/** @param[in] stdcomstring Connection string (ala "COM1 19200") that is passed to Galil's library
	* @return reference to the static instance inside this function
	* @warning The trick of supplying parameters to the static object creation obviously only works one time only (the first time Ctrl() is ever called) */
	static Galil& CreateInstance(const std::string& stdcomstring);

	/** For protecting concurrent hardware access */
	static std::mutex mutex;

protected:
	/** string for the RS232 connection */
	std::wstring comstring;
};


}

#endif