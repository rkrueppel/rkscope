#pragma once

#include "ScopeValue.h"

namespace scope {

/** A templated class for a thread-safe numeric value, with signals that are called on value changes.
* Derived from ScopeValue and ScopeValueBase (virtual, since ScopeValue is also derived from ScopeValueBase; but we want ScopeValueBase constructor). */
template <class T>
class ScopeNumber
	: public ScopeValue<T>
	, virtual public ScopeValueBase {

protected:
	/** lower limit (including) to which the value is coerced */
	T lowerLimit;

	/** upper limit (including) to which the value is coerced */
	T upperLimit;
	
	/** Does the actual coercion in between the lowerLimit and upperLimit. Not thread-safe. */
	inline T CoerceValue(const T& v) const override { 
		return( ( v < lowerLimit ) ? lowerLimit : ( v > upperLimit ? upperLimit : v ) );
	}

public:
	/** @param[in] _v initial value (will be coerced)
	* @param[in] _ll initial lower limit
	* @param[in] _ul initial upper limit
	* @param[in] _name name of the value */
	ScopeNumber(const T& _v = 0, const T& _ll = 0, const T& _ul = 0, const std::wstring& _name = L"None")
		: ScopeValueBase(_name) {
		SetWithLimits(_v, _ll, _ul);
	}

	/** Safe copy constructor */
	ScopeNumber(const ScopeNumber<T>& orig)
		: ScopeValueBase(orig.Name()) {
		SetWithLimits(orig.Value(), orig.ll(), orig.ul());
	}

	/** Safe assignment */
	ScopeNumber& operator=(const ScopeNumber<T>& v) {
		// avoid self assignment (leads to deadlock)
		if ( this != &v )
			SetWithLimits(v.Value(), v.ll(), v.ul());
		// assign name and rw state (after we have set the value)
		ScopeValueBase::operator=(v);
		return *this;
	}

	/** Safe assignment, does keep name and limits */
	ScopeNumber& operator=(const T& v) {
		Set(v);
		return *this;
	}

	/** @name Arithmetic operators
	* With this a ScopeNumber<T> can be treated like a T
	* @{ */
	ScopeNumber& operator+=(const ScopeNumber<T>& v) {
		Set(Value()+v.Value());
		return *this;
	}

	ScopeNumber& operator+=(const T& _val) {
		Set(Value()+_val);
		return *this;
	}

	ScopeNumber& operator-=(const ScopeNumber<T>& v) {
		Set(Value()-v.Value());
		return *this;
	}

	ScopeNumber& operator-=(const T& _val) {
		Set(Value()-_val);
		return *this;
	}

	ScopeNumber& operator*=(const ScopeNumber<T>& v) {
		Set(Value()*v.Value());
		return *this;
	}

	ScopeNumber& operator*=(const T& _val) {
		Set(Value()*_val);
		return *this;
	}

	ScopeNumber& operator/=(const ScopeNumber<T>& v) {
		Set(Value()/v.Value());
		return *this;
	}

	ScopeNumber& operator/=(const T& _val) {
		Set(Value()/_val);
		return *this;
	}

	ScopeNumber operator+(const ScopeNumber<T>& v) const { return ScopeNumber(*this) += v; }
	ScopeNumber operator-(const ScopeNumber<T>& v) const { return ScopeNumber(*this) -= v; }
	ScopeNumber operator*(const ScopeNumber<T>& v) const { return ScopeNumber(*this) *= v; }
	ScopeNumber operator/(const ScopeNumber<T>& v) const { return ScopeNumber(*this) /= v; }

	ScopeNumber operator+(const T& _val) const { return ScopeNumber(*this) += _val; }
	ScopeNumber operator-(const T& _val) const { return ScopeNumber(*this) -= _val; }
	ScopeNumber operator*(const T& _val) const { return ScopeNumber(*this) *= _val; }
	ScopeNumber operator/(const T& _val) const { return ScopeNumber(*this) /= _val; }
	/** @} */

	/** @name Comparison operators
	* With this a ScopeNumber<T> can be treated like a T
	* @{ */
	bool operator<(const ScopeNumber<T>& r) const {
		return ( Value() < r.Value() );
	}

	bool operator<(const T& r) const {
		return ( Value() < r );
	}

	bool operator>(const ScopeNumber<T>& r) const {
		return ( Value() > r.Value() );
	}

	bool operator>(const T& r) const {
		return ( Value() > r );
	}

	bool operator==(const ScopeNumber<T>& r) const {
		return ( Value() == r.Value() );
	}

	/*bool operator==(T r) {			// Bug? does not compile (see here: http://www.windows-api.com/microsoft/C/35711014/enum-operator-overload-ambiguity.aspx)
		return ( Value() == r );
	}*/

	bool operator!=(const ScopeNumber<T>& r) const {
		return !(operator==(r));
	}

	/* bool operator!=(const T& r) {
		return !(operator==(r));
	}*/
	/** @} */

	/** @return the lower limit */
	T ll() const {
		std::lock_guard<std::mutex> lock(mutex);
		return lowerLimit;
	}

	/** @return the upper limit */
	T ul() const {
		std::lock_guard<std::mutex> lock(mutex);
		return upperLimit;
	}

	/** Sets new value and new limits at the same time */
	T SetWithLimits(const T& v, const T& ll, const T& ul) {
		SetLimits(ll, ul);
		return Set(v);
	}

	/** Sets the limits and coerces the value accordingly
	* change signal is called if value has to be coerced
	* @param[in] ll new lower limit
	* @param[in] ul new upper limit
	* @param[in] _callguisignal call signal to GUI slots
	* @param[in] _callothersignal call signal to other slots
	* @param[in] _callatnochange call signals also if value did not change
	* @return the current value (possibly coerced to the new limits). */
	T SetLimits(const T& _ll, const T& _ul, const bool& _callguisignal = true, const bool& _callothersignal = true, const bool& _callatnochange = false) {
		T tmp(value);
		bool changed = false;
		if ( readwrite ) {
			std::lock_guard<std::mutex> lock(mutex);
			lowerLimit = ( ( _ll < _ul ) ? _ll : _ul );
			upperLimit = ( ( _ll < _ul ) ? _ul : _ll );
			value = CoerceValue(value);
			changed = (tmp != value);
			// keep the (now possibly changed) value for later outside the protected area
			tmp = value;
		}
		// Following code had to be copied from ScopeValue
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
};

}