#pragma once
#include "config\config_choices.h"

/** @file ScopeDatatypes.h In here all declarations for all kinds of datatypes Scope needs.
* Remember: For a new datatype you have to implement specializations of ScopeValue member functions (see helpers/ScopeValue.cpp) ! */

namespace scope {
	namespace parameters {
		class ScannerVectorFrameBasic;
		class ScannerVectorFrameSaw;
		class ScannerVectorFrameBiDi;
		class ScannerVectorFramePlaneHopper;
		class ScannerVectorFrameResonance;
		class ScannerVectorFrameResonanceHopper;
		class ScannerVectorLine;
	}
}

namespace scope {

	/** Enum for the return value of a Controller.
	* none (only initial value), stopped (through user stop), finished (only in nframes mode), error, or interrupted (when thread was waiting at an interruption point)
	* since values should be combinable with or, numbers are assigned to the enum values */
	enum ControllerReturnStatus {
		none = 0x0,
		stopped = 0x01,
		finished = 0x02,
		error = 0x04
	};

	/** Enum for the possible movement direction of an FPU, left, right, up, down */
	enum FPUMoveDirection {
		left,
		right,
		up,
		down
	};


	/** Base class for all Scope datatypes here, provides a uniform interface (and saves typing...).
	* This is mostly done to use Boost property trees with ScopeNumber<ScannerVectorType> and to have a similar interface that allows cast to wstring etc. */
	template<class T>
	class ScopeDatatypeBase {
		public:
			/** the enum from the template class */
			typename T::Mode t;

		public:
			/** @return name of enumerators */
			static std::wstring NameOf(const typename T::Mode& _t) { return T::NameOf(_t); }

			/** @return name of int corresponding to enumerators */
			static std::wstring NameOf(const uint32_t& _n) { return T::NameOf(_n); }


			ScopeDatatypeBase()
				: t(T::Mode(0)) { }

			/** Construct from enumerators */
			ScopeDatatypeBase(const typename T::Mode& _t)
				: t(_t) { }

			/** Construct from string */
			ScopeDatatypeBase(const std::wstring& _str)
				: t(T::Mode(0)) {
				for ( uint32_t s = 0 ; s < T::S ; s++ ) {
					if ( _str == T::NameOf(s) ) {
						t = T::Mode(s);
						break;
					}
				}
			}

			/** @name Some operator definitions to make life easier
			* @{ */
			ScopeDatatypeBase& operator=(const typename T::Mode& _t) { t = _t; return *this; }
			bool operator==(const ScopeDatatypeBase& _sdb) const { return t == static_cast<typename T::Mode>(_sdb); }
			bool operator==(const typename T::Mode& _t) const { return t == _t; }
			bool operator!=(const ScopeDatatypeBase& _sdb) const { return !(operator==(_sdb)); }
			bool operator!=(const typename T::Mode& _t) const { return !(operator==(_t)); }
			bool operator<(const ScopeDatatypeBase& _sdb) const { return t < static_cast<typename T::Mode>(_sdb); }
			bool operator>(const ScopeDatatypeBase& _sdb) const { return t > static_cast<typename T::Mode>(_sdb); }
			operator typename T::Mode() const { return t; }
			operator uint32_t() const { return static_cast<uint32_t>(t); }
			operator std::wstring() const { return T::NameOf(t); }
			/** @} */
	};

	/** The type of z control device that is used for a stack. FastT could be ETL or piezo */
	class ZDeviceHelper {
	public:
		/** The enum */
		enum Mode {
			ZStage,
			FastZ
		};

