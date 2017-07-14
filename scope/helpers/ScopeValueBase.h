#pragma once

namespace scope {

/** Base class for a thread-safe value, with signals that are called on value changes.
* It uses a mutex to protect the value and boost::signals2 to call connected function. */
class ScopeValueBase {

protected:
	/** the name of the value */
	std::wstring name;

	/** a mutex for protection. Is mutable so we can have accessor functions as const! */
	mutable std::mutex mutex;

	/** read-write state, true if read&write, false if onyl read (not an atomic, since access to the data variable has to be protected while we change readonly) */
	bool readwrite;

	/** typedef for change signal slot functions */
	typedef boost::signals2::signal<void ()> signalchange_t;

	/** typedef for state signal slot functions */
	typedef boost::signals2::signal<void (bool)> signalstate_t;

	/** signal that is called for GUI on value changes */
	signalchange_t changesiggui;

	/** signal that is called for other stuff on value changes */
	signalchange_t changesigother;

	/** signal that is called on changes of read/write status */
	signalstate_t statesig;

public:
	/** Initialize name and readonly */
	ScopeValueBase(const std::wstring& _name = L"None")
		: readwrite(true)
		, name(_name) {
	}

	/** Safe copy. Signals are not copied! */
	ScopeValueBase(const ScopeValueBase& _svb)
		: readwrite(_svb.GetRWState())
		, name(_svb.Name()) {
	}

	/** Safe assignment. Signals are not assigned! */
	ScopeValueBase& operator=(const ScopeValueBase& _svb) {
		// Avoid self-assignment
		if ( this != &_svb ) {
			std::lock_guard<std::mutex> lock(mutex);
			readwrite = _svb.GetRWState();
			name = _svb.Name();
		}
		return *this;
	}

	/** Disconnects all slots from signals */
	virtual ~ScopeValueBase() {
		std::lock_guard<std::mutex> lock(mutex);
		// If anything still connected now is the time to let it go...
		changesiggui.disconnect_all_slots();
		changesigother.disconnect_all_slots();
		statesig.disconnect_all_slots();
	}

	/** @return the values name */
	std::wstring Name() const { 
		std::lock_guard<std::mutex> lock(mutex);
		return name;
	}

	/** Set the name */
	void SetName(const std::wstring& _name) {
		std::lock_guard<std::mutex> lock(mutex);
		name = _name;
	}

	/** Sets readonly/read-write state. True = read&write, false = readonly. */
	void SetRWState(const bool& _state) {
		{
		std::lock_guard<std::mutex> lock(mutex);
		readwrite = _state;
		}
		statesig(_state);
	}

	/** @return true if read&write, false if read-only */
	bool GetRWState() const {
		std::lock_guard<std::mutex> lock(mutex);
		return readwrite;
	}

	/** Connect signal to slot to GUI. Save the connection object and call its disconnect to disconnect. */
	boost::signals2::connection ConnectGUI(signalchange_t::slot_type slot) {
		// signals2 is thread-safe, we do not have to protect method calls on signals with a mutex
		return changesiggui.connect(slot);
	}

	/** Connect signal to slot to other stuff. Save the connection object and call its disconnect to disconnect. */
	boost::signals2::connection ConnectOther(signalchange_t::slot_type slot) {
		return changesigother.connect(slot);
	}

	/** Connect signal to slot for state changes. Save the connection object and call its disconnect to disconnect. */
	boost::signals2::connection ConnectState(signalstate_t::slot_type slot) {
		return statesig.connect(slot);
	}
};

}