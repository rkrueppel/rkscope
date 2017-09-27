#include "StdAfx.h"
#include "ScopeException.h"
#include "FPGAException.h"
#include "controllers/ScopeLogger.h"

namespace scope {

void ScopeExceptionHandler(const std::string& _origin, const bool& _log, const bool& _showmessagebox, const bool& _trace, const bool& _rethrow) {
	try {
		// Rethrow the exception to catch it in this function
		throw;
	}
	catch (const ScopeException& ex) {
		std::string msg = "A Scope exception happened in " + _origin + ":\r\n" + ex.what();
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
		if (_rethrow) throw;
	}
	catch (const FPGAException& ex) {
		std::string msg = "An FPGA exception happened in " + _origin + ":\r\n" + "Status code: " + std::to_string(static_cast<long long>(ex.fpga_status)) + " Message: " + ex.what();
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
		if (_rethrow) throw;
	}
	catch (const boost::property_tree::ptree_bad_path& bp) {
		std::string msg = "A property tree exception happened in " + _origin + ":\r\n";
		msg += "The parameter \"" + bp.path<boost::property_tree::wpath>().dump() + "\" could not be found in the xml file.";
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
		if (_rethrow) throw;
	}
	catch (const std::bad_cast& exbc) {
		std::string msg = "A bad cast exception happened in " + _origin + ":\r\n" + exbc.what() + "\r\n\r\n Most probably ScopeDefines.h is misconfigured. Exit immediately.";
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
		if (_rethrow) throw;
	}
	catch (const std::bad_alloc& exba) {
		std::string msg = "A bad allocation exception happened int " + _origin + ":\r\n" + exba.what() + "\r\n\r\n Most probably there is not enough memory to allocate for Scope. Exit immediately.";
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
		if (_rethrow) throw;
	}
	catch (const std::exception& exstd) {
		std::string msg = "A STL exception happened in " + _origin + ":\r\n" + exstd.what();
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
		if (_rethrow) throw;
	}
	catch (const _com_error& comerr) {
		std::string msg = "A COM error happened in " + std::string(CW2A(comerr.Source())) + "\r\n\r\n" + std::string(CW2A(comerr.Description()));
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
	}
	catch (...) {
		std::string msg = "An unkown exception happened in " + _origin + "\r\nThis is serious. Exit Scope immediately.";
		__ScopeExceptionHandlerHelper(msg, _log, _showmessagebox, _trace);
		if (_rethrow) throw;
	}
}

void __ScopeExceptionHandlerHelper(const std::string& _msg, const bool& _log, const bool& _showmessagebox, const bool& _trace) {
	if ( _trace ) {
			DBOUT(CA2W(_msg.c_str()));
	}

	if ( _log ) {
		ScopeLogger::GetInstance().Log(std::wstring(CA2W(_msg.c_str())), log_error);
	}

	if ( _showmessagebox )
		::MessageBoxA(NULL, _msg.c_str(), "ScopeException", MB_OK | MB_ICONWARNING | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
}

void __ScopeExceptionHandlerHelperW(const std::wstring& _msg, const bool& _log, const bool& _showmessagebox, const bool& _trace) {
	if ( _trace ) {
			DBOUT(_msg.c_str());
	}

	if ( _log ) {
		ScopeLogger::GetInstance().Log(_msg, log_error);
	}

	if ( _showmessagebox )
		::MessageBoxW(NULL, _msg.c_str(), L"ScopeException", MB_OK | MB_ICONWARNING | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
}

}