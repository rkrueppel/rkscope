#pragma once

#include "config\config_choices.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"
#include "Framescan.h"
#include "Devices.h"
#include "IO.h"

namespace scope {
	namespace parameters {

		using boost::property_tree::wptree;

		/** Parameters for a whole area (includes a daq and a fpu)
		* @ingroup ScopeParameters */
		class BaseArea
			: public Base {

		public:
			ScopeValue<AreaType> areatype;
			
			ScopeNumber<uint32_t> area;
			
			/** the DaqParameters for this area */
			Daq daq;

			/** the xy stage parameters for this FPU (set type in ScopeDefines.h) */
			config::FPUXYStageParametersType fpuxystage;

			/** the fast z stage parameters for this FPU (set type in ScopeDefines.h) */
			config::FPUZStageParametersType fpuzstage;

			/** the choosen scanner vector type */
			ScopeValue<ScannerVectorType> scanmode;
			
			/** This map contains all ScannerVectors that are supported by the kind of scanner */
			std::map<ScannerVectorTypeHelper::Mode, std::unique_ptr<ScannerVectorFrameBasic>> scannervectorframesmap;

			/** @name Base scale for 256x256 pixels at zoom 1 and the (maxoutput-minoutput) range set in Daq
			* @{ */
			/** Base scale in x direction for 256x256 pixels at zoom 1 and the (maxoutput-minoutput) range set in Daq */
			ScopeNumber<double> basemicronperpixelx;
			/** Base scale in y direction for 256x256 pixels at zoom 1 and the (maxoutput-minoutput) range set in Daq */
			ScopeNumber<double> basemicronperpixely;
			/** @} */

			/** @name Current scale for current resolution and zoom
			* @{ */
			ScopeNumber<double> micronperpixelx;
			ScopeNumber<double> micronperpixely;
			/** @} */

			/** Line repetition rate in Hertz */
			ScopeNumber<double> linerate;

			/** Frame repetition rate in Hertz */
			ScopeNumber<double> framerate;

			/** Time per frame in seconds */
			ScopeNumber<double> frametime;

			/** Histogram range for the areas */
			ScopeNumber<uint32_t> histrange;

			/** Default constructor. Fills scannervectorframesmap and initializes connections inside MasterArea. */
			BaseArea(const uint32_t& _area, const AreaType& _at);

			/** Copy constructor (deep copy because of the pointers in the map) */
			BaseArea(const BaseArea& _other);

			/** Assignment (deep copy because of the pointers in the map) */
			BaseArea& operator=(const BaseArea& v);

			/** Virtual destructor, just in case we derive a something from Area somtime */
			virtual ~BaseArea() { }

			/** pointer to the current parameters::frame. */
			ScannerVectorFrameBasic& Currentframe() const;

			virtual void GetFrame(ScannerVectorFrameSaw* _svf) const {
				_svf = dynamic_cast<ScannerVectorFrameSaw*>(scannervectorframesmap.at(ScannerVectorTypeHelper::Sawtooth).get());
			}

			virtual void GetFrame(ScannerVectorFrameBiDi* _svf) const {
				_svf = dynamic_cast<ScannerVectorFrameBiDi*>(scannervectorframesmap.at(ScannerVectorTypeHelper::Bidirectional).get());
			}

			virtual void GetFrame(ScannerVectorFrameResonance* _svf) const {
				_svf = dynamic_cast<ScannerVectorFrameResonance*>(scannervectorframesmap.at(ScannerVectorTypeHelper::ResonanceBiDi).get());
			}
			virtual void GetFrame(ScannerVectorFramePlaneHopper* _svf) const {
				_svf = dynamic_cast<ScannerVectorFramePlaneHopper*>(scannervectorframesmap.at(ScannerVectorTypeHelper::Planehopper).get());
			}

			virtual ScannerVectorFrameSaw* FrameSaw() const {
				return dynamic_cast<ScannerVectorFrameSaw*>(scannervectorframesmap.at(ScannerVectorTypeHelper::Sawtooth).get());
			}

			virtual ScannerVectorFrameBiDi* FrameBiDi() const {
				return dynamic_cast<ScannerVectorFrameBiDi*>(scannervectorframesmap.at(ScannerVectorTypeHelper::Bidirectional).get());
			}

			virtual ScannerVectorFrameResonance*  FrameResonance() const {
				return dynamic_cast<ScannerVectorFrameResonance*>(scannervectorframesmap.at(ScannerVectorTypeHelper::ResonanceBiDi).get());
			}

			virtual ScannerVectorFramePlaneHopper* FrameHopper() const {
				return dynamic_cast<ScannerVectorFramePlaneHopper*>(scannervectorframesmap.at(ScannerVectorTypeHelper::Planehopper).get());
			}


			void Load(const wptree& pt) override;
			void Save(wptree& pt) const override;
			void SetReadOnlyWhileScanning(const RunState& _runstate) override;

			/** Save to preset in currentframe */
			virtual void SaveToPreset(const std::wstring& _name);

			/** Load from preset in currentframe */
			virtual void LoadFromPreset(const std::wstring& _name);

			/** Delete preset in currentframe */
			virtual void DeletePreset(const std::wstring& _name);

			/** Time per frame in seconds*/
			virtual double FrameTime() const;

			/** Time per line in seconds */
			virtual double LineTime() const;

			/** total number of pixels summed over all channels	 */
			virtual uint32_t TotalPixelsAllChannels() const;

			/** Gives the current framescan X offset in micrometers */
			virtual double XOffsetInMicron() const;

			/** Gives the current framescan Y offset in micrometers */
			virtual double YOffsetInMicron() const;

		protected:
			/** Helper for constructors and assignment to connect internal ScopeValues (as the connections in ScopeValues do not
			* get copied automatically */
			virtual void InitializeConnections();

			/** Force update of rates etc. after change of scanmode. scanmode is connected to this. */
			virtual void ChangeScanMode();

			/** Updates framerate, frametime, and linerate */
			virtual void UpdateRates();

			/** Updates fast z boundaries on changed fast Z (ETL) calibration file */
			virtual void UpdateFastZCalibration();

			/** Current scale calculated from micronperpixelx with the resolution set in currentframe */
			virtual void CalculateMicronPerPixel();

			/** Calculates the x resolution from the x aspect ratio and the y resolution from the y aspect ratio */
			virtual void CalculateResolution();
		};

