#include "StdAfx.h"
#include "ScopeValue.h"

namespace scope {

/** @name Specializations
* @{ */
/** Specialization for bool */
template<>
void ScopeValue<bool>::SetFromString(const std::wstring& str) {
	Set( (str == L"true") );
}

/** Specialization for Uint16Range */
template<>
void ScopeValue<Uint16Range>::SetFromString(const std::wstring& str) {
	Set(Uint16Range(str));
}

/** Specialization for AreaType */
template<>
void ScopeValue<AreaType>::SetFromString(const std::wstring& str) {
	Set(AreaType(str));
}

/** Specialization for ScannerType */
template<>
void ScopeValue<ScannerType>::SetFromString(const std::wstring& str) {
	Set(ScannerType(str));
}

/** Specialization for ScannerVectorType */
template<>
void ScopeValue<ScannerVectorType>::SetFromString(const std::wstring& str) {
	Set(ScannerVectorType(str));
}

/** Specialization for RunState */
template<>
void ScopeValue<RunState>::SetFromString(const std::wstring& str) {
	Set(RunState(str));
}

/** Specialization for ZDevice */
template<>
void ScopeValue<ZDevice>::SetFromString(const std::wstring& str) {
	Set(ZDevice(str));
}

/** Specialization for BehaviorMode */
template<>
void ScopeValue<BehaviorMode>::SetFromString(const std::wstring& str) {
	Set(BehaviorMode(str));
}

/** Specialization for DaqMode */
template<>
void ScopeValue<DaqMode>::SetFromString(const std::wstring& str) {
	Set(DaqMode(str));
}

/** Specialization for DaqTiming */
template<>
void ScopeValue<DaqTiming>::SetFromString(const std::wstring& str) {
	Set(DaqTiming(str));
}

/** Specialization for DaqSampling */
template<>
void ScopeValue<DaqSampling>::SetFromString(const std::wstring& str) {
	Set(DaqSampling(str));
}

/** Specialization for double */
template<>
void ScopeValue<double>::SetFromString(const std::wstring& str) {
	double tmp = 0.0;
	Set( !(std::wstringstream(str) >> tmp) ? 0.0 : tmp );			// operator! returns failbit, if failbit==false return tmp
}

/** Specialization for uint8_t */
template<>
void ScopeValue<uint8_t>::SetFromString(const std::wstring& str) {
	uint16_t tmp = 0;		// Workaround, using uint8_t (which is unsigned char!) gives problems
	Set( !(std::wstringstream(str) >> tmp) ? (uint8_t)0 : (uint8_t)tmp );				// operator! returns failbit, if failbit==false return tmp
}

/** Specialization for uint16_t */
template<>
void ScopeValue<uint16_t>::SetFromString(const std::wstring& str) {
	uint16_t tmp = 0;
	Set( !(std::wstringstream(str) >> tmp) ? 0 : tmp );				// operator! returns failbit, if failbit==false return tmp
}

/** Specialization for int32_t */
template<>
void ScopeValue<int32_t>::SetFromString(const std::wstring& str) {
	int32_t tmp = 0;
	Set( !(std::wstringstream(str) >> tmp) ? 0 : tmp );				// operator! returns failbit, if failbit==false return tmp
}

/** Specialization for uint32_t */
template<>
void ScopeValue<uint32_t>::SetFromString(const std::wstring& str) {
	uint32_t tmp = 0;
	Set( !(std::wstringstream(str) >> tmp) ? 0 : tmp );				// operator! returns failbit, if failbit==false return tmp
}

/** Specialization for uint64_t */
template<>
void ScopeValue<uint64_t>::SetFromString(const std::wstring& str) {
	uint64_t tmp = 0;
	Set( !(std::wstringstream(str) >> tmp) ? 0 : tmp );				// operator! returns failbit, if failbit==false return tmp
}

/** Specialization for std::wstring */
template<>
void ScopeValue<std::wstring>::SetFromString(const std::wstring& str) {
	Set(str);
}



/** Specialization for bool */
template<>
std::wstring ScopeValue<bool>::ToChar() const {
	return (Value())?L"true":L"false";
}

/** Specialization for Uint16Range */
template<>
std::wstring ScopeValue<Uint16Range>::ToChar() const {
	return Value().operator std::wstring();
}

/** Specialization for AreaType */
template<>
std::wstring ScopeValue<AreaType>::ToChar() const {
	return Value().operator std::wstring();
}

/** Specialization for ScannerType */
template<>
std::wstring ScopeValue<ScannerType>::ToChar() const {
	return Value().operator std::wstring();
}

/** Specialization for ScannerVectorType */
template<>
std::wstring ScopeValue<ScannerVectorType>::ToChar() const {
	return Value().operator std::wstring();
}

/** Specialization for RunState */
template<>
std::wstring ScopeValue<RunState>::ToChar() const {
	return Value().operator std::wstring();
}

/** Specialization for ZDevice*/
template<>
std::wstring ScopeValue<ZDevice>::ToChar() const {
	return Value().operator std::wstring();	
}

/** Specialization for BehaviorMode */
template<>
std::wstring ScopeValue<BehaviorMode>::ToChar() const {
	return Value().operator std::wstring();	
}

/** Specialization for DaqMode */
template<>
std::wstring ScopeValue<DaqMode>::ToChar() const {
	return Value().operator std::wstring();	
}

/** Specialization for DaqSampling */
template<>
std::wstring ScopeValue<DaqSampling>::ToChar() const {
	return Value().operator std::wstring();
}

/** Specialization for DaqTiming */
template<>
std::wstring ScopeValue<DaqTiming>::ToChar() const {
	return Value().operator std::wstring();
}

/** Specialization for double */
template<>
std::wstring ScopeValue<double>::ToChar() const {
	std::wstringstream stream;
	stream << Value();
	return stream.str();
}

/** Specialization for uint8_t */
template<>
std::wstring ScopeValue<uint8_t>::ToChar() const {
	std::wstringstream stream;
	stream << Value();
	return stream.str();
}

/** Specialization for uint16_t */
template<>
std::wstring ScopeValue<uint16_t>::ToChar() const {
	std::wstringstream stream;
	stream << Value();
	return stream.str();
}

/** Specialization for int32_t */
template<>
std::wstring ScopeValue<int32_t>::ToChar() const {
	std::wstringstream stream;
	stream << Value();
	return stream.str();
}

/** Specialization for uint32_t */
template<>
std::wstring ScopeValue<uint32_t>::ToChar() const {
	std::wstringstream stream;
	stream << Value();
	return stream.str();
}

/** Specialization for uint64_t */
template<>
std::wstring ScopeValue<uint64_t>::ToChar() const {
	std::wstringstream stream;
	stream << Value();
	return stream.str();
}

/** Specialization for std::wstring */
template<>
std::wstring ScopeValue<std::wstring>::ToChar() const {
	return Value();
}

/** @} */


}

