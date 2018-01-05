#include "stdafx.h"
#include "Inputs.h"
#include "devices/daqmx/DAQmxTask.h"
#include "helpers/ScopeException.h"

namespace scope {

	namespace parameters {

		// Save some typing here...
		using namespace boost::property_tree;


		Inputs::Inputs()
			: channels(2, 1, config::maxchannels, L"Channels")
			, oversampling(true, false, true, L"Oversampling")
			, rangetype(Uint16RangeHelper::full, L"Uint16Range")
			, preframelines(0, 0, 20, L"PreframeLines") {
		}

		void Inputs::Load(const wptree& _pt) {
			channels.SetFromPropertyTree(_pt);
			oversampling.SetFromPropertyTree(_pt);
			rangetype.SetFromPropertyTree(_pt);
			preframelines.SetFromPropertyTree(_pt);
		}

		void Inputs::Save(wptree& _pt) const {
			channels.AddToPropertyTree(_pt);
			oversampling.AddToPropertyTree(_pt);
			rangetype.AddToPropertyTree(_pt);
			preframelines.AddToPropertyTree(_pt);
		}

		void Inputs::SetReadOnlyWhileScanning(const RunState& _runstate) {
		}

		/*std::unique_ptr<Inputs> Inputs::Factory(const config::InputEnum& _type) {
			
			switch ( _type ) {
			case config::InputEnum::DAQmx:
				return InputsDAQmx::Create();
			case config::InputEnum::FPGA_NoiseOutput:
				return InputsFPGA::Create();
			case InputsTypeHelper::InputsFPGAIO6587:
				return InputsFPGAIO6587::Create();
			case config::InputEnum::FPGA_Digitaldemultiplexer:
				return InputsFPGADigitalDemultiplexer::Create();
			case config::InputEnum::FPGA_Photoncounter:
				return InputsFPGAPhotonCounter::Create();
			case InputsTypeHelper::InputsFPGAIO5771:
				return InputsFPGAIO5771::Create();
			case InputsTypeHelper::InputsFPGAIO5751:
				return InputsFPGAIO5751::Create();
			case config::InputEnum::FPGA_Analogdemultiplexer:
				return InputsFPGAAnalogDemultiplexer::Create();
			case config::InputEnum::FPGA_Analogintegrator:
				return InputsFPGAAnalogIntegrator::Create();
			case config::InputEnum::FPGA_Resonancescanner:
				return InputsFPGAResonanceScanner::Create();
			default:
				return InputsDAQmx::Create();
			}
		}*/

		InputsDAQmx::InputsDAQmx()
			: channelsstring(L"PXI-6259_0/ai0:1", L"ChannelsString")
			, range(0.5, 0.1, 10, L"Range_V")
			, daq_timing(DaqTimingHelper::ReferenceClock, L"DAQTimingMethod")
			, referenceclocksource(L"PXI_Clk10", L"ReferenceClockSource")
			, referenceclockrate(10000000, 1000000, 100000000, L"ReferenceClockRate_Hz")
			, externalclocksource(L"/PXI-6259_0/PXI_Trig1", L"ExternalClockSource")
			, maxrateaggregate(1000000, 100000, 20000000, L"MaxRateAggregate_Hz")
			, sampling(DaqSamplingHelper::NonSimultaneousSampling, L"Sampling") {
			uInt32 data = 0;
			std::wostringstream stream;
			DAQmxGetSysNIDAQMajorVersion(&data);
			stream << data;
			DAQmxGetSysNIDAQMinorVersion(&data);
			stream << L"." << data;
			DAQmxGetSysNIDAQUpdateVersion(&data);
			stream << L"." << data;
			diagnosis.daqmxversion = stream.str();
		}

		double InputsDAQmx::MinimumPixeltime() const {
			if ( sampling() == DaqSamplingHelper::SimultaneousSampling )
				return 1/maxrateaggregate() * 1E6;
			else
				return channels()/maxrateaggregate() * 1E6;
		}

