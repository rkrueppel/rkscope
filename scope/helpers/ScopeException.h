#pragma once

namespace scope {

/** @addtogroup HELPERS */
/** @{ */

/** Simple exception class for Scope. */
class ScopeException :
	public std::exception {
public:
	/** Construct with a message */
	ScopeException(const char* msg)
		: std::exception(msg)
	{}
};

/** Handles all exceptions and does nice logging
* @param[in] _origin string with the origin of the exception
* @param[in] _log true for logging to the ScopeLogger
* @param[in] _showmessagebox true for displaying a Windows message box with a warning text
* @param[in] _trace true for debug output
* @param[in] _rethrow true to rethrow the exception after handling in here */
void ScopeExceptionHandler(const std::string& _origin, const bool& _log = true, const bool& _showmessagebox = false, const bool& _trace = true, const bool& _rethrow = false);

/** Helper function that actually handles the logging to the ScopeLogger
* @param[in] _msg string with the exception's message
* @param[in] _log true for logging to the ScopeLogger
* @param[in] _showmessagebox true for displaying a Windows message box with a warning text
* @param[in] _trace true for debug output */
void __ScopeExceptionHandlerHelper(const std::string& _msg, const bool& _log, const bool& _showmessagebox, const bool& _trace);

/** Helper function that actually handles the logging to the ScopeLogger (wide string version)
* @param[in] _msg string with the exception's message
* @param[in] _log true for logging to the ScopeLogger
* @param[in] _showmessagebox true for displaying a Windows message box with a warning text
* @param[in] _trace true for debug output */
void __ScopeExceptionHandlerHelperW(const std::wstring& _msg, const bool& _log, const bool& _showmessagebox, const bool& _trace);

/** @} */

}