#pragma once

// Forward declarations
namespace scope {
	namespace gui {
		class CLogFrame;
	}
	namespace parameters {
		class Window;
	}
}

namespace scope {

/** Types of log messages */
enum log_message_type {
	log_info = 0x01,
	log_warning = 0x02,
	log_error = 0x04,
	log_all = log_info | log_warning | log_error
};	

/** A logger class to log various messages and user comments. */
class ScopeLogger {

protected:
	/** the hidden implementation class */
	class Impl;

	/** pointer to the implementation */
	Impl* const pimpl;

protected:
	/** Has the local static implementation object
	* static local variable is in there */
	Impl& GetImpl(const log_message_type& filem, const log_message_type& consolem, const log_message_type& logboxm);					

	/** @return pointer to the hidden implementation */
	Impl* const Pimpl();

public:
	/** Sets the pimpl from GetImpl */
	ScopeLogger(const log_message_type& _filem = log_all
		, const log_message_type& _consolem = log_all
		, const log_message_type& _logboxm = log_all);

	~ScopeLogger() {}

	/** Sets the filepath and creates logfile */
	void SetFilepath(const std::wstring& _filepath = L"C:\\ScopeData");

	/** Logs a message. Calls ScopeLoggerImpl::Log. */
	void Log(const std::wstring& message, const log_message_type& msgtype);

	/** Attaches a CLogFrame as the logbook window. Calls ScopeLogger::Impl::AttachLogFrame. */
	void AttachLogFrame(gui::CLogFrame* const _logframe);

	/** Saves log frame parameters into Window (for recreating windows on startup). Calls ScopeLogger::Impl::GetLogFrameWindow. */
	HWND GetLogFrameWindow();

	/** @return logbook already attached? Calls ScopeLogger::Impl::HasLogFrame.*/
	bool HasLogFrame();

	/** Gets the text in the logbook window and calls FlushLogbox. Calls ScopeLogger::Impl::GetUserLoggings. */
	void GetUserLoggings();

	/** Detaches a CLogFrame. Calls ScopeLogger::Impl::DetachLogFrame. */
	void DetachLogFrame();

	/** Calls ScopeLogger::Impl::Shutdown */
	void Shutdown();
};

}