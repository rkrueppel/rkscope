#include "stdafx.h"
#include "Framescan.h"
#include "Scope.h"

namespace scope {
	
namespace parameters {

		// Save some typing here...
		using namespace boost::property_tree;

		ScannerVectorFrameBasic::Preset::Preset()
			: name(L"None", L"Name")
			, xres(256, 16, 10000, L"XResolution_Pixel")
			, yres(256, 16, 10000, L"YResolution_Pixel")
			, xaspectratio(1, 0.0001, 10000, L"XAspectRatio")
			, yaspectratio(1, 0.0001, 10000, L"YAspectRatio")
			, squarepixels(false, false, true, L"SquarePixels") {
		}

		void ScannerVectorFrameBasic::Preset::Load(const wptree& pt)  {
			name.SetFromPropertyTree(pt);
			xres.SetFromPropertyTree(pt);
			yres.SetFromPropertyTree(pt);
			xaspectratio.SetFromPropertyTree(pt);
			yaspectratio.SetFromPropertyTree(pt);
			squarepixels.SetFromPropertyTree(pt);
		}

		void ScannerVectorFrameBasic::Preset::Save(wptree& pt) const {
			name.AddToPropertyTree(pt);
			xres.AddToPropertyTree(pt);
			yres.AddToPropertyTree(pt);
			xaspectratio.AddToPropertyTree(pt);
			yaspectratio.AddToPropertyTree(pt);
			squarepixels.AddToPropertyTree(pt);
		}

		ScannerVectorFrameBasic::ScannerVectorFrameBasic()
			: presets(0)
			, xres(256, 16, 10000, L"XResolution_Pixel")
			, yres(256, 16, 10000, L"YResolution_Pixel")
			, xaspectratio(1, 0.0001, 10000, L"XAspectRatio")
			, yaspectratio(1, 0.0001, 10000, L"YAspectRatio")
			, squarepixels(false, false, true, L"SquarePixels")
			, zoom(1.0, 1.0 , 20.0, L"Zoom")
			, xoffset(0.0, -1.0, 1.0, L"XOffset_Fraction")
			, yoffset(0.0, -1.0, 1.0, L"YOffset_Fraction")
			, fastz(0.0, -5.0, 5.0, L"FastZ_um")
			, pockels(0.0, 0.0, 1.0, L"Pockels_Fraction")
			, xrotation(0.0, 0.0, 90.0, L"XRotation_Degree")
			, yrotation(0.0, 0.0, 90.0, L"YRotation_Degree") {
			zoom.ConnectOther(std::bind(&ScannerVectorFrameBasic::CoerceOffset, this));
			CoerceOffset();
		}

		void ScannerVectorFrameBasic::Load(const wptree& pt) {
			xres.SetFromPropertyTree(pt);
			yres.SetFromPropertyTree(pt);
			xaspectratio.SetFromPropertyTree(pt);
			yaspectratio.SetFromPropertyTree(pt);
			squarepixels.SetFromPropertyTree(pt);
			zoom.SetFromPropertyTree(pt);
			xoffset.SetFromPropertyTree(pt);
			yoffset.SetFromPropertyTree(pt);
			fastz.SetFromPropertyTree(pt);
			pockels.SetFromPropertyTree(pt);
			xrotation.SetFromPropertyTree(pt);
			yrotation.SetFromPropertyTree(pt);
			try {
				wptree presetstree = pt.get_child(L"presets");
				for ( auto subtree : presetstree ) {
					// Call make preset (could be from derived class) to generate the correct (derived) Preset, and downcast it to
					// ScannerVectorFrameBasic::Preset
					std::shared_ptr<ScannerVectorFrameBasic::Preset> p = MakePreset();
					p->Load(subtree.second);
					presets.push_back(p);
				}
			} catch (boost::property_tree::ptree_bad_path) {
				DBOUT(L"Bad path while loading presets");
			}
		}

