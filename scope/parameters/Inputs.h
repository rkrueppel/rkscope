#pragma once

#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"

namespace scope {

/** all parameters live in this namespace */
namespace parameters {

using boost::property_tree::wptree;

/** Parameters for pixel acquisition, base class */
class Inputs
	: public Base {

public:
	Inputs();

	/** Clone method to clone into a pointer. Has to be overriden in derived classes. There it can use the compiler generated copy constructor with e.g. InputsDAQmx(*this). */
	virtual std::unique_ptr<Inputs> Clone() const = 0;

	/** Factory method to generate parameter sets for different scan types and put them into a ScannerVectorFrameBasic pointer. */
	//static std::unique_ptr<Inputs> Factory(const config::InputEnum& _type);

	/** number of channels to acquire */
	ScopeNumber<uint32_t> channels;

	/** if yes acquisition with maximum rate allowed by device (1/MinimumPixelTime(), calculated from maxrateaggregate and samplingtype), output with pixel rate. Does not make sense with daq_timing external!! */
	ScopeNumber<bool> oversampling;

	/** which part of the uint16_t range is used for valid input signals */
	ScopeValue<Uint16Range> rangetype;

	/** number of lines to acquire before each frame, e.g. to wait for the next sync from a resonance scanner (set to 0 usually, use 2 for a resonance scanner) */
	ScopeNumber<uint32_t> preframelines;

	/** @return the minimum pixel dwell time (in microseconds), depending on max aggregate rate, number of channels, and sampling type */
	virtual double MinimumPixeltime() const { return 1.0; }

	/** @return the maximum pixel dwell time (in microseconds) */
	virtual double MaximumPixeltime() const { return 15.0; }

	/** @return the _pixeltime coerced to the nearest value the device supports. */
	virtual double CoercedPixeltime(const double& _pixeltime) const { return _pixeltime; }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;

};

/** Parameters for pixel acquisition with NI-DAQmx */
class InputsDAQmx
	: public Inputs {

public:
	InputsDAQmx();

	/** Create method for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsDAQmx()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsDAQmx(*this)); }

	/** How many input channels (if sampling non-simulataneous this influences the available sampling rates) */
	ScopeString channelsstring;
	
	/** Input range of the ADC. NI-6259 analog in supports +-10, +-5, +-2, +-1, +-0.5, +-0.2, +-0.1, choose e.g. 1 for +-1V range. For a Hamatsu H7422, max 2µA, 150kOhm preamp, means max 0.3V choose 0.5 (unfortunately you loose half of the dynamic range), */
	ScopeNumber<double> range;

	/** where to get the sampling clock from? Onboard, from external reference clock, or from external source */
	ScopeValue<DaqTiming> daq_timing;
	
	/** source terminal of the reference clock */
	ScopeString referenceclocksource;
	
	/** rate in Hertz of the reference clock. Make sure this is correct even for using the onboard clock! Calculation of allowed pixel
	* times depends on this! */
	ScopeNumber<double> referenceclockrate;
	
	/** source terminal of an external clock */
	ScopeString externalclocksource;
	
	/** maximum aggregate sampling rate (divide this by number of channels to get per channel rate for non-simultaneous sampling) */
	ScopeNumber<double> maxrateaggregate;
	
	/** simultaneous sampling or not */
	ScopeValue<DaqSampling> sampling;

	/** Some diagnosis parameters, more useful for FPGAs ... */
	struct Diagnosis {
		/** %DAQmx library version number */
		ScopeString daqmxversion;

		Diagnosis()
			: daqmxversion(L"", L"DAQmxVersion") {
		}
	};
	
	/** Some diagnosis parameters, more useful for FPGAs ... */
	Diagnosis diagnosis;

	double MinimumPixeltime() const override;

	double CoercedPixeltime(const double& _pixeltime) const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition with NI-FPGA */
class InputsFPGA
	: public Inputs {
public:
	InputsFPGA();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGA()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGA(*this)); }

	double MinimumPixeltime() const override;

