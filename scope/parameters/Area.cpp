#include "stdafx.h"
#include "Area.h"

namespace scope {
	namespace parameters {

		using boost::property_tree::wptree;
	
		BaseArea::BaseArea(const uint32_t& _area, const AreaType& _at)
			: areatype(_at)
			, area(_area, 0, 100, (_at==AreaTypeHelper::Master)?L"MasterArea":L"SlaveArea")
			, histrange(100, 0, 65535, L"HistRange")
			, daq(false)
			, linerate(1, 0, 100000, L"Linerate_Hz")
			, framerate(1, 0, 1000, L"Framerate_Hz")
			, frametime(1, 0, 100000, L"Frametime_s")
			, scanmode(ScannerVectorTypeHelper::Sawtooth, L"ScanMode")
			, basemicronperpixelx(0.1, 1E-6, 100, L"BaseMicronPerPixelX")
			, basemicronperpixely(0.1, 1E-6, 100, L"BaseMicronPerPixelY")
			, micronperpixelx(0.1, 1E-6, 100, L"MicronPerPixelX")
			, micronperpixely(0.1, 1E-6, 100, L"MicronPerPixelY") {

			// Insert all possible scannervectors
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Sawtooth, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Sawtooth));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Bidirectional, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Bidirectional));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::Planehopper, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::Planehopper));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::ResonanceBiDi, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::ResonanceBiDi));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::ResonanceHopper, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::ResonanceHopper));
			scannervectorframesmap.emplace(ScannerVectorTypeHelper::Mode::LineStraight, ScannerVectorFrameBasic::Factory(ScannerVectorTypeHelper::Mode::LineStraight));

			InitializeConnections();
		}

		BaseArea::BaseArea(const BaseArea& _a)
			: areatype(_a.areatype)
			, area(_a.area)
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

		BaseArea& BaseArea::operator=(const BaseArea& _a) {
			if (this != &_a) {
				areatype = _a.areatype;
				area = _a.area;
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

		ScannerVectorFrameBasic& BaseArea::Currentframe() const {
			return *scannervectorframesmap.at(scanmode.Value()).get();
		}

		void BaseArea::Load(const wptree& pt) {
			areatype.SetFromPropertyTree(pt);
			area.SetFromPropertyTree(pt);
			histrange.SetFromPropertyTree(pt);
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

		void BaseArea::Save(wptree& pt) const {
			areatype.AddToPropertyTree(pt);
			area.AddToPropertyTree(pt);
			histrange.AddToPropertyTree(pt);
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

		void BaseArea::SetReadOnlyWhileScanning(const RunState& _runstate) {
			const bool enabler = (_runstate.t == RunStateHelper::Mode::Stopped) ? true : false;
			scanmode.SetRWState(enabler);
			fpuxystage.SetReadOnlyWhileScanning(_runstate);
			fpuzstage.SetReadOnlyWhileScanning(_runstate);
			daq.SetReadOnlyWhileScanning(_runstate);
			for (auto& sv : scannervectorframesmap)
				sv.second->SetReadOnlyWhileScanning(_runstate);
		}

		void BaseArea::SaveToPreset(const std::wstring& _name) {
			scannervectorframesmap.at(scanmode.Value())->SaveToPreset(_name, daq);
		}

		void BaseArea::LoadFromPreset(const std::wstring& _name) {
			scannervectorframesmap.at(scanmode.Value())->LoadFromPreset(_name, daq);
		}

		void BaseArea::DeletePreset(const std::wstring& _name) {
			scannervectorframesmap.at(scanmode.Value())->DeletePreset(_name);
		}

		double BaseArea::FrameTime() const {
			return scannervectorframesmap.at(scanmode.Value())->TotalPixels() * daq.pixeltime() * 1E-6;
		}

		double BaseArea::LineTime() const {
			return scannervectorframesmap.at(scanmode.Value())->XTotalPixels() * daq.pixeltime() * 1E-6;
		}

		uint32_t BaseArea::TotalPixelsAllChannels() const {
			return scannervectorframesmap.at(scanmode.Value())->TotalPixels()*daq.inputs->channels();
		}

		double BaseArea::XOffsetInMicron() const {
			// xoffset goes from -1..+1 (maxima of the zoom 1 field of view), thus:
			return scannervectorframesmap.at(scanmode.Value())->xoffset() * basemicronperpixelx() * 256.0 * 0.5;
		}

		double BaseArea::YOffsetInMicron() const {
			// yoffset goes from -1..+1 (maxima of the zoom 1 field of view), thus:
			return scannervectorframesmap.at(scanmode.Value())->yoffset() * basemicronperpixely() * 256.0 * 0.5;
		}

		void BaseArea::InitializeConnections() {
			scanmode.ConnectOther(std::bind(&BaseArea::ChangeScanMode, this));
			
			// Iterate over all scannervectorsframe and connect changes of their parameters to calculating functions in Area
			for (auto& sv : scannervectorframesmap) {
				sv.second->ConnectRateUpdate(std::bind(&BaseArea::UpdateRates, this));
				sv.second->ConnectResolutionUpdate(std::bind(&BaseArea::CalculateResolution, this));
				sv.second->ConnectMicronPerPixelUpdate(std::bind(&BaseArea::CalculateMicronPerPixel, this));
			}
			
			daq.pixeltime.ConnectOther(std::bind(&BaseArea::UpdateRates, this));

			ChangeScanMode();
			UpdateRates();
			UpdateFastZCalibration();
		}

		void BaseArea::ChangeScanMode() {
			UpdateRates();
			CalculateMicronPerPixel();
		}

		void BaseArea::UpdateRates() {
			framerate = 1 / FrameTime();
			frametime = FrameTime();
			linerate = 1 / LineTime();
		}

		void BaseArea::UpdateFastZCalibration() {
			DBOUT(L"BaseArea::UpdateFastZCalibration");
			if (!fpuzstage.calibration.empty()) {
				for (auto& sv : scannervectorframesmap)
					sv.second->fastz.SetLimits(fpuzstage.calibration.begin()->first, (--fpuzstage.calibration.end())->first);
			}
		}

		void BaseArea::CalculateMicronPerPixel() {
			// basemicronperpixel must be calibrated to zoom 1 256x256 pixel
			micronperpixelx = basemicronperpixelx() / Currentframe().zoom() * (256.0 / static_cast<double>(Currentframe().xres()));
			micronperpixely = basemicronperpixely() / Currentframe().zoom() * (256.0 / static_cast<double>(Currentframe().yres()));
		}

		void BaseArea::CalculateResolution() {
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

		SlaveArea::SlaveArea(const uint32_t& _slavearea, MasterArea * const _itsmasterarea)
			: BaseArea(_slavearea, AreaTypeHelper::Slave)
			, masterarea(_itsmasterarea) {
		}

		SlaveArea::SlaveArea(const SlaveArea& _a)
			: BaseArea(_a) {
			masterarea = nullptr;
		}

		
		SlaveArea& SlaveArea::operator=(const SlaveArea& _a) {
			BaseArea::operator=(_a);
			if (this != &_a) {
				masterarea = nullptr;
			}

			return *this;
		}

		void SlaveArea::SetMasterArea(MasterArea* const _masterarea) {
			assert(_masterarea != nullptr);
			masterarea = _masterarea;
			CopyFromMasterArea();
			ChangeScanMode();
			UpdateRates();
		}

		void SlaveArea::CopyFromMasterArea() {
			if (masterarea != nullptr) {
				// Iterate over scannervectors
				for (auto& sv : scannervectorframesmap) {
					*sv.second = *masterarea->scannervectorframesmap.at(sv.first);
				}

				// Copy master scanmode onto slave, so that the correct scanner vector is generated (Karlis)
				scanmode = masterarea->scanmode;
			}
			else {
				DBOUT(L"SlaveArea::CopyFromMasterArea called but masterarea* is nullptr.");
			}
		}

		void SlaveArea::InitializeConnections() {
			scanmode.ConnectOther(std::bind(&BaseArea::ChangeScanMode, this));

			// A slave area gets these from the master area. These connection go in the collector so they get disconnected when this
			// area is destructed
			// Iterate over all scannervectorsframe
			for (auto& masv : masterarea->scannervectorframesmap) {
				auto conns = masv.second->ConnectCopyTrigger(std::bind(&SlaveArea::CopyFromMasterArea, this));
				connection_collector.insert(connection_collector.end(), conns.begin(), conns.end());
			}
			CopyFromMasterArea();
			
			daq.pixeltime.ConnectOther(std::bind(&SlaveArea::UpdateRates, this));

			ChangeScanMode();
			UpdateRates();
			UpdateFastZCalibration();
		}

		void SlaveArea::ChangeScanMode() {
			BaseArea::ChangeScanMode();
			CopyFromMasterArea();
		}

	}

}