		void ScannerVectorFrameBasic::Save(wptree& pt) const {
			xres.AddToPropertyTree(pt);
			yres.AddToPropertyTree(pt);
			xaspectratio.AddToPropertyTree(pt);
			yaspectratio.AddToPropertyTree(pt);
			squarepixels.AddToPropertyTree(pt);
			zoom.AddToPropertyTree(pt);
			xoffset.AddToPropertyTree(pt);
			yoffset.AddToPropertyTree(pt);
			fastz.AddToPropertyTree(pt);
			pockels.AddToPropertyTree(pt);
			xrotation.AddToPropertyTree(pt);
			yrotation.AddToPropertyTree(pt);
			wptree ptpresets;
			uint32_t i = 0;
			CString name(L"");
			for ( auto p : presets ) {
				wptree subtree;
				p->Save(subtree);
				name.Format(L"preset%d", i++);
				ptpresets.add_child(name.GetString(), subtree);
			}
			pt.add_child(L"presets", ptpresets);
		}

		void ScannerVectorFrameBasic::SetReadOnlyWhileScanning(const RunState& _runstate) {
			switch ( static_cast<RunStateHelper::Mode>(_runstate) ) {
				case RunStateHelper::Mode::Stopped:
					zoom.SetRWState(true);
					pockels.SetRWState(true);
					fastz.SetRWState(true);
					xoffset.SetRWState(true);
					yoffset.SetRWState(true);
					xrotation.SetRWState(true);
					yrotation.SetRWState(true);
					xres.SetRWState(true);
					yres.SetRWState(true);
					xaspectratio.SetRWState(true);
					yaspectratio.SetRWState(true);
					squarepixels.SetRWState(true);
					xrotation.SetRWState(true);
					yrotation.SetRWState(true);
					break;
				case RunStateHelper::Mode::RunningSingle:
				case RunStateHelper::Mode::RunningStack:
				case RunStateHelper::Mode::RunningTimeseries:
					zoom.SetRWState(false);
					pockels.SetRWState(false);
					fastz.SetRWState(false);
					xoffset.SetRWState(false);
					yoffset.SetRWState(false);
					xrotation.SetRWState(false);
					yrotation.SetRWState(false);
					// Do not call break, but do the rest too
				case RunStateHelper::Mode::RunningContinuous:
				default:
					xres.SetRWState(false);
					yres.SetRWState(false);
					xaspectratio.SetRWState(false);
					yaspectratio.SetRWState(false);
					squarepixels.SetRWState(false);
					xrotation.SetRWState(false);
					yrotation.SetRWState(false);
			}
		}

		std::unique_ptr<ScannerVectorFrameBasic::Preset> ScannerVectorFrameBasic::MakePreset() const {
			return std::make_unique<ScannerVectorFrameBasic::Preset>();
		}

		void ScannerVectorFrameBasic::DeletePreset(const std::wstring& _name) {
			auto samename = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if  ( presets.end() != samename )
				presets.erase(samename);				// If name exists, delete
		}

		void ScannerVectorFrameBasic::CoerceOffset() {
			xoffset.SetLimits(-1+1/zoom(), 1-1/zoom());
			yoffset.SetLimits(-1+1/zoom(), 1-1/zoom());
			// SetLimits call change signals, thus xoffset and yoffset get updated on GUI etc.
		}


		std::unique_ptr<ScannerVectorFrameBasic> ScannerVectorFrameBasic::Factory(const ScannerVectorType& _type, const ScannerVectorFrameBasic* const _o) {
			switch ( _type.t ) {
			case ScannerVectorTypeHelper::Sawtooth:
				return (_o==nullptr)?ScannerVectorFrameSaw::Create():ScannerVectorFrameSaw::Create(*dynamic_cast<const ScannerVectorFrameSaw*>(_o));
			case ScannerVectorTypeHelper::Bidirectional:
				return (_o==nullptr)?ScannerVectorFrameBiDi::Create():ScannerVectorFrameBiDi::Create(*dynamic_cast<const ScannerVectorFrameBiDi*>(_o));
			case ScannerVectorTypeHelper::Planehopper:
				return (_o==nullptr)?ScannerVectorFramePlaneHopper::Create():ScannerVectorFramePlaneHopper::Create(*dynamic_cast<const ScannerVectorFramePlaneHopper*>(_o));
			case ScannerVectorTypeHelper::ResonanceBiDi:
				return (_o == nullptr) ? ScannerVectorFrameResonance::Create() : ScannerVectorFrameResonance::Create(*dynamic_cast<const ScannerVectorFrameResonance*>(_o));
			default:
				return (_o==nullptr)?ScannerVectorFrameBasic::Create():ScannerVectorFrameBasic::Create(*dynamic_cast<const ScannerVectorFrameBasic*>(_o));
			}
		}