	double CoercedPixeltime(const double& _pixeltime) const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for the noise generating FPGA vi */
class InputsFPGANoiseOutput
	: public InputsFPGA {
public:
	InputsFPGANoiseOutput() { }

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGANoiseOutput()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGANoiseOutput(*this)); }

	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	struct Diagnosis {
		/** @name the FIFO indicators
		* @{ */
		ScopeNumber<bool> ToHostOverflowA1Ch1;
		ScopeNumber<bool> ToHostOverflowA1Ch2;
		ScopeNumber<bool> ToHostOverflowA2Ch1;
		ScopeNumber<bool> ToHostOverflowA2Ch2;
		/** @} */
		/** and the Acquiring indicator */
		ScopeNumber<bool> Acquiring;

		Diagnosis()
			: ToHostOverflowA1Ch1(false, false, true, L"ToHostOverflowA1Ch1")
			, ToHostOverflowA1Ch2(false, false, true, L"ToHostOverflowA1Ch2")
			, ToHostOverflowA2Ch1(false, false, true, L"ToHostOverflowA1Ch1")
			, ToHostOverflowA2Ch2(false, false, true, L"ToHostOverflowA1Ch2")
			, Acquiring(false, false, true, L"Acquiring") {
		}
	};
	
	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	Diagnosis diagnosis;
};

/** Parameters for pixel acquisition using the NI FlexRIO 6587 IO adapter module (high-speed LVDS digital input card) */
class InputsFPGAIO6587
	: public InputsFPGA {
public:
	InputsFPGAIO6587();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGAIO6587()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGAIO6587(*this)); }

	/** Sampling rate of the IO modules clock (which is double the clock rate of the FPGA acquisition loop) in Hz. Usually 1-1.4E9 Hz. */
	ScopeNumber<double> samplingrate;

	double MinimumPixeltime() const override;

	double CoercedPixeltime(const double& _pixeltime) const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition with FPGADigitalDemultiplexer */
class InputsFPGADigitalDemultiplexer
	: public InputsFPGAIO6587 {
public:
	InputsFPGADigitalDemultiplexer();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGADigitalDemultiplexer()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGADigitalDemultiplexer(*this)); }

	/** Type of photon counting. Count high samples (true) or pulses/flanks (false) */
	ScopeNumber<bool> countmode;

	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	struct Diagnosis {
		/** @name the FIFO indicators
		* @{ */
		ScopeNumber<bool> ToHostOverflowA1Ch1;
		ScopeNumber<bool> ToHostOverflowA1Ch2;
		ScopeNumber<bool> ToHostOverflowA2Ch1;
		ScopeNumber<bool> ToHostOverflowA2Ch2;
		ScopeNumber<bool> InterloopOverflow;
		ScopeNumber<bool> InterloopTimeout;
		/** @} */
		/** and the Acquiring indicator */
		ScopeNumber<bool> Acquiring;
		/** Measured laser frequency, calculated from counted laserpulses per pixel */
		ScopeNumber<double> MeasuredLaserFreq;

		Diagnosis()
			: ToHostOverflowA1Ch1(false, false, true, L"ToHostOverflowA1Ch1")
			, ToHostOverflowA1Ch2(false, false, true, L"ToHostOverflowA1Ch2")
			, ToHostOverflowA2Ch1(false, false, true, L"ToHostOverflowA2Ch1")
			, ToHostOverflowA2Ch2(false, false, true, L"ToHostOverflowA2Ch2")
			, InterloopOverflow(false, false, true, L"InterloopOverflow")
			, InterloopTimeout(false, false, true, L"InterloopTimeout")
			, Acquiring(false, false, true, L"Acquiring")
			, MeasuredLaserFreq(0.0, 0.0, 1E10, L"MeasuredLaserFrequency_Hz") {
		}
	};
	
	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	Diagnosis diagnosis;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition with FPGAPhotonCounter */
