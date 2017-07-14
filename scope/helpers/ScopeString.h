#pragma once

#include "ScopeValue.h"

namespace scope {

/** A templated class for a thread-safe std::wstring, with signals that are called on value changes. */
class ScopeString
	: public ScopeValue<std::wstring>
	, virtual public ScopeValueBase {

public:
	/** @param[in] _str initial value
	* @param[in] _name name of this string variable */
	ScopeString(const std::wstring& _str = L"", const std::wstring& _name = L"None")
		: ScopeValueBase(_name)
		, ScopeValue(_str, _name) {
	}

	/** Safe copy constructor */
	ScopeString(const ScopeString& origstr)
		: ScopeValueBase(origstr.Name())
		, ScopeValue(origstr.Value(), origstr.Name()) {
	}

	/** Use base classes assignment operators */
	using ScopeValue::operator=;

	/** @name Addition operator
	* For convenient string concatenation
	* @{ */
	ScopeString& operator+=(const ScopeString& r) {
		Set(Value()+r.Value());
		return *this;
	}

	ScopeString& operator+=(const std::wstring& r) {
		Set(Value()+r);
		return *this;
	}

	const ScopeString operator+(const ScopeString& r) const { return ScopeString(*this) += r; }
	const ScopeString operator+(const std::wstring& r) const { return ScopeString(*this) += r; }
	/** @} */

	/** @name Comparison operator
	* For convenient string comparisons
	*@{ */
	bool operator==(const ScopeString& r) const { return Value()==r.Value(); }
	bool operator!=(const ScopeString& r) const { return !((*this)==r); }
	bool operator==(const std::wstring& r) const { return Value()==r; }
	bool operator!=(const std::wstring& r) const { return !((*this)==r); }
	/** @} */
};



}