		double InputsDAQmx::CoercedPixeltime(const double& _pixeltime) const {
			// If oversampling pixeltime can only be integer multiple of minimum pixel time
			// If not oversampling, supported pixel time is dependent on DAQmx::CoerceSampleRates (see there)
			if ( oversampling() )
				return std::floor(_pixeltime/MinimumPixeltime()) * MinimumPixeltime();	// Always floor here, not to coerce to pixeltime bigger than upper limit (causes circular call in Daq::CoercePixeltime)
			else
				return 1/DAQmx::PredictSampleRate(1/_pixeltime, channels(), referenceclockrate(), DAQmx_Val_AI);
		}

		void InputsDAQmx::Load(const wptree& _pt) {
			Inputs::Load(_pt);
			channelsstring.SetFromPropertyTree(_pt);
			range.SetFromPropertyTree(_pt);
			daq_timing.SetFromPropertyTree(_pt);
			referenceclocksource.SetFromPropertyTree(_pt);
			referenceclockrate.SetFromPropertyTree(_pt);
			externalclocksource.SetFromPropertyTree(_pt);
			maxrateaggregate.SetFromPropertyTree(_pt);
			sampling.SetFromPropertyTree(_pt);
		}

		void InputsDAQmx::Save(wptree& _pt) const {
			Inputs::Save(_pt);
			channelsstring.AddToPropertyTree(_pt);
			range.AddToPropertyTree(_pt);
			daq_timing.AddToPropertyTree(_pt);
			referenceclocksource.AddToPropertyTree(_pt);
			referenceclockrate.AddToPropertyTree(_pt);
			externalclocksource.AddToPropertyTree(_pt);
			maxrateaggregate.AddToPropertyTree(_pt);
			sampling.AddToPropertyTree(_pt);
		}

		void InputsDAQmx::SetReadOnlyWhileScanning(const RunState& _runstate) {
		}

		InputsFPGA::InputsFPGA() {
			// Not oversampling is standard for FPGA, since FPGA generates the pixel clock for the output task
			oversampling = false;
		}

		double InputsFPGA::MinimumPixeltime() const {
			return 1.0;		// in µs
		}

		double InputsFPGA::CoercedPixeltime(const double& _pixeltime) const {
			return _pixeltime;
		}

		void InputsFPGA::Load(const wptree& _pt) {
			Inputs::Load(_pt);
		}

		void InputsFPGA::Save(wptree& _pt) const {
			Inputs::Save(_pt);
		}

		void InputsFPGA::SetReadOnlyWhileScanning(const RunState& _runstate) {
			Inputs::SetReadOnlyWhileScanning(_runstate);
		}

		InputsFPGAIO6587::InputsFPGAIO6587()
			: samplingrate(1E9, 1E8, 1.4E9, L"SamplingRate_Hz") {
		}

		double InputsFPGAIO6587::MinimumPixeltime() const {
			return 60.0 / samplingrate() * 1E6;		// in µs. Must be multiple of 60 samples, since FPGA acquistion loop works on arrays of 60 samples
		}

		double InputsFPGAIO6587::CoercedPixeltime(const double& _pixeltime) const {
			// must be multiple of 60 samples
			return std::floor(_pixeltime/MinimumPixeltime()) * MinimumPixeltime();		// Always floor here, not to coerce to pixeltime bigger than upper limit (causes circular call in Daq::CoercePixeltime)
		}

		void InputsFPGAIO6587::Load(const wptree& _pt) {
			InputsFPGA::Load(_pt);
			samplingrate.SetFromPropertyTree(_pt);
		}

		void InputsFPGAIO6587::Save(wptree& _pt) const {
			InputsFPGA::Save(_pt);
			samplingrate.AddToPropertyTree(_pt);
		}

		void InputsFPGAIO6587::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGA::SetReadOnlyWhileScanning(_runstate);
		}


		InputsFPGADigitalDemultiplexer::InputsFPGADigitalDemultiplexer()
			: countmode(true, false, true, L"CountingMode") {
		}