		class MasterArea
			: public BaseArea {
		public:
			MasterArea(const uint32_t& _area)
				: BaseArea(_area, AreaTypeHelper::Master) {}
		};

		class SlaveArea
			: public BaseArea {

		public:
			/** const pointer to the master area parameters (provided in the constructor through parametes::Scope) */
			MasterArea* masterarea;

			/** @param[in] _area the area number
			* @param[in] _isslave is this a slave area?
			* @param[in] _masterarea pointer to the masterarea parameters, or nullptr */
			SlaveArea(const uint32_t& _area, MasterArea * const _itsmasterarea = nullptr);

			/** Copy constructor (deep copy because of the pointers in the map). Attention: masterarea pointer is not copied! */
			SlaveArea(const SlaveArea& _other);

			/** Assignment (deep copy because of the pointers in the map). Attention: masterarea pointer is not copied! */
			SlaveArea& operator=(const SlaveArea& v);

			/** Virtual destructor, just in case we derive a something from Area somtime */
			virtual ~SlaveArea() { }

			/** (Re)set the pointer to the master area e.g. after a copy */
			void SetMasterArea(MasterArea* const _masterarea);

			/** Copies parts of the ScannerVectorParameters from the master area's. Keeps Pockels and fast z */
			void CopyFromMasterArea();

		protected:
			/** Helper for constructors and assignment to connect internal ScopeValues (as the connections in ScopeValues do not
			* get copied automatically */
			virtual void InitializeConnections();

			/** Force update of rates etc. after change of scanmode. scanmode is connected to this. */
			virtual void ChangeScanMode();
		};

	}
}