		std::vector<boost::signals2::connection> ScannerVectorFrameBasic::ConnectCopyTrigger(signalchange_t::slot_type _slot) {
			std::vector<boost::signals2::connection> conns;
			conns.push_back(xres.ConnectOther(_slot));
			conns.push_back(yres.ConnectOther(_slot));
			conns.push_back(zoom.ConnectOther(_slot));
			conns.push_back(xaspectratio.ConnectOther(_slot));
			conns.push_back(yaspectratio.ConnectOther(_slot));
			conns.push_back(squarepixels.ConnectOther(_slot));
			return conns;
		}

		void ScannerVectorFrameBasic::ConnectRateUpdate(signalchange_t::slot_type _slot) {
			xres.ConnectOther(_slot);
			yres.ConnectOther(_slot);
		}

		void ScannerVectorFrameBasic::ConnectResolutionUpdate(signalchange_t::slot_type _slot) {
			xaspectratio.ConnectOther(_slot);
			yaspectratio.ConnectOther(_slot);
		}

		void ScannerVectorFrameBasic::ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) {
			xres.ConnectOther(_slot);
			yres.ConnectOther(_slot);
			zoom.ConnectOther(_slot);
		}

		void ScannerVectorFrameBasic::ConnectResolutionChange(signalchange_t::slot_type _slot) {
			xres.ConnectOther(_slot);
			yres.ConnectOther(_slot);
		}

		void ScannerVectorFrameBasic::ConnectOnlineUpdate(signalchange_t::slot_type _slot) {
			zoom.ConnectOther(_slot);
			fastz.ConnectOther(_slot);
			pockels.ConnectOther(_slot);
			xoffset.ConnectOther(_slot);
			yoffset.ConnectOther(_slot);
			xrotation.ConnectOther(_slot);
			yrotation.ConnectOther(_slot);
		}

		ScannerVectorFrameSaw::ScannerVectorFrameSaw()
			: xcutoff(0.1, 0, 0.5, L"XCutoff_Fraction")
			, ycutoff(0.1, 0, 0.5, L"YCutoff_Fraction")
			, xretrace(0.1, 0, 0.5, L"XRetrace_Fraction")
			, yretrace(0.1, 0, 0.5, L"YRetrace_Fraction") {
		}

		std::vector<boost::signals2::connection> ScannerVectorFrameSaw::ConnectCopyTrigger(signalchange_t::slot_type _slot) {
			std::vector<boost::signals2::connection> conns(ScannerVectorFrameBasic::ConnectCopyTrigger(_slot));
			conns.push_back(xcutoff.ConnectOther(_slot));
			conns.push_back(ycutoff.ConnectOther(_slot));
			conns.push_back(xretrace.ConnectOther(_slot));
			conns.push_back(yretrace.ConnectOther(_slot));
			return conns;
		}

