#pragma once

#include "ScopeLogger.h"
#include "helpers/Active.h"
#include "parameters/Scope.h"
#include "ScopeDatatypes.h"
#include "gui/LogFrame.h"

namespace scope {

/** Implementation class of the ScopeLogger */
class ScopeLogger::Impl {

protected:
	/** Active object for running the loggings */
	Active<ControllerReturnStatus> active;

	/** path string for log file */
	std::wstring file_path;

	/** true if file path set and files opened */
	bool filepath_set;

	/** File with user comments and logging of performed scans etc */
	std::wofstream userlogfile;

	/** which message types to save to disk */
	log_message_type file_messages;

	/** which message types to print out on debug console */
	log_message_type console_message;

	/** which message types to print in log frame */
	log_message_type logframe_message;

	/** pointer to the log window frame */
	gui::CLogFrame* logframe;

	/** the complete log text as a string */
	std::wstring logbooktext;

	/** a vector of all log entries done by scope */
	std::list<std::wstring> logbookentries;

protected:
	/** disable copy */
	Impl(const Impl& i);

	/** disable assignment */
	Impl operator=(const Impl& i);

	/** Writes message to debug console */
	ControllerReturnStatus WriteToConsole(StopCondition* const sc, const std::wstring message) {
		DBOUT(L"" << message.c_str());
		return finished;
	}

	/** Writes message to the logbook window */
	ControllerReturnStatus WriteToLogbox(StopCondition* const sc, const std::wstring message, const log_message_type msgtype) {
		logbookentries.push_back(message+L"\r\n");

		// Get current logbook stuff from log window
		if ( logframe != nullptr )
			logbooktext = logframe->GetLogText();
		
		// Add linebreak if needed
		if ( logbooktext.length() > 2 )
			if ( logbooktext.substr(logbooktext.length()-2, 2) != L"\r\n" )
				logbooktext += L"\r\n";
		
		// Add message
		logbooktext += message+L"\r\n";
		
		// Flush to disk and log window
		FlushLogbox(sc);

		return finished;
	}

	/** Updates the logbook window , writes complete logbook to disk, overwrites the old logbook on disk.
	* Since user can edit logbook everywhere, we always have to (over)write everything */
	ControllerReturnStatus FlushLogbox(StopCondition* const sc) {
		if ( logframe != nullptr )
			logframe->ReplaceLogText(logbooktext);
		if ( filepath_set ) {
			if ( !userlogfile.is_open() )
				userlogfile.open(file_path, std::ios_base::out | std::ios_base::trunc);	// Trunc: overwrite
			userlogfile << logbooktext;
			userlogfile.close();
		}
		return ControllerReturnStatus::finished;
	}

public:
	/** Create logfiles */
	Impl(const log_message_type& filem = log_all, const log_message_type& consolem = log_all, const log_message_type& logboxm = log_all)
		: file_path(L"")
		, filepath_set(false)
		, file_messages(filem)
		, console_message(consolem)
		, logframe_message(logboxm)
		, logframe(nullptr)
		, logbookentries(1, L"")
		, logbooktext(L"") {
		Log(L"Logbook started", log_info);
	}

	~Impl() {
		if ( userlogfile.is_open() )
			userlogfile.close();
	}

	/** Sets the filepath and creates logfile */
	void SetFilepath(const std::wstring& _filepath = L"C:\\ScopeData") {
		if ( !filepath_set ) {
			file_path = _filepath + L"\\log_" + GetCurrentDateString() + L"_" + GetCurrentTimeString(true) + L".txt";

			userlogfile.open(file_path);
			if ( userlogfile.bad() )
				throw std::invalid_argument("Unable to open user log file\n");
			userlogfile.close();		// We only wanted it to be cleared/old overwritten, do not keep it open
	
			filepath_set = true;

			Log(L"Logfile started", log_info);
		}
	}

	/** Logs a message */
	void Log(const std::wstring& message, const log_message_type& msgtype) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		std::wostringstream logmsg;
		logmsg << st.wHour << L":" << std::setfill(L'0') << std::setw(2) << st.wMinute << L"::" << st.wSecond << L" - ";
		logmsg << message;
		active.Send(std::bind(&Impl::WriteToConsole, this, std::placeholders::_1, logmsg.str()));
		// Console is flushed to disk 
		active.Send(std::bind(&Impl::WriteToLogbox, this, std::placeholders::_1, logmsg.str(), msgtype));
	}

	/** Attaches a CLogFrame as the logbook window */
	void AttachLogFrame(gui::CLogFrame* const _logframe) {
		logframe = _logframe;
		active.Send(std::bind(&Impl::FlushLogbox, this, std::placeholders::_1));
	}

	/** Saves log frame parameters into Window (for recreating windows on startup) */
	HWND GetLogFrameWindow() {
		return this->logframe->m_hWnd;
	}

	/** @return logbook already attached? */
	bool HasLogFrame() const {
		return logframe!=nullptr;
	}

	/** Gets the text in the logbook window and calls FlushLogbox */
	void GetUserLoggings() {
		if ( logframe != nullptr )
			logbooktext = logframe->GetLogText();
		active.Send(std::bind(&Impl::FlushLogbox, this, std::placeholders::_1));
	}

	/** Detaches a CLogFrame */
	void DetachLogFrame() {
		logbooktext = logframe->GetLogText();
		logframe = nullptr;
	}

	/** Shutdown the Active, called from Run in scope.cpp.
	* avoids a dangling async thread from the ScopeLogger (generates problems since ScopeLoggerImpl is
	* static and destructor is called very late/at undefined timepoint). */
	void Shutdown() {
		active.Quit();
	}
};

}

