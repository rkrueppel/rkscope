#pragma once

/** @file helpers.h Various helper functions and classes for Scope. */

/** @defgroup HELPERS General helper function and classes
* Contains various classes and functions
* @{ */
#ifdef max						// If this was defined, e.g. in windef.h without NOMINMAX undefine it!
#define max
#endif
#ifdef min
#define min
#endif

/** @return +1 for positive T, -1 for negative T */
template<class T> T Signum(const T& _v) { return (std::abs(_v) == _v)?1:0; }

/** Extracts low dword (32 bits) from an ULONG64 (64 bits) */
#define LODWORD32(l)           ((DWORD32)(((ULONG64)(l)) & 0xffffffff))
/** Extracts high dword (32 bits) from an ULONG64 (64 bits) */
#define HIDWORD32(l)           ((DWORD32)((((ULONG64)(l)) >> 32) & 0xffffffff))

/** A safe release for COM objects */
template<class Interface>
void SafeRelease(Interface **ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != nullptr) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = nullptr;
	}
}

/** @name Rounding functions
* For convenience, round double to whatever
* @{ */
inline double round(const double& v) { return floor(v+0.5); }
inline uint64_t round2ui64(const double& v) { return static_cast<uint64_t>(floor(v+0.5)); }
inline uint32_t round2ui32(const double& v) { return static_cast<uint32_t>(floor(v+0.5)); }
inline uint16_t round2ui16(const double& v) { return static_cast<uint16_t>(floor(v+0.5)); }
inline uint8_t round2ui8(const double& v) { return static_cast<uint8_t>(floor(v+0.5)); }
inline int64_t round2i64(const double& v) { return static_cast<int64_t>(floor(v+0.5)); }
inline int32_t round2i32(const double& v) { return static_cast<int32_t>(floor(v+0.5)); }
inline int16_t round2i16(const double& v) { return static_cast<int16_t>(floor(v+0.5)); }
inline int8_t round2i8(const double& v) { return static_cast<int8_t>(floor(v+0.5)); }

template<class T>
T round2(const double& v) { return static_cast<T>(floor(v+0.5)); }
/** @} */

/** @name Date/Time string functions
* Provide nice formatting of date or time
* @{ */

/** @return string with the current date in format YYYY-MM-DD */
std::wstring GetCurrentDateString();

/** @param[in] _filenamecompatible true for compatiblity with file name, returns . instead of : (whichis  required by ISO standard)
* @return string with current time in format HH:MM:SS */
std::wstring GetCurrentTimeString(const bool& _filenamecompatible = true);
/** @} */

/** Enum for area types, usefull in cases like std::array<whatever, 2> with 0 for masters and 1 for slaves. */
enum AreaType {
	ATMaster = 0,
	ATSlave = 1
};

constexpr static uint32_t NAreaTypes = 2;

std::array<std::wstring, NAreaTypes> AreaTypeString{ L"Master", L"Slave" };

/** Tag for ScopeMessage */
enum class ScopeMessageTag { nothing, abort };

/** Message with tag and cargo for SyncQueues between controllers */
template<class T>
class ScopeMessage {
public:
	/** the tag of the message, at the moment only nothing or abort */
	ScopeMessageTag tag;

	/** the cargo */
	T cargo;

	ScopeMessage()
		: tag(ScopeMessageTag::nothing)
		, cargo() {
	}

	/** Construct. Attention: Copies the _cargo! Use e.g. a shared_ptr. */
	ScopeMessage(const ScopeMessageTag& _tag, const T& _cargo)
		: tag(_tag)
		, cargo(_cargo) {
	}
};

/** Thread-safe lock-free bool to signal a requested stop to the worker function currently executed in the Active's thread */
class StopCondition {

private:
	/** the stop signal */
	mutable std::atomic_bool stop;

private:
	/** disable copy */
	StopCondition(StopCondition& ac);

	/** disable assignment */
	StopCondition& operator=(StopCondition&  ac);

public:
	/** Initialize as false */
	StopCondition(const bool& _init = false) { stop.store(_init); }

	/** @return status of abort signal */
	bool IsSet() const { return stop; }

	/** @param[in] _a new value of abort signal */
	void Set(const bool& _a = true) { stop.store(_a); }
};

/** Class for scaling to the full range of a datatype. E.g. to scale -1.0 ... +1.0 to 0..65535 use Scaler<uint16_t> test(-1.0, 1.0). */
template<class T>
struct Scaler {
	/** @param[in] _minin,_maxin minimum and maximum of range to scale from */
	Scaler(const double& _minin, const double& _maxin)	
		: minin(_minin)
		, maxin(_maxin)
		, maxout(static_cast<double>(std::numeric_limits<T>::max()))
		, minout(static_cast<double>(std::numeric_limits<T>::min()))
		, rangerelation( static_cast<double>(std::numeric_limits<T>::max()-std::numeric_limits<T>::min()) / (_maxin-_minin) ) {
		assert(std::numeric_limits<T>::is_specialized);
		assert(std::numeric_limits<T>::is_integer);
	}

	/** @return scaled version of v (from maxin-minin to full scale of type T) */
	T operator() (const double& v) const { return round2<T>((v-minin)*rangerelation+minout); }

	/** minimum of range */
	const double minin;

	/** maximum of range */
	const double maxin;

	/** maximum value of type T */
	const double maxout;

	/** minimum value of type T */
	const double minout;

	/** scaler between (maxin-minin) and (maxout - minout) */
	const double rangerelation;
};

/** A debug output to the debug console */
#ifdef _DEBUG
#define DBOUT(s) \
{ \
std::wostringstream os_; \
os_ << s << std::endl; \
ATLTRACE2( os_.str().c_str() ); \
}
#else
#define DBOUT(s)
#endif

/** @} */