class InputsFPGAPhotonCounter
	: public InputsFPGAIO6587 {
public:
	InputsFPGAPhotonCounter();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGAPhotonCounter()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGAPhotonCounter(*this)); }

	/** Type of photon counting. Count high samples (true) or pulses/flanks (false) */
	ScopeNumber<bool> countmode;

	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	struct Diagnosis {
		/** @name the FIFO indicators
		* @{ */
		ScopeNumber<bool> ToHostOverflowCh1;
		ScopeNumber<bool> ToHostOverflowCh2;
		ScopeNumber<bool> InterloopOverflow;
		ScopeNumber<bool> InterloopTimeout;
		/** @} */
		/** and the Acquiring indicator */
		ScopeNumber<bool> Acquiring;

		/** Switch for counting mode. true: count high samples, false: count pulses/flanks */
		ScopeNumber<bool> CountingMode;

		Diagnosis()
			: ToHostOverflowCh1(false, false, true, L"ToHostOverflowCh1")
			, ToHostOverflowCh2(false, false, true, L"ToHostOverflowCh2")
			, InterloopOverflow(true, false, true, L"InterloopOverflow")
			, InterloopTimeout(false, false, true, L"InterloopTimeout")
			, Acquiring(false, false, true, L"Acquiring")
			, CountingMode(true, false, true, L"CountingMode") {
		}
	};
	
	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	Diagnosis diagnosis;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition using the NI FlexRIO 5771 IO adapter module (high-speed 8-bit digitizer) */
class InputsFPGAIO5771
	: public InputsFPGA {
public:
	InputsFPGAIO5771();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGAIO5771()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGAIO5771(*this)); }

	/** Baseline U8 value to set zero in channel 1 (NI5771 reads ~ -1..+1V as unsigned 8 bit number, thus 0V is ~127) */
	ScopeNumber<uint8_t> BaselineCh1;

	/** Baseline U8 value to set zero in channel 2 (NI5771 reads ~ -1..+1V as unsigned 8 bit number, thus 0V is ~127) */
	ScopeNumber<uint8_t> BaselineCh2;

	/** Cutoff U8 value to set zero in channel 1 (NI5771 reads ~ -1..+1V as unsigned 8 bit number, thus 0V is ~127) */
	ScopeNumber<uint8_t> CutoffCh1;

	/** Cutoff U8 value to set zero in channel 2 (NI5771 reads ~ -1..+1V as unsigned 8 bit number, thus 0V is ~127) */
	ScopeNumber<uint8_t> CutoffCh2;

	double MinimumPixeltime() const override;

	double CoercedPixeltime(const double& _pixeltime) const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition using the NI FlexRIO 5751 IO adapter module (14-bit digitizer) */
class InputsFPGAIO5751
	: public InputsFPGA {
public:
	InputsFPGAIO5751();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGAIO5751()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGAIO5751(*this)); }

	/** Baseline U16 value to set zero in channel 1 */
	ScopeNumber<uint32_t> BaselineCh1;

	/** Baseline U16 value to set zero in channel 2 */
	ScopeNumber<uint32_t> BaselineCh2;

	/** Frequency in Hz of the clock used in the FPGA VI for the acquisition */
	ScopeNumber<double> AcquisitionClockRate;

	double MinimumPixeltime() const override;
	double MaximumPixeltime() const override;

	double CoercedPixeltime(const double& _pixeltime) const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition FPGAAnalogDemultiplexer */
class InputsFPGAAnalogDemultiplexer
	: public InputsFPGAIO5771 {
public:
	InputsFPGAAnalogDemultiplexer();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGAAnalogDemultiplexer()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGAAnalogDemultiplexer(*this)); }

	/** Number of bits to shift U32 right before casting to U16 (area 1 channel 1) */
	ScopeNumber<uint8_t> BitshiftA1Ch1;

	/** Number of bits to shift U32 right before casting to U16 (area 1 channel 2) */
	ScopeNumber<uint8_t> BitshiftA1Ch2;

	/** Number of bits to shift U32 right before casting to U16 (area 2 channel 1) */
	ScopeNumber<uint8_t> BitshiftA2Ch1;

	/** Number of bits to shift U32 right before casting to U16 (area 2 channel 2) */
	ScopeNumber<uint8_t> BitshiftA2Ch2;

	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	struct Diagnosis {
		/** @name the FIFO indicators
		* @{ */
		ScopeNumber<bool> ToHostOverflowA1;
		ScopeNumber<bool> ToHostOverflowA2;
		ScopeNumber<bool> InterloopOverflow;
		ScopeNumber<bool> InterloopTimeout;
		/** @} */
		/** and the Acquiring indicator */
		ScopeNumber<bool> Acquiring;

		/** indicator if input is over the ADC range */
		ScopeNumber<bool> AIOverRange;

		Diagnosis()
			: ToHostOverflowA1(false, false, true, L"ToHostOverflowA1")
			, ToHostOverflowA2(false, false, true, L"ToHostOverflowA2")
			, InterloopOverflow(false, false, true, L"InterloopOverflow")
			, InterloopTimeout(false, false, true, L"InterloopTimeout")
			, Acquiring(false, false, true, L"Acquiring")
			, AIOverRange(false, false, true, L"AIOverRange") {
		}
	};
	
	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	Diagnosis diagnosis;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition FPGAAnalogIntegrator */
