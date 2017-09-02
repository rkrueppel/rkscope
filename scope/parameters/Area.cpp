#include "stdafx.h"
#include "Area.h"

namespace scope {
	namespace parameters {

		using boost::property_tree::wptree;
	
		Area::Area(const uint32_t& _area, const bool& _isslave, Area * const _masterarea)
			: area(_area, 0, 16, L"Area")
			, histrange(100, 0, 65535, L"HistRange")
			, isslave(_isslave, false, true, L"IsSlaveArea")
			, daq(_isslave)
			, masterarea(_masterarea)
			, linerate(1, 0, 100000, L"Linerate_Hz")
			, framerate(1, 0, 1000, L"Framerate_Hz")
			, frametime(1, 0, 100000, L"Frametime_s")
			, scanmode(ScannerVectorTypeHelper::Sawtooth, L"ScanMode")
			, basemicronperpixelx(0.1, 1E-6, 100, L"BaseMicronPerPixelX")
			, basemicronperpixely(0.1, 1E-6, 100, L"BaseMicronPerPixelY")
			, micronperpixelx(0.1, 1E-6, 100, L"MicronPerPixelX")
			, micronperpixely(0.1, 1E-6, 100, L"MicronPerPixelY") {

			// Make sure the parameter (isslave) equals the compiled logic (ThisIsSlaveArea, which uses the macro definitions of SCOPE_NAREAS etc)
			assert(isslave() == ThisIsSlaveArea(area));

			// Make sure we set the pointer to the masterarea for slave areas
			assert((isslave() == true && (masterarea != nullptr)) || (isslave() == false));

			// Insert all possible scannervectors
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Sawtooth, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Sawtooth));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Bidirectional, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Bidirectional));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Planehopper, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Planehopper));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::ResonanceBiDi, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::ResonanceBiDi));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::ResonanceHopper, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::ResonanceHopper));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::LineStraight, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::LineStraight));

			InitializeConnections();
		}

		Area::Area(const Area& _a)
			: area(_a.area)
			, isslave(_a.isslave)
			, masterarea(_a.masterarea)
			, daq(_a.daq)
			, fpuxystage(_a.fpuxystage)
			, fpuzstage(_a.fpuzstage)
			, scannervectorframesmap()
			, scanmode(_a.scanmode(), L"ScanMode")
			, basemicronperpixelx(_a.basemicronperpixelx)
			, basemicronperpixely(_a.basemicronperpixely)
			, micronperpixelx(_a.micronperpixelx)
			, micronperpixely(_a.micronperpixely)
			, linerate(_a.linerate)
			, framerate(_a.framerate)
			, frametime(_a.frametime) {

			for (auto& sv : _a.scannervectorframesmap)
				scannervectorframesmap.emplace(std::make_pair(sv.first, ScannerVectorFrameBasic::Factory(sv.first, sv.second.get())));

			InitializeConnections();
		}

		Area& Area::operator=(const Area& _a) {
			if (this != &_a) {
				area = _a.area;
				isslave = _a.isslave;
				masterarea = _a.masterarea;
				daq = _a.daq;
				fpuxystage = _a.fpuxystage;
				fpuzstage = _a.fpuzstage;
				scanmode = _a.scanmode();
				basemicronperpixelx = _a.basemicronperpixelx;
				basemicronperpixely = _a.basemicronperpixely;
				micronperpixelx = _a.micronperpixelx;
				micronperpixely = _a.micronperpixely;
				linerate = _a.linerate;
				framerate = _a.framerate;
				frametime = _a.frametime;

				for (auto& sv : _a.scannervectorframesmap)
					scannervectorframesmap.emplace(std::make_pair(sv.first, ScannerVectorFrameBasic::Factory(sv.first, sv.second.get())));

			}

			// Not needed here?!
			//InitializeConnections();

			return *this;
		}

		void Area::SetMasterArea(Area* const _masterarea) {
			assert((isslave() == true && (_masterarea != nullptr)) || (isslave() == false));
			masterarea = _masterarea;
			if (isslave())
				CopyFromMasterArea();
			ChangeScanMode();
			UpdateRates();
		}

		void Area::CopyFromMasterArea() {
			// Iterate over scannervectors
			for (auto& sv : scannervectorframesmap) {
				*sv.second = *masterarea->scannervectorframesmap.at(sv.first);
			}

			// Copy master scanmode onto slave, so that the correct scanner vector is generated (Karlis)
			scanmode = masterarea->scanmode;
		}

		ScannerVectorFrameBasic& Area::Currentframe() const {
			return *scannervectorframesmap.at(scanmode.Value()).get();
		}

		ScannerVectorFrameSaw& Area::FrameSaw() const {
			try {
				return dynamic_cast<ScannerVectorFrameSaw&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::Sawtooth).get());
			}
			catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameSaw", true, true, true, true); }
		}

		ScannerVectorFrameResonance& Area::FrameResonance() const {
			try {
				return dynamic_cast<ScannerVectorFrameResonance&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::ResonanceBiDi));
			}
			catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameResonance", true, true, true, true); }
		}

		ScannerVectorFrameBiDi& Area::FrameBiDi() const {
			try {
				return dynamic_cast<ScannerVectorFrameBiDi&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::Bidirectional));
			}
			catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameBiDi", true, true, true, true); }
		}

		ScannerVectorFramePlaneHopper& Area::FrameHopper() const {
			try {
				return dynamic_cast<ScannerVectorFramePlaneHopper&>(*scannervectorframesmap.at(ScannerVectorTypeHelper::Mode::Planehopper));
			}
			catch (std::bad_cast) { ScopeExceptionHandler("parameters::Area::FrameHopper", true, true, true, true); }
		}

		void Area::Load(const wptree& pt) {
			area.SetFromPropertyTree(pt);
			histrange.SetFromPropertyTree(pt);
			isslave.SetFromPropertyTree(pt);
			linerate.SetFromPropertyTree(pt);
			framerate.SetFromPropertyTree(pt);
			frametime.SetFromPropertyTree(pt);
			scanmode.SetFromPropertyTree(pt);
			basemicronperpixelx.SetFromPropertyTree(pt);
			basemicronperpixely.SetFromPropertyTree(pt);
			micronperpixelx.SetFromPropertyTree(pt);
			micronperpixely.SetFromPropertyTree(pt);
			daq.Load(pt.get_child(L"daq"));

			for (auto& sv : scannervectorframesmap) {
				sv.second->Load(pt.get_child(ScannerVectorTypeHelper::NameOf(sv.first)));
			}

			fpuzstage.Load(pt.get_child(L"fpuzstage"));
			fpuxystage.Load(pt.get_child(L"fpuxystage"));
			ChangeScanMode();
			UpdateRates();
			UpdateFastZCalibration();
		}

		void Area::Save(wptree& pt) const {
			area.AddToPropertyTree(pt);
			histrange.AddToPropertyTree(pt);
			isslave.AddToPropertyTree(pt);
			linerate.AddToPropertyTree(pt);
			framerate.AddToPropertyTree(pt);
			frametime.AddToPropertyTree(pt);
			scanmode.AddToPropertyTree(pt);
			basemicronperpixelx.AddToPropertyTree(pt);
			basemicronperpixely.AddToPropertyTree(pt);
			micronperpixelx.AddToPropertyTree(pt);
			micronperpixely.AddToPropertyTree(pt);
			wptree ptdaq;
			wptree ptpresets;
			wptree ptfpuzstage;
			wptree ptfpuxystage;

			daq.Save(ptdaq);
			pt.add_child(L"daq", ptdaq);

			for (const auto& sv : scannervectorframesmap) {
				wptree ptsv;
				sv.second->Save(ptsv);
				pt.add_child(ScannerVectorTypeHelper::NameOf(sv.first), ptsv);
			}

			fpuzstage.Save(ptfpuzstage);
			pt.add_child(L"fpuzstage", ptfpuzstage);
			fpuxystage.Save(ptfpuxystage);
			pt.add_child(L"fpuxystage", ptfpuxystage);
		}

		void Area::SetReadOnlyWhileScanning(const RunState& _runstate) {
			const bool enabler = (_runstate.t == RunStateHelper::Mode::Stopped) ? true : false;
			scanmode.SetRWState(enabler);
			fpuxystage.SetReadOnlyWhileScanning(_runstate);
			fpuzstage.SetReadOnlyWhileScanning(_runstate);
			daq.SetReadOnlyWhileScanning(_runstate);
			for (auto& sv : scannervectorframesmap)
				sv.second->SetReadOnlyWhileScanning(_runstate);
		}

		void Area::SaveToPreset(const std::wstring& _name) {
			scannervectorframesmap.at(scanmode.Value())->SaveToPreset(_name, daq);
		}

		void Area::LoadFromPreset(const std::wstring& _name) {
			scannervectorframesmap.at(scanmode.Value())->LoadFromPreset(_name, daq);
		}

		void Area::DeletePreset(const std::wstring& _name) {
			scannervectorframesmap.at(scanmode.Value())->DeletePreset(_name);
		}

		double Area::FrameTime() const {
			return scannervectorframesmap.at(scanmode.Value())->TotalPixels() * daq.pixeltime() * 1E-6;
		}

		double Area::LineTime() const {
			return scannervectorframesmap.at(scanmode.Value())->XTotalPixels() * daq.pixeltime() * 1E-6;
		}

		uint32_t Area::TotalPixelsAllChannels() const {
			return scannervectorframesmap.at(scanmode.Value())->TotalPixels()*daq.inputs->channels();
		}

		double Area::XOffsetInMicron() const {
			// xoffset goes from -1..+1 (maxima of the zoom 1 field of view), thus:
			return scannervectorframesmap.at(scanmode.Value())->xoffset() * basemicronperpixelx() * 256.0 * 0.5;
		}

		double Area::YOffsetInMicron() const {
			// yoffset goes from -1..+1 (maxima of the zoom 1 field of view), thus:
			return scannervectorframesmap.at(scanmode.Value())->yoffset() * basemicronperpixely() * 256.0 * 0.5;
		}

		void Area::InitializeConnections() {
			scanmode.ConnectOther(std::bind(&Area::ChangeScanMode, this));
			if (isslave()) {
				// A slave area gets these from the master area. These connection go in the collector so they get disconnected when this
				// area is destructed
				// Iterate over all scannervectorsframe
				for (auto& masv : masterarea->scannervectorframesmap) {
					auto conns = masv.second->ConnectCopyTrigger(std::bind(&Area::CopyFromMasterArea, this));
					connection_collector.insert(connection_collector.end(), conns.begin(), conns.end());
				}
				CopyFromMasterArea();
			}
			else {
				// Iterate over all scannervectorsframe and connect changes of their parameters to calculating functions in Area
				for (auto& sv : scannervectorframesmap) {
					sv.second->ConnectRateUpdate(std::bind(&Area::UpdateRates, this));
					sv.second->ConnectResolutionUpdate(std::bind(&Area::CalculateResolution, this));
					sv.second->ConnectMicronPerPixelUpdate(std::bind(&Area::CalculateMicronPerPixel, this));
				}
			}
			daq.pixeltime.ConnectOther(std::bind(&Area::UpdateRates, this));

			ChangeScanMode();
			UpdateRates();
			UpdateFastZCalibration();
		}

		void Area::ChangeScanMode() {
			UpdateRates();
			CalculateMicronPerPixel();
			if (isslave())
				CopyFromMasterArea();
		}

		void Area::UpdateRates() {
			framerate = 1 / FrameTime();
			frametime = FrameTime();
			linerate = 1 / LineTime();
		}

		void Area::UpdateFastZCalibration() {
			DBOUT(L"Area::UpdateFastZCalibration");
			if (!fpuzstage.calibration.empty()) {
				for (auto& sv : scannervectorframesmap)
					sv.second->fastz.SetLimits(fpuzstage.calibration.begin()->first, (--fpuzstage.calibration.end())->first);
			}
		}

		void Area::CalculateMicronPerPixel() {
			// basemicronperpixel must be calibrated to zoom 1 256x256 pixel
			micronperpixelx = basemicronperpixelx() / Currentframe().zoom() * (256.0 / static_cast<double>(Currentframe().xres()));
			micronperpixely = basemicronperpixely() / Currentframe().zoom() * (256.0 / static_cast<double>(Currentframe().yres()));
		}

		void Area::CalculateResolution() {
			scannervectorframesmap.at(scanmode.Value())->xres = static_cast<double>(Currentframe().yres) / Currentframe().yaspectratio * Currentframe().xaspectratio;
			double xaspectratioboundary = Currentframe().yaspectratio / static_cast<double>(Currentframe().yres) * static_cast<double>(Currentframe().xres);
			if (Currentframe().xaspectratio < xaspectratioboundary || Currentframe().xaspectratio > xaspectratioboundary) {
				scannervectorframesmap.at(scanmode.Value())->xaspectratio = xaspectratioboundary;
			}

			scannervectorframesmap.at(scanmode.Value())->yres = static_cast<double>(Currentframe().xres) / Currentframe().xaspectratio * Currentframe().yaspectratio;
			double yaspectratioboundary = Currentframe().xaspectratio / static_cast<double>(Currentframe().xres) * static_cast<double>(Currentframe().yres);
			if (Currentframe().yaspectratio < yaspectratioboundary || Currentframe().yaspectratio > yaspectratioboundary) {
				scannervectorframesmap.at(scanmode.Value())->yaspectratio = yaspectratioboundary;
			}
		}

}}