		/** Number of enumerators */
		static const uint32_t S = 2;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"ZStage", L"FastZ" };
			return names[_n];
		}
	};

	/** The type of z control device that is used for a stack. FastT could be ETL or piezo */
	typedef ScopeDatatypeBase<ZDeviceHelper> ZDevice;


	/** How does the animal behavior trigger/gate acquisition? */
	class BehaviorModeHelper {
	public:
		/** The enum */
		enum Mode {
			Gated
		};

		/** Number of enumerators */
		static const uint32_t S = 1;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"Gated" };
			return names[_n];
		}
	};

	/** How does the animal behavior trigger/gate acquisition? */
	typedef ScopeDatatypeBase<BehaviorModeHelper> BehaviorMode;


	/** Class for the DAQmx timing, either by onboard clock, through a reference clock or by an external clock signal */
	class DaqTimingHelper {
	public:
		/** The enum */
		enum Mode  {
			OnboardClock,
			ReferenceClock,
			External
		};

		/** Number of enumerators */
		static const uint32_t S = 3;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"OnboardClock", L"ReferenceClock", L"External" };
			return names[_n];
		}
	};

	/** Class for the DAQmx timing, either by onboard clock, through a reference clock or by an external clock signal */
	typedef ScopeDatatypeBase<DaqTimingHelper> DaqTiming;


	/** Enum for the usable data input range. Usually uint16_t is read from the device, if the device's input range is
	* -1V .. +1V but the PMT's signal goes only in negative direction, only the first half of uint16_t is usable. Displaying the
	* whole uint16_t range in CChannelView or CHistogramView does not make sense then. */
	class Uint16RangeHelper {
	public:
		/** The enum */
		enum Mode {
			full,
			firsthalf,
			secondhalf,
			first4096
		};

		/** Number of enumerators */
		static const uint32_t S = 4;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"full", L"firsthalf", L"secondhalf", L"first4096" };
			return names[_n];
		}
	};

	/** Enum for the usable data input range. Usually uint16_t is read from the device, if the device's input range is
	* -1V .. +1V but the PMT's signal goes only in negative direction, only the first half of uint16_t is usable. Displaying the
	* whole uint16_t range in CChannelView or CHistogramView does not make sense then. */
	typedef ScopeDatatypeBase<Uint16RangeHelper> Uint16Range;


	/** Helpers to get uint16_t values for the lower and upper boundary of a Uint16Range
	* @{ */
	uint16_t Uint16LowerBoundary(const Uint16Range& r);
	uint16_t Uint16UpperBoundary(const Uint16Range& r);
	/** @} */

	/** Describes the type of signals output to scanners/fast z/Pockels */
	class OutputsTypeHelper {
	public:
		/** The different types of outputs */
		enum Mode {
			OutputsDAQmx,
			OutputsDAQmxLineClock,
			OutputsDAQmxSlave,
			OutputsDAQmxResonance
		};

		/** Number of enumerators */
		static const uint32_t S = 4;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"OutputsDAQmx", L"OutputsDAQmxLineClock", L"OutputsDAQmxSlave", L"OutputsDAQmxResonance" };
			return names[_n];
		}
	};

	/** Describes the type of signals output to scanners/fast z/Pockels */
	typedef ScopeDatatypeBase<OutputsTypeHelper> OutputsType;


	/** Describes the type of signal input from PMTs */
	class InputsTypeHelper {
	public:
		/** The different types of inputs */
		enum Mode {
			InputsDAQmx,
			InputsFPGA,
			InputsFPGAIO6587,
			InputsFPGADigitalDemultiplexer,
			InputsFPGAPhotonCounter,
			InputsFPGAIO5771,
			InputsFPGAIO5751,
			InputsFPGAAnalogDemultiplexer,
			InputsFPGAAnalogIntegrator,
			InputsFPGAResonanceScanner
		};

		/** Number of enumerators */
		static const uint32_t S = 10;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names =
				{ L"InputsDAQmx"
				, L"InputsFPGA"
				, L"InputsFPGAIO6587"
				, L"InputsFPGADigitalDemultiplexer"
				, L"InputsFPGAPhotonCounter"
				, L"InputsFPGAIO5771"
				, L"InputsFPGAIO5751"
				, L"InputsFPGAAnalogDemultiplexer"
				, L"InputsFPGAAnalogIntegrator"
				, L"InputsFPGAResonanceScanner" };
			return names[_n];
		}
	};

	/** Describes the type of signal input from PMTs */
	typedef ScopeDatatypeBase<InputsTypeHelper> InputsType;


	/** Describes the type of scanner */
	class ScannerTypeHelper {
	public:
		/** The different types of scanners */
		enum Mode {
			Regular,
			Resonance,
			AOD,
			Fiber
		};

		/** Number of enumerators */
		static const uint32_t S = 4;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names =
				{ L"Regular"
				, L"Resonance"
				, L"AOD"
				, L"Fiber" };
			return names[_n];
		}
	};

	/** Describes the type of scanner */
	typedef ScopeDatatypeBase<ScannerTypeHelper> ScannerType;

	/** Describes the scanner vector type */
	class ScannerVectorTypeHelper {
		public:
			/** The different types of scans */
			enum Mode {
				Basic,
				Sawtooth,
				Bidirectional,
				Planehopper,
				LineStraight,
				ResonanceBiDi,
				ResonanceHopper
			};

			/** Number of enumerators */
			static const uint32_t S = 7;

			/** @return name of enumerator */
			static std::wstring NameOf(const uint32_t& _n) {
				static std::array<std::wstring, S> names =
					{ L"Basic"
					, L"Sawtooth"
					, L"Bidirectional"
					, L"Planehopper"
					, L"LineStraight"
					, L"ResonanceBiDi"
					, L"ResonanceHopper" };
				return names[_n];
			}
	};

	template<ScannerVectorTypeHelper::Mode>
	class ScannerVectorTypeSelector {
	public:
		typedef parameters::ScannerVectorFrameBasic type;
	};

	template<>
	class ScannerVectorTypeSelector<ScannerVectorTypeHelper::Mode::Sawtooth> {
	public:
		typedef parameters::ScannerVectorFrameSaw type;
	};

	template<>
	class ScannerVectorTypeSelector<ScannerVectorTypeHelper::Mode::Bidirectional> {
	public:
		typedef parameters::ScannerVectorFrameBiDi type;
	};

	template<>
	class ScannerVectorTypeSelector<ScannerVectorTypeHelper::Mode::Planehopper > {
	public:
		typedef parameters::ScannerVectorFramePlaneHopper type;
	};

	template<>
	class ScannerVectorTypeSelector<ScannerVectorTypeHelper::Mode::ResonanceBiDi> {
	public:
		typedef parameters::ScannerVectorFrameResonance type;
	};

	template<>
	class ScannerVectorTypeSelector<ScannerVectorTypeHelper::Mode::ResonanceHopper> {
	public:
		typedef parameters::ScannerVectorFrameResonanceHopper type;
	};

	template<>
	class ScannerVectorTypeSelector<ScannerVectorTypeHelper::Mode::LineStraight> {
	public:
		typedef parameters::ScannerVectorLine type;
	};

	/** Describes the scanner vector type */
	typedef ScopeDatatypeBase<ScannerVectorTypeHelper> ScannerVectorType;


	/** Describes the scanner vector types that are supported for a given scanner type */
	class ScannerSupportedVectors {
		public:
			/** Returns a vector with all supported scannervectors for a given scannertype. Default scannertype is the one compiled for (see config_choices.h). */
			static std::vector<ScannerVectorTypeHelper::Mode> List(const config::ScannerEnum& _scannertype = config::scannerselect);

			/** Returns true if a given scannervector/scanmode is supported by a given scannertypeDefault scannertype is the one compiled for (see config_choices.h). */
			static bool IsSupported(const ScannerVectorTypeHelper::Mode& _scanmode, const config::ScannerEnum& _scannertype = config::scannerselect);

			/** Returns true if a given scannervector/scanmode is supported by the builtin/hardcoded (see config_choices.h) scannertype */
			static bool IsBuiltinSupported(const ScannerVectorTypeHelper::Mode& _scanmode);
	};


	/** Describes the scanner vector fill type for different parameters::Outputs types :
	* - FullframeXYZP for parameters::OutputsDAQmx in a master area
	* - LineXPColumnYZ for parameters::OutputsDAQmxLineClock in a master area
	* - LineZP for parameters::OutputsDAQmxSlave in a slave area */
	class ScannerVectorFillTypeHelper {
	public:
		/** The different types of scanner vector fill types.  */
		enum Mode {
			FullframeXYZP,
			LineXPColumnYZ,
			LineZP
		};

		/** Number of enumerators */
		static const uint32_t S = 3;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"FullframeXYZP", L"LineXPColumnYZ", L"LineZP" };
			return names[_n];
		}
	};

	/** Describes the scanner vector fill type for different parameters::Outputs types :
	* - FullframeXYZP for parameters::OutputsDAQmx in a master area
	* - LineXPColumnYZ for parameters::OutputsDAQmxLineClock in a master area
	* - LineZP for parameters::OutputsDAQmxSlave in a slave area */
	typedef ScopeDatatypeBase<ScannerVectorFillTypeHelper> ScannerVectorFillType;


	/** Describes the actual state of the scope */
	class RunStateHelper {
	public:
		/** Enum for the run state of the scope, stopped, continuously scanning, single scan, stack scan, timeseries scan, or behavior triggered scan */
		enum Mode {
			Stopped,
			RunningContinuous,
			RunningSingle,
			RunningStack,
			RunningTimeseries,
			RunningBehavior
		};

		/** Number of enumerators */
		static const uint32_t S = 6;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"Stopped", L"RunningContinuous", L"RunningSingle", L"RunningStack", L"RunningTimeseries", L"RunningBehavior" };
			return names[_n];
		}
	};

	/** Describes the actual state of the scope */
	typedef ScopeDatatypeBase<RunStateHelper> RunState;


	/** Describes the data acquisiton mode */
	class DaqModeHelper {
	public:
		/** Enum for the mode of scanning/acquisition, either continuous or a certain number of frames */
		enum Mode {
			continuous,
			nframes
		};

		/** Number of enumerators */
		static const uint32_t S = 2;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"continuous", L"nframes" };
			return names[_n];
		}
	};

	/** Describes the data acquisiton mode */
	typedef ScopeDatatypeBase<DaqModeHelper> DaqMode;

	/** Sampling method (depending on card etc), either simultaneous (clock independent of channel number)
	* or non-simultaneous samplingrate is maxrateaggregate/nochannels */
	class DaqSamplingHelper {
	public:
		/** Enum for the acquisition sampling type (either simultaneous or not) */
		enum Mode {
			SimultaneousSampling,
			NonSimultaneousSampling
		};

		/** Number of enumerators */
		static const uint32_t S = 2;

		/** @return name of enumerator */
		static std::wstring NameOf(const uint32_t& _n) {
			static std::array<std::wstring, S> names = { L"SimultaneousSampling", L"NonSimultaneousSampling" };
			return names[_n];
		}
	};

	/** Sampling method (depending on card etc), either simultaneous (clock independent of channel number)
	* or non-simultaneous samplingrate is maxrateaggregate/nochannels */
	typedef ScopeDatatypeBase<DaqSamplingHelper> DaqSampling;


}