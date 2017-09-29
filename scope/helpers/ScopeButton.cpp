#include "StdAfx.h"
#include "ScopeButton.h"

namespace scope {

	ScopeButton::ScopeButton(const std::wstring& _name)
		: name(_name)
		, enabled(true) {
	}

	ScopeButton::ScopeButton(const ScopeButton& sb)
		: name(sb.name)
		, enabled(sb.enabled) {
	}

	ScopeButton& ScopeButton::operator=(const ScopeButton& sb) {
		name = sb.name;
		enabled = sb.enabled;
		return *this;
	}

	std::wstring ScopeButton::Name() const {
		return name;
	}

	void ScopeButton::Enable(const bool& state) {
		enabled = state;
		m_ctrlstatesig(enabled);
	}

	void ScopeButton::Click() {
		m_changesig();
	}

	boost::signals2::connection ScopeButton::Connect(signalchange_t::slot_type slot) {
		return m_changesig.connect(slot);
	}

	boost::signals2::connection ScopeButton::ConnectControlState(signalstate_t::slot_type slot) {
		return m_ctrlstatesig.connect(slot);
	}

	void ScopeButton::Disconnect(signalchange_t::slot_type slot) {
		m_changesig.disconnect(&slot);
	}

	void ScopeButton::DisconnectControlState(signalstate_t::slot_type slot) {
		m_ctrlstatesig.disconnect(&slot);
	}

}
