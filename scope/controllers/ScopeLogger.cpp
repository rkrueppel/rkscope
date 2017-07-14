#include "stdafx.h"
#include "ScopeLogger.h"
#include "ScopeLogger_p.h"
#include "gui/LogFrame.h"
#include "parameters/Windows.h"

namespace scope {

ScopeLogger::Impl& ScopeLogger::GetImpl(const log_message_type& filem, const log_message_type& consolem, const log_message_type& logboxm) {
	static Impl sImpl(filem, consolem, logboxm);
	return sImpl;
}

ScopeLogger::Impl* const ScopeLogger::Pimpl() {
	return pimpl;
}

ScopeLogger::ScopeLogger(const log_message_type& filem, const log_message_type& consolem, const log_message_type& logboxm)
	: pimpl(&GetImpl(filem, consolem, logboxm)) {
}

void ScopeLogger::SetFilepath(const std::wstring& _filepath) {
	Pimpl()->SetFilepath(_filepath);
}

void ScopeLogger::Log(const std::wstring& message, const log_message_type& msgtype) {
	Pimpl()->Log(message, msgtype);
}

void ScopeLogger::AttachLogFrame(gui::CLogFrame* const _logframe) {
	Pimpl()->AttachLogFrame(_logframe);
}

HWND ScopeLogger::GetLogFrameWindow() {
	return Pimpl()->GetLogFrameWindow();
}

bool ScopeLogger::HasLogFrame() {
	return Pimpl()->HasLogFrame();
}

void ScopeLogger::GetUserLoggings() {
	Pimpl()->GetUserLoggings();
}

void ScopeLogger::DetachLogFrame() {
	Pimpl()->DetachLogFrame();
}

void ScopeLogger::Shutdown() {
	Pimpl()->Shutdown();
}

}