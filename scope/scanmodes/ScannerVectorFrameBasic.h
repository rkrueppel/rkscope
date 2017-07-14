#pragma once

#include "parameters/Scope.h"
#include "ScopeDatatypes.h"

namespace scope {

/** Parent class for frame scans. When you construct a ScannerVectorFrame object you have to supply to fundamental parameters, ScannerVectorType 
* and ScannerVectorFillType. The ScannerVectorType describes what kind of frame scan you do in your derived class, e.g. sawtooth or bidirectional. 
* The ScannerVectorFillType determines how the signals for xyzp are actually filled into the datavector that is later on written to the hardware.
* You could be putting out complette frames (FullframeXYZP), use a pixel and a line clock (LineXPColumnYZ), or have a slave area without its own
* scanners (LineZP). Your derived class has to be able to generate signals for all of these cases! */
class ScannerVectorFrameBasic {

protected:
	/** Type/scan mode of this vector */
	const ScannerVectorTypeHelper::Mode type;

	/** The fill type */
	const ScannerVectorFillTypeHelper::Mode filltype;

	/** iterator definition, just handy */
	typedef std::vector<int16_t>::iterator iterator;

	/** current daq parameter set */
	parameters::Daq* daqparameters;
	
	/** current scanner vector parameter set (needs to be pointer for dynamic_cast in derived classes and because the parameters object is localized in ScopeController) */
	parameters::ScannerVectorFrameBasic* svparameters;

	/** current fast z parameter set */
	parameters::SCOPE_FPUZCONTROL_T* zparameters;

	/** the actual scanner vector (if fullframevector) with x, y, fast z, Pockels either interleaved (for fullframevector)
	* or first x,p interleaved and then y,z interleaved (for not fullframevector). */
	std::unique_ptr<std::vector<int16_t>> vecptr;

	/** gives the position in the image vector for each position in the acquired data vector (keep in mind that the acquired data is read in chunks). */
	std::unique_ptr<std::vector<std::size_t>> lookup;

	/** how much is the current lookup vector rotated to adjust for scannerdelay */
	int32_t lookup_rotation;

	/** Calculate the scanner vector based on the current parameters */
	virtual void UpdateVector();

public:
	/** Initialize data vector
	* @param[in] _type Type of scanner vector, set when derived class calls base constructor. Used to generate a fitting parameters set via  parameters::ScannerVectorFrameBasic::Factory.
	* @param[in] _filltype type of vector fill, see GetInterleavedVector for details */
	ScannerVectorFrameBasic(const ScannerVectorType& _type, const ScannerVectorFillType& _filltype);

	/** We need a virtual destructor here, so that derived types get correctly destroyed */
	virtual ~ScannerVectorFrameBasic();

	/** Set current parameters and update vector */
	virtual void SetParameters(parameters::Daq* const _daqparameters, parameters::ScannerVectorFrameBasic* const _svparameters, parameters::SCOPE_FPUZCONTROL_T* const _zparameters);

	/** Set current pockels value and update. Do we need these set methods? Is it not automagically set in AreaParameters?! */
	virtual void SetPockels(const double& _pockelsval);

	/** Set current zoom factor and update */
	virtual void SetZoom(const double& _zoom);

	/** Set the current position for the fast z control */
	virtual void SetFastZ(const double& _fastz);

	/** Set the scannerdelay, rotates the lookup vector
	* @param[in] _scannerdelaysamples the scannerdelay in samples */
	virtual void SetScannderdelay(const uint32_t& _scannerdelaysamples);

	/** @return a reference to the const scanner vector, the content of what you get depends on the fill type (see ScannerVectorFillTypeHelper)
	* - FullframeXYZP: xyzp interleaved, total size is one complete frame
	* - LineXPColumnYZ: xp interleaved and then yz interleaved, total size is one line plus one column
	* - LineZP: zp interleaved, total size is one line */
	virtual std::vector<int16_t>* GetInterleavedVector() const;

	/** @return a reference to the lookup vector */
	virtual std::vector<size_t>* GetLookupVector() const;

	/** @return a pointer to the scanner vector parameters */
	virtual parameters::ScannerVectorFrameBasic* GetSVParameters() const;

	/** @return the fill type of the scanner vector */
	ScannerVectorFillType FillType() const { return filltype; }

public:
	/** A static factory method for scan vectors */
	static std::unique_ptr<ScannerVectorFrameBasic> Factory(const ScannerVectorType& _type, const ScannerVectorFillType& _filltype);

};

/** A shared pointer to a ScannerVectorFrameBasic */
typedef std::shared_ptr<ScannerVectorFrameBasic> ScannerVectorFrameBasicPtr;


}