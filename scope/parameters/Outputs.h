#pragma once

#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"

namespace scope {

/** all parameters live in this namespace */
namespace parameters {

using boost::property_tree::wptree;

/** Parameters for scanner data generation, base class */
class Outputs
	: public Base {

public:
	Outputs();

	
	/** Abstract clone method to clone into a pointer. Has to be overriden in derived classes.
	* There it can use the compiler generated copy constructor with e.g. OutputsDAQmx(*this). */
	virtual std::unique_ptr<Outputs> Clone() const = 0;

	/** Factory method to generate parameter sets for different scan types and put them into a ScannerVectorFrameBasic pointer. */
	static std::unique_ptr<Outputs> Factory(const OutputsType& _type);

	/** Output range of the DAC to use for all outputs. E.g. NI-6259 supports only +-10 or +-5V output range (or external reference). Choose e.g. 5 for +-5V range. */
	ScopeNumber<double> range;

	/** minimum output voltage for scaling of the scanner signal */
	ScopeNumber<double> minoutputscanner;
	
	/** maximum output voltage for scaling of the scanner signal */
	ScopeNumber<double> maxoutputscanner;
	
	/** minimum output voltage for scaling of the pockels signal */
	ScopeNumber<double> minoutputpockels;
	
	/** maximum output voltage for scaling of the pockels signal */
	ScopeNumber<double> maxoutputpockels;

	/** @return the _pixeltime coerced to the nearest value the device supports. */
	virtual double CoercedPixeltime(const double& _pixeltime) const { return _pixeltime; }

	/** @return the minimum pixel dwell time/sample time (in microseconds), depending on ... */
	virtual double MinimumPixeltime() const { return 1.0; }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for x/y-scanner/fast z/Pockels signal generation with NI-DAQmx where all output channels (xyzp) are driven by the same (pixel) clock */
class OutputsDAQmx
	: public Outputs {

public:
	OutputsDAQmx();

	std::unique_ptr<Outputs> Clone() const override { return std::unique_ptr<Outputs>(new OutputsDAQmx(*this)); }

	/** Create function for factory */
	static std::unique_ptr<Outputs> Create() { return std::unique_ptr<Outputs>(new OutputsDAQmx()); }

	/** the four analog output channels to use for x, y, z, Pockels */
	ScopeString channelsstring;
	
	/** where to get the sampling clock from? Onboard, from external reference clock, or from external source */
	ScopeValue<DaqTiming> daq_timing;

	/** Minimum sample time for 4 channel output. E.g. for the NI-6259 this is 1/(1.25MS/s) = 0.8 us */
	ScopeNumber<double> minimumpixeltime;
	
	/** source terminal of the reference clock */
	ScopeString referenceclocksource;
	
	/** rate in Hertz of the reference clock.
	* @warning Make sure this is correct even for using the onboard clock! Calculation of allowed pixeltimes depends on this! */
	ScopeNumber<double> referenceclockrate;
	
	/** source terminal of an external clock */
	ScopeString externalclocksource;

	/** terminal to which the pixel/sampleclock is exported */
	ScopeString exportpixelclockterminal;

	double CoercedPixeltime(const double& _pixeltime) const override;

	double MinimumPixeltime() const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for x/y-scanner/fast z/Pockels signal generation with NI-DAQmx where xp are driven by a pixel clock and yz by a line clock.
* @warning Make sure that xpout and yzout are on different boards! On a single NI board there can only be one output tasks (with one sampling clock)!! */
class OutputsDAQmxLineClock
	: public Outputs {

public:
	OutputsDAQmxLineClock();

	std::unique_ptr<Outputs> Clone() const override { return std::unique_ptr<Outputs>(new OutputsDAQmxLineClock(*this)); }

	/** Create function for factory */
	static std::unique_ptr<Outputs> Create() { return std::unique_ptr<Outputs>(new OutputsDAQmxLineClock()); }

	/** Analog output channel for x-scanner and Pockels cell (clocked by pixel clock) */
	ScopeString xpout;

	/** Analog output channel for y-scanner and fast z device (clocked by line clock) */
	ScopeString yzout;

	/** where to get the pixel sampling clock from? Onboard, from external reference clock, or from external source */
	ScopeValue<DaqTiming> pixel_timing;

	/** where to get the line sampling clock from? Onboard, from external reference clock, or from external source */
	ScopeValue<DaqTiming> line_timing;

	/** Minimum sample time for 2 channel output. E.g. for the NI-6259 this is 1/(2.00MS/s) = 0.5 us (see e.g. http://sine.ni.com/ds/app/doc/p/id/ds-22/lang/en).
	* @warning If you are using two different boards the minimumpixeltime is obviously the one from the slower board. 
	* @warning For two different boards there can be problems if the minimum pixeltimes are different/not a multiple of each other! Better use two identical boards! */
	ScopeNumber<double> minimumpixeltime;
	
	/** source terminal of the reference clock for pixel sampling clock */
	ScopeString pixel_referenceclocksource;

	/** source terminal of the reference clock for line sampling clock */
	ScopeString line_referenceclocksource;

	/** rate in Hertz of the reference clock for pixel clock. Make sure this is correct even for using the onboard clock! Calculation of allowed pixel
	* times depends on this! */
	ScopeNumber<double> pixel_referenceclockrate;

	/** rate in Hertz of the reference clock for line clock.
	* @warning Make sure this is correct even for using the onboard clock! Calculation of allowed pixeltimes depends on this! */
	ScopeNumber<double> line_referenceclockrate;

	/** source terminal of an external clock for x and p pixel clock*/
	ScopeString pixel_externalclocksource;

	/** source terminal of an external clock for y and z line clock */
	ScopeString line_externalclocksource;

	double CoercedPixeltime(const double& _pixeltime) const override;

	double MinimumPixeltime() const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for fastz/Pockels signal generation with NI-DAQmx where zp are driven by a pixel clock from the master area.
* @warning Make sure that zpout is on a different board than the master area output. On a single NI board there can only be one output tasks!
* And the master area anyway takes up 4 output channels, so you definitely need a second board. */
class OutputsDAQmxSlave
	: public Outputs {

public:
	OutputsDAQmxSlave();

	std::unique_ptr<Outputs> Clone() const override { return std::unique_ptr<Outputs>(new OutputsDAQmxSlave(*this)); }

	/** Create function for factory */
	static std::unique_ptr<Outputs> Create() { return std::unique_ptr<Outputs>(new OutputsDAQmxSlave()); }

	/** Analog output channel for fast z and Pockels cell (clocked by a pixel clock) */
	ScopeString zpout;

	/** where to get the sampling clock from? Onboard, from external reference clock, or from external source */
	ScopeValue<DaqTiming> timing;

	/** Minimum sample time for 2 channel output. E.g. for the NI-6259 this is 1/(2.00MS/s) = 0.5 us (see e.g. http://sine.ni.com/ds/app/doc/p/id/ds-22/lang/en) */
	ScopeNumber<double> minimumpixeltime;

	/** source terminal of the reference clock for the sampling clock */
	ScopeString referenceclocksource;

	/** rate in Hertz of the reference clock for sampling clock.
	* @warning Make sure this is correct even for using the onboard clock! Calculation of allowed pixeltimes depends on this! */
	ScopeNumber<double> referenceclockrate;

	/** source terminal of an external clock for sampling clock (could be e.g. a routed pixel clock from the master area or pixel clock from an FPGA) */
	ScopeString externalclocksource;

	double CoercedPixeltime(const double& _pixeltime) const override;

	double MinimumPixeltime() const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for signal generation for y scanner and digital zoom with NI-DAQmx when the x scanner is a resonance scanner */
class OutputsDAQmxResonance
	: public Outputs {

public:
	OutputsDAQmxResonance();

	std::unique_ptr<Outputs> Clone() const override { return std::unique_ptr<Outputs>(new OutputsDAQmxResonance(*this)); }

	/** Create function for factory */
	static std::unique_ptr<Outputs> Create() { return std::unique_ptr<Outputs>(new OutputsDAQmxResonance()); }

	/** the four analog output channels to use for x, y, z, Pockels */
	ScopeString channelsstring;

	/** the length of the scanner vector (per channel) */
	ScopeString scanveclength;
	
	/** the digital channel to use for the resonance scanner zoom factor */
	ScopeString zoomchannelstring;

	/** where to get the sampling clock from? Onboard, from external reference clock, or from external source */
	ScopeNumber<DaqTiming> daq_timing;

	/** Minimum sample time for 4 channel output. E.g. for the NI-6259 this is 1/(1.25MS/s) = 0.8 us */
	ScopeNumber<double> minimumpixeltime;
	
	/** source terminal of the reference clock */
	ScopeString referenceclocksource;
	
	/** rate in Hertz of the reference clock.
	* @warning Make sure this is correct even for using the onboard clock! Calculation of allowed pixeltimes depends on this! */
	ScopeNumber<double> referenceclockrate;
	
	/** source terminal of an external clock */
	ScopeString externalclocksource;

	/** terminal to which the pixel/sampleclock is exported */
	ScopeString exportpixelclockterminal;

	double CoercedPixeltime(const double& _pixeltime) const override;

	double MinimumPixeltime() const override;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

/** Parameters for fastz/Pockels signal generation with NI-DAQmx for a slave to a resonance master.
* @warning Make sure that zpout is on a different board than the master area output. On a single NI board there can only be one output tasks!
* And the master area anyway takes up 4 output channels, so you definitely need a second board. */
class OutputsDAQmxResonanceSlave
	: public OutputsDAQmxSlave {

public:
	OutputsDAQmxResonanceSlave();

	std::unique_ptr<Outputs> Clone() const override { return std::unique_ptr<Outputs>(new OutputsDAQmxResonanceSlave(*this)); }

	/** Create function for factory */
	static std::unique_ptr<Outputs> Create() { return std::unique_ptr<Outputs>(new OutputsDAQmxResonanceSlave()); }

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};



}

}
