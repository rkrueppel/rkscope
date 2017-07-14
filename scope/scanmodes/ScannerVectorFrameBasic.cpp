#include "stdafx.h"
#include "ScannerVectorFrameBasic.h"
#include "ScannerVectorFrameSaw.h"
#include "ScannerVectorFrameBiDi.h"
#include "ScannerVectorFramePlaneHopper.h"
#include "ScannerVectorFrameResonanceBiDi.h"
#include "ScannerVectorFrameResonanceHopper.h"

namespace scope {

ScannerVectorFrameBasic::ScannerVectorFrameBasic(const ScannerVectorType& _type, const ScannerVectorFillType& _filltype)
	: type(_type)
	, filltype(_filltype)
	, daqparameters(nullptr)
	, svparameters(parameters::ScannerVectorFrameBasic::Factory(_type).release())
	, zparameters(nullptr)
	, lookup_rotation(0) {
	vecptr = std::unique_ptr<std::vector<int16_t>>(new std::vector<int16_t>(svparameters->TotalPixels() * 4));		// for x, y, fast z, Pockels
	lookup = std::unique_ptr<std::vector<std::size_t>>(new std::vector<std::size_t>(svparameters->TotalPixels()));
	UpdateVector();
}

ScannerVectorFrameBasic::~ScannerVectorFrameBasic() {

}

std::unique_ptr<ScannerVectorFrameBasic> ScannerVectorFrameBasic::Factory(const ScannerVectorType& _type, const ScannerVectorFillType& _filltype) {
	switch ( _type.t ) {
	case ScannerVectorTypeHelper::Sawtooth:
		return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameSaw(_filltype));
	case ScannerVectorTypeHelper::Bidirectional:
		return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameBiDi(_filltype));
	case ScannerVectorTypeHelper::Planehopper:
		return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFramePlaneHopper(_filltype));
	case ScannerVectorTypeHelper::ResonanceBiDi:
		return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameResonanceBiDi(_filltype));
	case ScannerVectorTypeHelper::ResonanceHopper:
		return std::unique_ptr<ScannerVectorFrameBasic>(new ScannerVectorFrameResonanceHopper(_filltype));

	}
}

void ScannerVectorFrameBasic::UpdateVector() {
}

void ScannerVectorFrameBasic::SetParameters(parameters::Daq* const _daqparameters, parameters::ScannerVectorFrameBasic* const _svparameters, parameters::SCOPE_FPUZCONTROL_T* const _zparameters) {
	this->daqparameters = _daqparameters;
	this->svparameters = _svparameters;
	this->zparameters = _zparameters;
	UpdateVector();
}

void ScannerVectorFrameBasic::SetPockels(const double& _pockelsval) {
	svparameters->pockels.Set(_pockelsval);
	UpdateVector();
}

void ScannerVectorFrameBasic::SetZoom(const double& _zoom) {
	svparameters->zoom.Set(_zoom);
	UpdateVector();
}

void ScannerVectorFrameBasic::SetFastZ(const double& _fastz) {
	svparameters->fastz.Set(_fastz);
	UpdateVector();
}

void ScannerVectorFrameBasic::SetScannderdelay(const uint32_t& _scannerdelaysamples) {
	const uint32_t newrot = _scannerdelaysamples - lookup_rotation;				// how far do we have to rotate now (taking into account earlier rotation);
	if ( newrot > 0 )
		std::rotate(std::begin(*lookup), std::begin(*lookup)+newrot, std::end(*lookup));
	if ( newrot < 0 )
		std::rotate(std::begin(*lookup), std::end(*lookup)+newrot, std::end(*lookup));
	lookup_rotation = _scannerdelaysamples;
}

const std::vector<int16_t>* ScannerVectorFrameBasic::GetInterleavedVector() const {
	return vecptr.get();
}

const std::vector<std::size_t>* ScannerVectorFrameBasic::GetLookupVector() const {
	return lookup.get();
}

parameters::ScannerVectorFrameBasic* ScannerVectorFrameBasic::GetSVParameters() const {
	return svparameters;
}



}