#include "stdafx.h"
#include "Daq.h"
#include "devices/daqmx/DAQmxTask.h"
#include "helpers/ScopeException.h"

namespace scope {

namespace parameters {

		// Save some typing here...
		using namespace boost::property_tree;


		Daq::Daq(const bool& _isslave)
			: isslave(_isslave)
			, inputs(config::InputParametersType::Create())
			, outputs((_isslave)?config::SlaveOutputParametersType::Create():config::OutputParametersType::Create())
			, shutterline(L"PXI-6259_0/port0/line0", L"ShutterLine")
			, switchresonanceline(L"PXI-6259_0/port0/line3", L"SwitchResonanceLine")
			, pixeltime(10, 0.02, 15, L"Pixeltime_us")
			, scannerdelay(0, -500, 500, L"Scannerdelay_us")
			, averages(1, 1, 32, L"Averages")
			, requested_frames(1, 1, 10000, L"RequestedFrames")
			, resonance_frequency(7910, 3000, 10000, L"ResonanceFrequency_Hz") {

			// Pixeltime lower limit is maximum of minimum pixel times of inputs and outputs (important for FPGA/DAQmx combinations)
			pixeltime.SetLimits(std::max(inputs->MinimumPixeltime(), outputs->MinimumPixeltime()), pixeltime.ul());
			pixeltime.ConnectOther(std::bind(&Daq::CoercePixeltime, this));
		}

		Daq::Daq(const Daq& _daq)
			: isslave(_daq.isslave)
			, inputs(_daq.inputs->Clone())
			, outputs(_daq.outputs->Clone())
			, shutterline(_daq.shutterline)
			, switchresonanceline(_daq.switchresonanceline)
			, pixeltime(_daq.pixeltime)
			, scannerdelay(_daq.scannerdelay)
			, averages(_daq.averages)
			, requested_frames(_daq.requested_frames)
			, resonance_frequency(_daq.resonance_frequency) {

			// Pixeltime lower limit is maximum of minimum pixel times of inputs and outputs (important for FPGA/DAQmx combinations)
			pixeltime.SetLimits(std::max(inputs->MinimumPixeltime(), outputs->MinimumPixeltime()), pixeltime.ul());
			pixeltime.ConnectOther(std::bind(&Daq::CoercePixeltime, this));
		}

		Daq& Daq::operator=(const Daq& _daq) {
			if (isslave != _daq.isslave )
				throw ScopeException(__FUNCTION__);

			inputs = _daq.inputs->Clone();
			outputs = _daq.outputs->Clone();

			shutterline = _daq.shutterline;
			switchresonanceline = _daq.switchresonanceline;
			pixeltime = _daq.pixeltime;
			scannerdelay = _daq.scannerdelay;
			averages = _daq.averages;
			requested_frames = _daq.requested_frames;
			resonance_frequency = _daq.resonance_frequency;

			// Pixeltime lower limit is maximum of minimum pixel times of inputs and outputs (important for FPGA/DAQmx combinations)
			pixeltime.SetLimits(std::max(inputs->MinimumPixeltime(), outputs->MinimumPixeltime()), pixeltime.ul());
			pixeltime.ConnectOther(std::bind(&Daq::CoercePixeltime, this));

			return *this;
		}

		int32_t Daq::ScannerDelaySamples(const bool& _respectoversampling) const {
			if ( inputs->oversampling() && _respectoversampling )
				return round2i32(scannerdelay() / inputs->MinimumPixeltime());
			else
				return round2i32(scannerdelay() / pixeltime());
		}

		void Daq::Load(const wptree& _pt) {
			inputs->Load(_pt.get_child(L"inputs"));
			outputs->Load(_pt.get_child(L"outputs"));
			shutterline.SetFromPropertyTree(_pt);
			switchresonanceline.SetFromPropertyTree(_pt);
			pixeltime.SetFromPropertyTree(_pt);
			scannerdelay.SetFromPropertyTree(_pt);
			averages.SetFromPropertyTree(_pt);
			requested_frames.SetFromPropertyTree(_pt);
			resonance_frequency.SetFromPropertyTree(_pt);
			// easier than to connect from samplingtype, maxrateaggregate, and channels
			pixeltime.SetLimits(std::max(inputs->MinimumPixeltime(), outputs->MinimumPixeltime()), pixeltime.ul());
		}

		void Daq::Save(wptree& _pt) const {
			wptree ptinputs;
			wptree ptoutputs;
			inputs->Save(ptinputs);
			_pt.add_child(L"inputs", ptinputs);
			outputs->Save(ptoutputs);
			_pt.add_child(L"outputs", ptoutputs);
			shutterline.AddToPropertyTree(_pt);
			switchresonanceline.AddToPropertyTree(_pt);
			pixeltime.AddToPropertyTree(_pt);
			scannerdelay.AddToPropertyTree(_pt);
			averages.AddToPropertyTree(_pt);
			requested_frames.AddToPropertyTree(_pt);
			resonance_frequency.AddToPropertyTree(_pt);
		}

		void Daq::SetReadOnlyWhileScanning(const RunState& _runstate) {
			inputs->SetReadOnlyWhileScanning(_runstate);
			outputs->SetReadOnlyWhileScanning(_runstate);
			switch ( static_cast<RunStateHelper::Mode>(_runstate) ) {
				case RunStateHelper::Mode::Stopped:
					pixeltime.SetRWState(true);
					averages.SetRWState(true);
					scannerdelay.SetRWState(true);
					break;
				// RESONANCE CODE
				case RunStateHelper::Mode::RunningSingle:
					//if( SCOPE_USE_RESONANCESCANNER )
					//	scannerdelay.SetRWState(false);
				case RunStateHelper::Mode::RunningStack:
					//if( SCOPE_USE_RESONANCESCANNER )
					//	scannerdelay.SetRWState(false);
				case RunStateHelper::Mode::RunningTimeseries:
					scannerdelay.SetRWState(false);
				case RunStateHelper::Mode::RunningContinuous:
					//if( SCOPE_USE_RESONANCESCANNER )
					//	scannerdelay.SetRWState(false);
				default:
					pixeltime.SetRWState(false);
					averages.SetRWState(false);
			}
		}

		void Daq::CoercePixeltime() {
			// If oversampling, input samples at maximum rate and pixel time can only be integer multiple of that sampling time
			// Otherwise, rate is given by DAQmx or by FPGA
			const double inputtime = inputs->CoercedPixeltime(pixeltime());
			const double outputtime = outputs->CoercedPixeltime(pixeltime());
			DBOUT(L"Daq::CoercePixeltime " << pixeltime() << L" " << inputtime << L" " << outputtime);
			// Only set if not good time already (otherwise we get circular calls...)
			DBOUT(L"Daq::CoercePixeltime inputtime " << inputtime);
			DBOUT(L"Daq::CoercePixeltime outputtime " << outputtime);
			// Do it like this to avoid trouble because of limited precision/rounding errors etc
			if ( !(std::fabs(pixeltime()-inputtime) < std::numeric_limits<double>::epsilon())  )
				pixeltime.Set(inputtime);
		}


	}

}