		void InputsFPGADigitalDemultiplexer::Load(const wptree& _pt) {
			InputsFPGAIO6587::Load(_pt);
			countmode.SetFromPropertyTree(_pt);
		}

		void InputsFPGADigitalDemultiplexer::Save(wptree& _pt) const {
			InputsFPGAIO6587::Save(_pt);
			countmode.AddToPropertyTree(_pt);
		}

		void InputsFPGADigitalDemultiplexer::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGAIO6587::SetReadOnlyWhileScanning(_runstate);
		}

		InputsFPGAPhotonCounter::InputsFPGAPhotonCounter()
			: countmode(true, false, true, L"CountingMode") {
		}

		void InputsFPGAPhotonCounter::Load(const wptree& _pt) {
			InputsFPGAIO6587::Load(_pt);
			countmode.SetFromPropertyTree(_pt);
		}

		void InputsFPGAPhotonCounter::Save(wptree& _pt) const {
			InputsFPGAIO6587::Save(_pt);
			countmode.AddToPropertyTree(_pt);
		}

		void InputsFPGAPhotonCounter::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGAIO6587::SetReadOnlyWhileScanning(_runstate);
		}

		InputsFPGAIO5771::InputsFPGAIO5771()
			: BaselineCh1(127, 0, 255, L"BaselineCh1")
			, BaselineCh2(127, 0, 255, L"BaselineCh2") 
			, CutoffCh1(127, 0, 255, L"CutoffCh1")
			, CutoffCh2(127, 0, 255, L"CutoffCh2"){
		}

		double InputsFPGAIO5771::MinimumPixeltime() const {
			return 8.0E6 / 1.5E9 ;		// in µs. Samples at 1.5GHz. Must be multiple of 8 samples, since FPGA acquires 8 samples in one loop cycle
		}

		double InputsFPGAIO5771::CoercedPixeltime(const double& _pixeltime) const {
			// must be multiple of 8 samples
			return std::floor(_pixeltime/MinimumPixeltime()) * MinimumPixeltime();		// Always floor here, not to coerce to pixeltime bigger than upper limit (causes circular call in Daq::CoercePixeltime)
		}

		void InputsFPGAIO5771::Load(const wptree& _pt) {
			InputsFPGA::Load(_pt);
			BaselineCh1.SetFromPropertyTree(_pt);
			BaselineCh2.SetFromPropertyTree(_pt);
			CutoffCh1.SetFromPropertyTree(_pt);
			CutoffCh2.SetFromPropertyTree(_pt);
		}

		void InputsFPGAIO5771::Save(wptree& _pt) const {
			InputsFPGA::Save(_pt);
			BaselineCh1.AddToPropertyTree(_pt);
			BaselineCh2.AddToPropertyTree(_pt);
			CutoffCh1.AddToPropertyTree(_pt);
			CutoffCh2.AddToPropertyTree(_pt);
		}

		void InputsFPGAIO5771::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGA::SetReadOnlyWhileScanning(_runstate);
		}


		InputsFPGAIO5751::InputsFPGAIO5751()
			: BaselineCh1(0, 0, 65535, L"BaselineCh1")
			, BaselineCh2(0, 0, 65535, L"BaselineCh2")
			, AcquisitionClockRate(40E6, 10E6, 50E6, L"AcquisitionClockRate_Hz") {
		}

		double InputsFPGAIO5751::MinimumPixeltime() const {
			return 1/AcquisitionClockRate()*1E6;		// in microseconds.
		}

		double InputsFPGAIO5751::MaximumPixeltime() const {
			return 10;
		}

		double InputsFPGAIO5751::CoercedPixeltime(const double& _pixeltime) const {
			// 2, because the fastest possible acqusition would lead to performance problems in Scope
			return std::max(2*MinimumPixeltime(), std::floor(_pixeltime*AcquisitionClockRate()/1E6) * 1E6/AcquisitionClockRate());		// Always floor here, not to coerce to pixeltime bigger than upper limit (causes circular call in Daq::CoercePixeltime)
		}

		void InputsFPGAIO5751::Load(const wptree& _pt) {
			InputsFPGA::Load(_pt);
			BaselineCh1.SetFromPropertyTree(_pt);
			BaselineCh2.SetFromPropertyTree(_pt);
			AcquisitionClockRate.SetFromPropertyTree(_pt);
		}

		void InputsFPGAIO5751::Save(wptree& _pt) const {
			InputsFPGA::Save(_pt);
			BaselineCh1.AddToPropertyTree(_pt);
			BaselineCh2.AddToPropertyTree(_pt);
			AcquisitionClockRate.AddToPropertyTree(_pt);
		}

		void InputsFPGAIO5751::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGA::SetReadOnlyWhileScanning(_runstate);
		}


		InputsFPGAAnalogDemultiplexer::InputsFPGAAnalogDemultiplexer()
			: BitshiftA1Ch1(3, 0, 16, L"BitshiftA1Ch1")
			, BitshiftA1Ch2(3, 0, 16, L"BitshiftA1Ch2")
			, BitshiftA2Ch1(3, 0, 16, L"BitshiftA2Ch1")
			, BitshiftA2Ch2(3, 0, 16, L"BitshiftA2Ch2") {
		}

		void InputsFPGAAnalogDemultiplexer::Load(const wptree& pt) {
			InputsFPGAIO5771::Load(pt);
			BitshiftA1Ch1.SetFromPropertyTree(pt);
			BitshiftA1Ch2.SetFromPropertyTree(pt);
			BitshiftA2Ch1.SetFromPropertyTree(pt);
			BitshiftA2Ch2.SetFromPropertyTree(pt);
		}

		void InputsFPGAAnalogDemultiplexer::Save(wptree& pt) const {
			InputsFPGAIO5771::Save(pt);
			BitshiftA1Ch1.AddToPropertyTree(pt);
			BitshiftA1Ch2.AddToPropertyTree(pt);
			BitshiftA2Ch1.AddToPropertyTree(pt);
			BitshiftA2Ch2.AddToPropertyTree(pt);
		}

		void InputsFPGAAnalogDemultiplexer::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGAIO5771::SetReadOnlyWhileScanning(_runstate);
		}


		InputsFPGAAnalogIntegrator::InputsFPGAAnalogIntegrator()
			: BitshiftCh1(3, 0, 16, L"BitshiftCh1")
			, BitshiftCh2(3, 1, 16, L"BitshiftCh2") {
		}

		void InputsFPGAAnalogIntegrator::Load(const wptree& pt) {
			InputsFPGAIO5771::Load(pt);
			BitshiftCh1.SetFromPropertyTree(pt);
			BitshiftCh2.SetFromPropertyTree(pt);
		}

		void InputsFPGAAnalogIntegrator::Save(wptree& pt) const {
			InputsFPGAIO5771::Save(pt);
			BitshiftCh1.AddToPropertyTree(pt);
			BitshiftCh2.AddToPropertyTree(pt);
		}

		void InputsFPGAAnalogIntegrator::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGAIO5771::SetReadOnlyWhileScanning(_runstate);
		}


		InputsFPGAResonanceScanner::InputsFPGAResonanceScanner()
			: BitshiftCh1(0, 0, 16, L"BitshiftCh1")
			, BitshiftCh2(0, 0, 16, L"BitshiftCh2") {
		}

		void InputsFPGAResonanceScanner::Load(const wptree& pt) {
			InputsFPGAIO5751::Load(pt);
			BitshiftCh1.SetFromPropertyTree(pt);
			BitshiftCh2.SetFromPropertyTree(pt);
		}

		void InputsFPGAResonanceScanner::Save(wptree& pt) const {
			InputsFPGAIO5751::Save(pt);
			BitshiftCh1.AddToPropertyTree(pt);
			BitshiftCh2.AddToPropertyTree(pt);
		}

		void InputsFPGAResonanceScanner::SetReadOnlyWhileScanning(const RunState& _runstate) {
			InputsFPGAIO5751::SetReadOnlyWhileScanning(_runstate);
		}


	}

}