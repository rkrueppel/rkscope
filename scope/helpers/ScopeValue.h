#pragma once

#include "ScopeValueBase.h"
#include "helpers/ScopeDatatypes.h"
#include "ScopeException.h"

namespace scope {

/** A templated class for a thread-safe value, with signals to GUI or other stuff that are called on value changes */
template<class T>
class ScopeValue
	: virtual public ScopeValueBase {

protected:
	/** the value proper */
	T value;

protected:
	/** When setting the value it can be coerced by a derivation of this function */
	virtual inline T CoerceValue(const T& v) const { return v; }

public:
	/** Construct and set silently */
	ScopeValue(const T& _value = T(0), const std::wstring& _name = L"None")
		:ScopeValueBase(_name)
		, value(_value) {
	}

	/** Safe assignment */
	ScopeValue& operator=(const ScopeValue& v) {
		// Avoid self-assignment
		if ( this != &v )
			Set(v.Value(), true, true, false);
		// assign name and rw state (after we have set the value)
		ScopeValueBase::operator=(v);
		return *this;
	}

	/** Safe assignment */
	ScopeValue& operator=(const T& v) {
		Set(v, true, true, false);
		return *this;
	}

	/** @return the value */
	T Value() const {
		std::lock_guard<std::mutex> lock(mutex);
		return value;
	}

	/** @return the value */
	T operator() () const {
		return Value();
	}

	/** @return the value */
	operator T() const {
		return Value();
	}

	/** Sets the value and calls both change signals if the value actually changed
	* @param[in] _v the new value
	* @param[in] _callguisignal call signal to GUI slots
	* @param[in] _callothersignal call signal to other slots
	* @param[in] _callatnochange call signals also if value did not change
	* @return the current value (i.e. the coerced _v). */
	virtual T Set(const T& _v, const bool& _callguisignal = true, const bool& _callothersignal = true, const bool& _callatnochange = false) {
		T tmp;
		bool changed = false;
		// protected
		{
			std::lock_guard<std::mutex> lock(mutex);
			// If we try to set while onyl read access return immediately
			if ( !readwrite )
				return value;
			// Check if value is actually changed
			if ( value != _v ) {
				changed = true;
				value = CoerceValue(_v);
			}
			// keep the value for later outside the protected area
			tmp = value;
		}
		// call signals after unlocking the mutex to avoid possible deadlock!
		// Since signals2 is thread-safe we do not have to protect call to signal methods with a mutex.
		// avoid calling when variable was not changed unless desired.
		if ( changed || _callatnochange) {
			if ( _callguisignal )
				changesiggui();
			if ( _callothersignal )
				changesigother();
		}
		return tmp;
	}

	/** Parses a wstring and sets the value accordingly. See ScopeValue.cpp for specializations. */
	void SetFromString(const std::wstring& str);

	/** Converts value into a wstring. See ScopeValue.cpp for specializations. */
	std::wstring ToChar() const;

	/** Adds the value to a  Boost property tree, using its name and value */
	void AddToPropertyTree(boost::property_tree::wptree& pt) const {
		pt.add<std::wstring>(Name(), ToChar());
	}

	/** Set value from a Boost property, using its name as a key */
	void SetFromPropertyTree(const boost::property_tree::wptree& pt) {
		try {
			SetFromString(pt.get<std::wstring>(Name()));
		}	// catch if we did not find this ScopeValue in the property tree
		catch (...) {
			ScopeExceptionHandler(__FUNCTION__, true, true);
		}
	}
};

}