class InputsFPGAAnalogIntegrator
	: public InputsFPGAIO5771 {
public:
	InputsFPGAAnalogIntegrator();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGAAnalogIntegrator()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGAAnalogIntegrator(*this)); }

	/** Number of bits to shift U32 right before casting to U16 (channel 1) */
	ScopeNumber<uint8_t> BitshiftCh1;

	/** Number of bits to shift U32 right before casting to U16 (channel 2) */
	ScopeNumber<uint8_t> BitshiftCh2;

	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	struct Diagnosis {
		/** @name the FIFO indicators
		* @{ */
		ScopeNumber<bool> ToHostOverflowCh1;
		ScopeNumber<bool> ToHostOverflowCh2;
		ScopeNumber<bool> InterloopOverflow;
		ScopeNumber<bool> InterloopTimeout;
		/** @} */
		/** and the Acquiring indicator */
		ScopeNumber<bool> Acquiring;

		/** indicator if input is over the ADC range */
		ScopeNumber<bool> AIOverRange;

		Diagnosis()
			: ToHostOverflowCh1(false, false, true, L"ToHostOverflowCh1")
			, ToHostOverflowCh2(false, false, true, L"ToHostOverflowCh2")
			, InterloopOverflow(false, false, true, L"InterloopOverflow")
			, InterloopTimeout(false, false, true, L"InterloopTimeout")
			, Acquiring(false, false, true, L"Acquiring")
			, AIOverRange(false, false, true, L"AIOverRange") {
		}
	};
	
	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	Diagnosis diagnosis;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for pixel acquisition FPGAResonanceScanner */
class InputsFPGAResonanceScanner
	: public InputsFPGAIO5751 {
public:
	InputsFPGAResonanceScanner();

	/** Create function for factory */
	static std::unique_ptr<Inputs> Create() { return std::unique_ptr<Inputs>(new InputsFPGAResonanceScanner()); }

	std::unique_ptr<Inputs> Clone() const override { return std::unique_ptr<Inputs>(new InputsFPGAResonanceScanner(*this)); }

	/** Number of bits to shift U32 right before casting to U16 (channel 1) */
	ScopeNumber<uint8_t> BitshiftCh1;

	/** Number of bits to shift U32 right before casting to U16 (channel 2) */
	ScopeNumber<uint8_t> BitshiftCh2;

	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	struct Diagnosis {
		/** @name the FIFO indicators
		* @{ */
		ScopeNumber<bool> ToHostOverflowCh1;
		ScopeNumber<bool> ToHostOverflowCh2;
		ScopeNumber<bool> InterloopOverflow;
		ScopeNumber<bool> InterloopTimeout;
		/** @} */
		/** and the Acquiring indicator */
		ScopeNumber<bool> Acquiring;

		/** indicator if input is over the ADC range */
		ScopeNumber<bool> AIOverRange;

		Diagnosis()
			: ToHostOverflowCh1(false, false, true, L"ToHostOverflowCh1")
			, ToHostOverflowCh2(false, false, true, L"ToHostOverflowCh2")
			, InterloopOverflow(false, false, true, L"InterloopOverflow")
			, InterloopTimeout(false, false, true, L"InterloopTimeout")
			, Acquiring(false, false, true, L"Acquiring")
			, AIOverRange(false, false, true, L"AIOverRange") {
		}
	};
	
	/** Keeps the LEDs/ScopeNumber for FPGA diagnosis together. These can be connected to ScopeLEDs */
	Diagnosis diagnosis;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

}

}