		void ScannerVectorFrameSaw::ConnectRateUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectRateUpdate(_slot);
			xcutoff.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			xretrace.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
		}

		void ScannerVectorFrameSaw::ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectMicronPerPixelUpdate(_slot);
			xcutoff.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			xretrace.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
		}

		void ScannerVectorFrameSaw::ConnectOnlineUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectOnlineUpdate(_slot);
			xcutoff.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			xretrace.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
		}

		ScannerVectorFrameSaw::Preset::Preset()
			: xcutoff(0.1, 0, 0.5, L"XCutoff_Fraction")
			, ycutoff(0.1, 0, 0.5, L"YCutoff_Fraction")
			, xretrace(0.1, 0, 0.5, L"XRetrace_Fraction")
			, yretrace(0.1, 0, 0.5, L"YRetrace_Fraction")  {
		}

		void ScannerVectorFrameSaw::Preset::Load(const wptree& pt)  {
			ScannerVectorFrameBasic::Preset::Load(pt);
			xcutoff.SetFromPropertyTree(pt);
			ycutoff.SetFromPropertyTree(pt);
			xretrace.SetFromPropertyTree(pt);
			yretrace.SetFromPropertyTree(pt);
		}

		void ScannerVectorFrameSaw::Preset::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Preset::Save(pt);
			xcutoff.AddToPropertyTree(pt);
			ycutoff.AddToPropertyTree(pt);
			xretrace.AddToPropertyTree(pt);
			yretrace.AddToPropertyTree(pt);
		}

		void ScannerVectorFrameSaw::Load(const wptree& pt) {
			ScannerVectorFrameBasic::Load(pt);
			xcutoff.SetFromPropertyTree(pt);
			ycutoff.SetFromPropertyTree(pt);
			xretrace.SetFromPropertyTree(pt);
			yretrace.SetFromPropertyTree(pt);
		}

		void ScannerVectorFrameSaw::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Save(pt);
			xcutoff.AddToPropertyTree(pt);
			ycutoff.AddToPropertyTree(pt);
			xretrace.AddToPropertyTree(pt);
			yretrace.AddToPropertyTree(pt);
		}

		void ScannerVectorFrameSaw::SetReadOnlyWhileScanning(const RunState& _runstate) {
			ScannerVectorFrameBasic::SetReadOnlyWhileScanning(_runstate);
			bool enabler = (static_cast<RunStateHelper::Mode>(_runstate)==RunStateHelper::Mode::Stopped)?true:false;
			xcutoff.SetRWState(enabler);
			ycutoff.SetRWState(enabler);
			xretrace.SetRWState(enabler);
			yretrace.SetRWState(enabler);

		}

		std::unique_ptr<ScannerVectorFrameBasic::Preset> ScannerVectorFrameSaw::MakePreset() const {
			return std::make_unique<ScannerVectorFrameBasic::Preset>();
		}

		void ScannerVectorFrameSaw::SaveToPreset(const std::wstring& _name, const Daq& _daq) {
			auto p = std::make_shared<Preset>();
			p->name = _name;
			p->pixeltime = _daq.pixeltime();
			p->scannerdelay = _daq.scannerdelay();
			p->averages = _daq.averages();
			p->xres = xres();
			p->yres = yres();
			p->xcutoff = xcutoff();
			p->ycutoff = ycutoff();
			p->xretrace = xretrace();
			p->yretrace = yretrace();
			auto samename = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if  ( presets.end() != samename )
				presets.erase(samename);				// If name already exists, delete the old (thus overwrite)
			presets.push_back(p);
		}

		void ScannerVectorFrameSaw::LoadFromPreset(const std::wstring& _name, Daq& _daq) {
			auto which = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if ( which == std::end(presets) )
				return ;
	
			_daq.pixeltime = (*which)->pixeltime();
			_daq.scannerdelay = (*which)->scannerdelay();
			_daq.averages = (*which)->averages();
			xres = (*which)->xres();
			yres = (*which)->yres();

			// This is one of the few locations where not everything is compile-time type-safe
			// Casting from ScannerVectorFrameBasic::Preset to ScannerVectorFrameSaw::Preset
			xcutoff = dynamic_cast<ScannerVectorFrameSaw::Preset*>(which->get())->xcutoff();
			ycutoff = dynamic_cast<ScannerVectorFrameSaw::Preset*>(which->get())->ycutoff();
			xretrace = dynamic_cast<ScannerVectorFrameSaw::Preset*>(which->get())->xretrace();
			yretrace = dynamic_cast<ScannerVectorFrameSaw::Preset*>(which->get())->yretrace();
		}

		ScannerVectorFrameBiDi::ScannerVectorFrameBiDi()
			: xturnfraction(0.1, 0, 0.5, L"XTurning_Fraction")
			, ycutoff(0.1, 0, 0.5, L"YCutoff_Fraction")
			, yretrace(0.1, 0, 0.5, L"YRetrace_Fraction") {
		}

		std::vector<boost::signals2::connection> ScannerVectorFrameBiDi::ConnectCopyTrigger(signalchange_t::slot_type _slot) {
			std::vector<boost::signals2::connection> conns(ScannerVectorFrameBasic::ConnectCopyTrigger(_slot));
			conns.push_back(xturnfraction.ConnectOther(_slot));
			conns.push_back(ycutoff.ConnectOther(_slot));
			conns.push_back(yretrace.ConnectOther(_slot));
			return conns;
		}

		void ScannerVectorFrameBiDi::ConnectRateUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectRateUpdate(_slot);
			xturnfraction.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
		}

		void ScannerVectorFrameBiDi::ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectMicronPerPixelUpdate(_slot);
			xturnfraction.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
		}

		void ScannerVectorFrameBiDi::ConnectOnlineUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectOnlineUpdate(_slot);
			xturnfraction.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
		}

		ScannerVectorFrameBiDi::Preset::Preset()
			: xturnfraction(0.1, 0, 0.5, L"XTurning_Fraction")
			, ycutoff(0.1, 0, 0.5, L"YCutoff_Fraction")
			, yretrace(0.1, 0, 0.5, L"YRetrace_Fraction") {
		}

		void ScannerVectorFrameBiDi::Preset::Load(const wptree& pt) {
			ScannerVectorFrameBasic::Preset::Load(pt);
			xturnfraction.SetFromPropertyTree(pt);
			ycutoff.SetFromPropertyTree(pt);
			yretrace.SetFromPropertyTree(pt);
		}

		void ScannerVectorFrameBiDi::Preset::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Preset::Save(pt);
			xturnfraction.AddToPropertyTree(pt);
			ycutoff.AddToPropertyTree(pt);
			yretrace.AddToPropertyTree(pt);
		}

		void ScannerVectorFrameBiDi::Load(const wptree& pt) {
			ScannerVectorFrameBasic::Load(pt);
			xturnfraction.SetFromPropertyTree(pt);
			ycutoff.SetFromPropertyTree(pt);
			yretrace.SetFromPropertyTree(pt);
		}

		void ScannerVectorFrameBiDi::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Save(pt);
			xturnfraction.AddToPropertyTree(pt);
			ycutoff.AddToPropertyTree(pt);
			yretrace.AddToPropertyTree(pt);
		}

		void ScannerVectorFrameBiDi::SetReadOnlyWhileScanning(const RunState& _runstate) {
			ScannerVectorFrameBasic::SetReadOnlyWhileScanning(_runstate);
			bool enabler = (static_cast<RunStateHelper::Mode>(_runstate)==RunStateHelper::Mode::Stopped)?true:false;
			xturnfraction.SetRWState(enabler);
			ycutoff.SetRWState(enabler);
			yretrace.SetRWState(enabler);
		}

		std::unique_ptr<ScannerVectorFrameBasic::Preset> ScannerVectorFrameBiDi::MakePreset() const {
			return std::make_unique<ScannerVectorFrameBasic::Preset>();
		}

		void ScannerVectorFrameBiDi::SaveToPreset(const std::wstring& _name, const Daq& _daq) {
			auto p = std::make_shared<Preset>();
			p->name = _name;
			p->pixeltime = _daq.pixeltime();
			p->scannerdelay = _daq.scannerdelay();
			p->averages = _daq.averages();
			p->xres = xres();
			p->yres = yres();
			p->xturnfraction = xturnfraction();
			p->ycutoff = ycutoff();
			p->yretrace = yretrace();
			auto samename = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if  ( presets.end() != samename )
				presets.erase(samename);				// If name already exists, delete the old (thus overwrite)
			presets.push_back(p);
		}

		void ScannerVectorFrameBiDi::LoadFromPreset(const std::wstring& _name, Daq& _daq) {
			auto which = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if ( which == std::end(presets) )
				return ;
			_daq.pixeltime = (*which)->pixeltime();
			_daq.scannerdelay = (*which)->scannerdelay();
			_daq.averages = (*which)->averages();
			xres = (*which)->xres();
			yres = (*which)->yres();
			// Casting from ScannerVectorFrameBasic::Preset to ScannerVectorFrameBidi::Preset
			xturnfraction = dynamic_cast<ScannerVectorFrameBiDi::Preset*>(which->get())->xturnfraction();
			ycutoff = dynamic_cast<ScannerVectorFrameBiDi::Preset*>(which->get())->ycutoff();
			yretrace = dynamic_cast<ScannerVectorFrameBiDi::Preset*>(which->get())->yretrace();
		}


		ScannerVectorFramePlaneHopper::Preset::Preset()
			: planes(0) {
		}

		std::vector<boost::signals2::connection> ScannerVectorFramePlaneHopper::ConnectCopyTrigger(signalchange_t::slot_type _slot) {
			std::vector<boost::signals2::connection> conns(ScannerVectorFrameBasic::ConnectCopyTrigger(_slot));
			for ( auto& p : planes ) {
				conns.push_back(p.pockels.ConnectOther(_slot));
				conns.push_back(p.position.ConnectOther(_slot));
			}

			return conns;
		}

		void ScannerVectorFramePlaneHopper::ConnectRateUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectRateUpdate(_slot);
			for ( auto& p : planes ) {
				p.pockels.ConnectOther(_slot);
				p.position.ConnectOther(_slot);
			}
		}

		void ScannerVectorFramePlaneHopper::ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectMicronPerPixelUpdate(_slot);
			for ( auto& p : planes ) {
				p.pockels.ConnectOther(_slot);
				p.position.ConnectOther(_slot);
			}
		}

		void ScannerVectorFramePlaneHopper::ConnectOnlineUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectOnlineUpdate(_slot);
			for ( auto& p : planes ) {
				p.pockels.ConnectOther(_slot);
				p.position.ConnectOther(_slot);
			}
		}

		void ScannerVectorFramePlaneHopper::Preset::Load(const wptree& pt) {
			ScannerVectorFrameBasic::Preset::Load(pt);
	
		}

		void ScannerVectorFramePlaneHopper::Preset::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Preset::Save(pt);
	
		}

		ScannerVectorFramePlaneHopper::ScannerVectorFramePlaneHopper()
			: planes(0) {
		}

		void ScannerVectorFramePlaneHopper::Load(const wptree& pt) {
			ScannerVectorFrameBasic::Load(pt);
			try {
				// Load until get_child throws
				for ( uint32_t p = 0 ; p < 100 ; p++ ) {
					PlaneProperties pl;
					pl.Load(pt.get_child(boost::str(boost::wformat(L"Plane%d") % p)));
					planes.push_back(pl);
				}
			}
			catch (...) { }
		}

		void ScannerVectorFramePlaneHopper::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Save(pt);
			uint32_t i = 0;
			for ( const auto& pl : planes ) {
				wptree wt;
				pl.Save(wt);
				pt.add_child(boost::str(boost::wformat(L"Plane%d") % i++), wt);
			}
		}

		void ScannerVectorFramePlaneHopper::SetReadOnlyWhileScanning(const RunState& _runstate) {
			ScannerVectorFrameBasic::SetReadOnlyWhileScanning(_runstate);
		}

		std::unique_ptr<ScannerVectorFrameBasic::Preset> ScannerVectorFramePlaneHopper::MakePreset() const {
			return std::make_unique<ScannerVectorFrameBasic::Preset>();
		}

		void ScannerVectorFramePlaneHopper::SaveToPreset(const std::wstring& _name, const Daq& _daq) {
			auto p = std::make_shared<Preset>();
			p->name = _name;
			p->pixeltime = _daq.pixeltime();
			p->scannerdelay = _daq.scannerdelay();
			p->averages = _daq.averages();
			p->xres = xres();
			p->yres = yres();
			p->planes = planes;
			auto samename = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if  ( presets.end() != samename )
				presets.erase(samename);				// If name already exists, delete the old (thus overwrite)
			presets.push_back(p);
		}
	
	
		void ScannerVectorFramePlaneHopper::LoadFromPreset(const std::wstring& _name, Daq& _daq) {
			auto which = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if ( which == std::end(presets) )
				return ;
			_daq.pixeltime = (*which)->pixeltime();
			_daq.scannerdelay = (*which)->scannerdelay();
			_daq.averages = (*which)->averages();
			xres = (*which)->xres();
			yres = (*which)->yres();
			// Casting from ScannerVectorFrameBasic::Preset to ScannerVectorFramePlaneHopper::Preset
			planes = dynamic_cast<ScannerVectorFramePlaneHopper::Preset*>(which->get())->planes;
		}


		ScannerVectorFrameResonance::ScannerVectorFrameResonance()
			: planes(0)
			, xturnfraction(0.1, 0, 0.5, L"XTurning_Fraction")
			, ycutoff(0.1, 0, 0.5, L"YCutoff_Fraction")
			, yretrace(0.1, 0, 0.5, L"YRetrace_Fraction")
			, waitafterenqueuestorage(2, 0, 1000, L"WaitAfterEnqueueStorage_Millisecond")
			, waitafterenqueuedisplay(5, 0, 1000, L"WaitAfterEnqueueDisplay_Millisecond") {
			xres.ConnectOther(std::bind(&ScannerVectorFrameResonance::UpdateXTurnFraction, this));
			yres.ConnectOther(std::bind(&ScannerVectorFrameResonance::CoerceYResolution, this));
			zoom.SetLimits(1, 4);
		}

		std::vector<boost::signals2::connection> ScannerVectorFrameResonance::ConnectCopyTrigger(signalchange_t::slot_type _slot) {
			std::vector<boost::signals2::connection> conns(ScannerVectorFrameBasic::ConnectCopyTrigger(_slot));
			conns.push_back(xturnfraction.ConnectOther(_slot));
			conns.push_back(ycutoff.ConnectOther(_slot));
			conns.push_back(yretrace.ConnectOther(_slot));
			conns.push_back(waitafterenqueuestorage.ConnectOther(_slot));
			conns.push_back(waitafterenqueuedisplay.ConnectOther(_slot));
			return conns;
		}

		void ScannerVectorFrameResonance::ConnectRateUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectRateUpdate(_slot);
			xturnfraction.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
			waitafterenqueuestorage.ConnectOther(_slot);
			waitafterenqueuedisplay.ConnectOther(_slot);
		}

		void ScannerVectorFrameResonance::ConnectMicronPerPixelUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectMicronPerPixelUpdate(_slot);
			xturnfraction.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
			waitafterenqueuestorage.ConnectOther(_slot);
			waitafterenqueuedisplay.ConnectOther(_slot);
		}

		void ScannerVectorFrameResonance::ConnectOnlineUpdate(signalchange_t::slot_type _slot) {
			ScannerVectorFrameBasic::ConnectOnlineUpdate(_slot);
			xturnfraction.ConnectOther(_slot);
			ycutoff.ConnectOther(_slot);
			yretrace.ConnectOther(_slot);
			// No online update of waitafters possible
		}

		ScannerVectorFrameResonance::Preset::Preset()
			: xturnfraction(0.1, 0, 0.5, L"XTurning_Fraction")
			, ycutoff(0.1, 0, 0.5, L"YCutoff_Fraction")
			, yretrace(0.1, 0, 0.5, L"YRetrace_Fraction")
			, waitafterenqueuestorage(2, 0, 1000, L"WaitAfterEnqueueStorage_Millisecond")
			, waitafterenqueuedisplay(5, 0, 1000, L"WaitAfterEnqueueDisplay_Millisecond") {
		}

		void ScannerVectorFrameResonance::Preset::Load(const wptree& pt) {
			ScannerVectorFrameBasic::Preset::Load(pt);
			xturnfraction.SetFromPropertyTree(pt);
			ycutoff.SetFromPropertyTree(pt);
			yretrace.SetFromPropertyTree(pt);
			waitafterenqueuestorage.SetFromPropertyTree(pt);
			waitafterenqueuedisplay.SetFromPropertyTree(pt);
		}

		void ScannerVectorFrameResonance::Preset::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Preset::Save(pt);
			xturnfraction.AddToPropertyTree(pt);
			ycutoff.AddToPropertyTree(pt);
			yretrace.AddToPropertyTree(pt);
			waitafterenqueuestorage.AddToPropertyTree(pt);
			waitafterenqueuedisplay.AddToPropertyTree(pt);
		}

		void ScannerVectorFrameResonance::Load(const wptree& pt) {
			ScannerVectorFrameBasic::Load(pt);
			xturnfraction.SetFromPropertyTree(pt);
			ycutoff.SetFromPropertyTree(pt);
			yretrace.SetFromPropertyTree(pt);
			waitafterenqueuestorage.SetFromPropertyTree(pt);
			waitafterenqueuedisplay.SetFromPropertyTree(pt);

			// Catch possible exceptions from Load, continue unharmed after
			try {
				uint32_t p = 0;
				// Load complete planes until no more found (which causes an exception)
				while (true) {
					const wptree planetree = pt.get_child(boost::str(boost::wformat(L"plane%d") % p));
					// Load single area planes
					for ( uint32_t a = 0 ; a < config::totalareas ; a++ ) {
						PlaneProperties plane;
						plane.Load(planetree.get_child(boost::str(boost::wformat(L"area%d") % a)));
					}
					p++;
				}
			}
			catch (...) { // Just catch
			}
		}

		void ScannerVectorFrameResonance::Save(wptree& pt) const {
			ScannerVectorFrameBasic::Save(pt);
			xturnfraction.AddToPropertyTree(pt);
			ycutoff.AddToPropertyTree(pt);
			yretrace.AddToPropertyTree(pt);
			waitafterenqueuestorage.AddToPropertyTree(pt);
			waitafterenqueuedisplay.AddToPropertyTree(pt);

			// Go through all complete planes
			for ( const auto& p : planes ) {
				p.position.AddToPropertyTree(pt);
				p.pockels.AddToPropertyTree(pt);
			}
		}

		void ScannerVectorFrameResonance::SetReadOnlyWhileScanning(const RunState& _runstate) {
			ScannerVectorFrameBasic::SetReadOnlyWhileScanning(_runstate);
			bool enabler = (static_cast<RunStateHelper::Mode>(_runstate)==RunStateHelper::Mode::Stopped)?true:false;
			ycutoff.SetRWState(enabler);
			yretrace.SetRWState(enabler);
			zoom.SetRWState(enabler);
			waitafterenqueuestorage.SetRWState(enabler);
			waitafterenqueuedisplay.SetRWState(enabler);
		}

		std::unique_ptr<ScannerVectorFrameBasic::Preset> ScannerVectorFrameResonance::MakePreset() const {
			return std::make_unique<ScannerVectorFrameBasic::Preset>();
		}

		void ScannerVectorFrameResonance::SaveToPreset(const std::wstring& _name, const Daq& _daq) {
			auto p = std::make_shared<Preset>();
			p->name = _name;
			p->pixeltime = _daq.pixeltime();
			p->scannerdelay = _daq.scannerdelay();
			p->averages = _daq.averages();
			p->xres = xres();
			p->yres = yres();
			p->xturnfraction = xturnfraction();
			p->ycutoff = ycutoff();
			p->yretrace = yretrace();
			p->waitafterenqueuestorage = waitafterenqueuestorage();
			p->waitafterenqueuedisplay = waitafterenqueuedisplay();
			auto samename = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if  ( presets.end() != samename )
				presets.erase(samename);				// If name already exists, delete the old (thus overwrite)
			presets.push_back(p);
		}

		void ScannerVectorFrameResonance::LoadFromPreset(const std::wstring& _name, Daq& _daq) {
			auto which = std::find_if(std::begin(presets), std::end(presets), [&](std::shared_ptr<ScannerVectorFrameBasic::Preset> _pres)
				{ return _pres->name() == _name; } );
			if ( which == std::end(presets) )
				return ;
			_daq.pixeltime = (*which)->pixeltime();
			_daq.scannerdelay = (*which)->scannerdelay();
			_daq.averages = (*which)->averages();
			xres = (*which)->xres();
			yres = (*which)->yres();
			// Casting from ScannerVectorFrameBasic::Preset to ScannerVectorFrameResonance::Preset
			xturnfraction = dynamic_cast<ScannerVectorFrameResonance::Preset*>(which->get())->xturnfraction();
			ycutoff = dynamic_cast<ScannerVectorFrameResonance::Preset*>(which->get())->ycutoff();
			yretrace = dynamic_cast<ScannerVectorFrameResonance::Preset*>(which->get())->yretrace();
			waitafterenqueuestorage = dynamic_cast<ScannerVectorFrameResonance::Preset*>(which->get())->waitafterenqueuestorage();
			waitafterenqueuedisplay = dynamic_cast<ScannerVectorFrameResonance::Preset*>(which->get())->waitafterenqueuedisplay();
		}

		void ScannerVectorFrameResonance::CoerceYResolution() {
			// only even values are allowed, because the image gets divided into two parts (forward/backward lines)
			yres.Set(yres - yres%2);
		}

		void ScannerVectorFrameResonance::UpdateXTurnFraction() {
			xturnfraction.Set(static_cast<double>((XTotalPixels() - XImagePixels())) / XImagePixels());
		}


	}

}