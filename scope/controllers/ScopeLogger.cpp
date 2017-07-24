#include "stdafx.h"
#include "ScopeLogger.h"
#include "ScopeLogger_p.h"
#include "gui/LogFrame.h"
#include "parameters/Windows.h"

namespace scope {
	
ScopeLogger::ScopeLogger()
	: file_path(L"")
	, filepath_set(false)
	, file_messages(log_all)
	, console_message(log_all)
	, logframe_message(log_all)
	, logframe(nullptr)
	, logbookentries(1, L"")
	, logbooktext(L"") {
	Log(L"Logbook started", log_info);
}

ScopeLogger::~ScopeLogger() {
	if ( userlogfile.is_open() )
		userlogfile.close();
}

ScopeLogger& ScopeLogger::GetInstance() {
	static ScopeLogger sl();
	return sl;
}

ControllerReturnStatus ScopeLogger::WriteToConsole(StopCondition* const sc, const std::wstring message) {
	DBOUT(L"" << message.c_str());
	return finished;
}
	
ControllerReturnStatus ScopeLogger::WriteToLogbox(StopCondition* const sc, const std::wstring message, const log_message_type msgtype) {
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

ControllerReturnStatus ScopeLogger::FlushLogbox(StopCondition* const sc) {
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

void ScopeLogger::SetLoggingTypes(const log_message_type& filem, const log_message_type& consolem, const log_message_type& logframem) {
	file_messages = filem;
	console_message = consolem;
	logframe_message = logframem;
}

void ScopeLogger::SetFilepath(const std::wstring& _filepath) {
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

void ScopeLogger::Log(const std::wstring& message, const log_message_type& msgtype) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::wostringstream logmsg;
	logmsg << st.wHour << L":" << std::setfill(L'0') << std::setw(2) << st.wMinute << L"::" << st.wSecond << L" - ";
	logmsg << message;
	active.Send(std::bind(&WriteToConsole, this, std::placeholders::_1, logmsg.str()));
	// Console is flushed to disk 
	active.Send(std::bind(&WriteToLogbox, this, std::placeholders::_1, logmsg.str(), msgtype));
}

void ScopeLogger::AttachLogFrame(gui::CLogFrame* const _logframe) {
	logframe = _logframe;
	active.Send(std::bind(&FlushLogbox, this, std::placeholders::_1));
}

HWND ScopeLogger::GetLogFrameWindow() {
	return this->logframe->m_hWnd;
}

bool ScopeLogger::HasLogFrame() const {
	return logframe!=nullptr;
}

void ScopeLogger::GetUserLoggings() {
	if ( logframe != nullptr )
		logbooktext = logframe->GetLogText();
	active.Send(std::bind(&FlushLogbox, this, std::placeholders::_1));
}

void ScopeLogger::DetachLogFrame() {
	logbooktext = logframe->GetLogText();
	logframe = nullptr;
}

void ScopeLogger::Shutdown() {
	active.Quit();
}

	
}