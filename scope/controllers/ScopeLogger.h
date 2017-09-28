#pragma once

#include "helpers/Active.h"
#include "parameters/Scope.h"
#include "helpers/ScopeDatatypes.h"
#include "gui/LogFrame.h"

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
			/** Active object for running the loggings */
			Active<ControllerReturnStatus> active;

			/** path string for log file */
			std::wstring file_path;

			/** true if file path set and files opened */
			std::atomic<bool> filepath_set;

			/** File with user comments and logging of performed scans etc */
			std::wofstream userlogfile;

			/** which message types to save to disk */
			std::atomic<log_message_type> file_messages;

			/** which message types to print out on debug console */
			std::atomic<log_message_type> console_message;

			/** which message types to print in log frame */
			std::atomic<log_message_type> logframe_message;

			/** pointer to the log window frame */
			scope::gui::CLogFrame* logframe;

			/** the complete log text as a string */
			std::wstring logbooktext;

			/** a vector of all log entries done by scope */
			std::list<std::wstring> logbookentries;

			ScopeLogger();

			/** Writes message to debug console */
			ControllerReturnStatus WriteToConsole(StopCondition* const sc, const std::wstring message);
			
			/** Writes message to the logbook window */
			ControllerReturnStatus WriteToLogbox(StopCondition* const sc, const std::wstring message, const log_message_type msgtype);
			
			/** Updates the logbook window , writes complete logbook to disk, overwrites the old logbook on disk.
			* Since user can edit logbook everywhere, we always have to (over)write everything */
			ControllerReturnStatus FlushLogbox(StopCondition* const sc);
				
		public:	
			/** disable copy */
			ScopeLogger(const ScopeLogger&) = delete;

			/** disable assignment */
			ScopeLogger operator=(const ScopeLogger&) = delete;

			~ScopeLogger();
			
			static ScopeLogger& GetInstance();

			/** Sets which types of message will be logged to file, console, and logframe.*/
			void SetLoggingTypes(const log_message_type& filem, const log_message_type& consolem, const log_message_type& logframem);
			
			/** Sets the filepath and creates logfile */
			void SetFilepath(const std::wstring& _filepath = L"C:\\ScopeData");

			/** Logs a message.*/
			void Log(const std::wstring& message, const log_message_type& msgtype);

			/** Attaches a CLogFrame as the logbook window.*/
			void AttachLogFrame(gui::CLogFrame* const _logframe);

			/** Saves log frame parameters into Window (for recreating windows on startup).*/
			HWND GetLogFrameWindow();

			/** @return logbook already attached?*/
			bool HasLogFrame() const;

			/** Gets the text in the logbook window and calls FlushLogbox.*/
			void GetUserLoggings();

			/** Detaches a CLogFrame.*/
			void DetachLogFrame();

			/** Shutdown the Active, called from Run in scope.cpp.
			* avoids a dangling async thread from the ScopeLogger.*/
			void Shutdown();
	};

}