#pragma once

#include <atlexcept.h>

/** Exception class for HRESULTs */
class hresult_exception
	: public std::exception {

public:
	/** Current HRESULT */
	HRESULT hr;

	/** Function where exception occured */
	char function[255];

	/** @param[in] _hr current HRESULT
	* @param[in] _function function where exception occured */
	hresult_exception(const HRESULT& _hr = S_OK, const char _function[] = "none")
		: hr(_hr) {
		strcpy_s(function, 255, _function);
	}

	/** @return "HRESULT exception" */
	const char *what() const {
		return "HRESULT exception";
	}
};

/** Templated helper class to check an HRESULT and throw an exception (or not throw) */
template<bool ThrowException>
struct HResultChecker {
	/** Check */
	static bool Check(HRESULT _hr);
};

/** Helper class definition to check an HRESULT and not throw an exception */
template<> struct HResultChecker<false> {
	/** @return true if HRESULT is error */
	static bool Check(const HRESULT& _hr) {
		_hr;
		return true;
	}
};

/** Helper class definition to check an HRESULT and throw an exception (currently disabled!) */
template<> struct HResultChecker<true> {
	/** @return true if HRESULT is error */
	static bool Check(const HRESULT& _hr) {
		if(FAILED(_hr)) {
			throw std::exception("HResult exception");
			return false;
		}
		return true;
	}
};

/** Class around a HRESULT which breaks down the HRESULT code and makes life easier etc */
template<bool ThrowException>
class HResultT {

protected:
	/** HRESULT */
	HRESULT hr;

	/** function where exception occured */
	char function[255];

	/** more description */
	std::basic_string<WCHAR> desc;

protected:
	/** Assign a HRESULT */
	void Assign(const HRESULT& _hr) {
		//WORD code = HRESULT_CODE(_hr);
		//WORD facility = HRESULT_FACILITY(_hr);
		//DWORD severity = HRESULT_SEVERITY(_hr);
		if (!HResultChecker<ThrowException>::Check(hr = _hr))
			throw hresult_exception(hr, function);
	}

public:
	/** Constructor */
	HResultT(const HRESULT& _hr = S_OK, const char _function[] = "none")
		: hr(_hr) {
		strcpy_s(function, 255, _function);
		Assign(hr);
	}

	/** @name some helpfull operator overloads
	* @{ */
	HResultT& operator() (const char _function[] = "none") {
		strcpy_s(function, 255, _function);
		return *this;
	}

	HResultT& operator() (const HRESULT& _hr, const char _function[] = "none") {
		strcpy_s(function, 255, _function);
		Assign(_hr);
		return *this;
	}

	HResultT& operator= (const HRESULT& _hr) {
		Assign(_hr);
		return *this;
	}
	/** @} */

	/** @return "Error" */
	LPCTSTR ErrrorMessage() const {
		return L"Error";
	}

	/** @return the HRESULT */
	operator HRESULT () const {
		return hr;
	}
};

typedef HResultT<true> HResult;

typedef HResultT<false> HResultSafe;
