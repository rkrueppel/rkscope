#include "stdafx.h"

#include "ScopeDatatypes.h"
#include "parameters\Runstates.h"

namespace scope {

uint16_t Uint16LowerBoundary(const Uint16Range& r) {
	switch ( r.t ) {
		case Uint16RangeHelper::full:
		case Uint16RangeHelper::firsthalf:
		case Uint16RangeHelper::first4096:
			return 0;
		case Uint16RangeHelper::secondhalf:
			return 32768;
		default:
			return 0;
	}
}

uint16_t Uint16UpperBoundary(const Uint16Range& r) {
	switch ( r.t ) {
		case Uint16RangeHelper::full:
		case Uint16RangeHelper::secondhalf:
			return UINT16_MAX;
		case Uint16RangeHelper::firsthalf:
			return 32767;
		case Uint16RangeHelper::first4096:
			return 4095;
		default:
			return UINT16_MAX;
	}
}

std::vector<ScannerVectorTypeHelper::Mode> ScannerSupportedVectors::List(const config::ScannerEnum& _scannertype) {
	switch (_scannertype) {
		default:
		case config::ScannerEnum::RegularGalvo:
			{ ScannerVectorTypeHelper::Mode ret[] = {ScannerVectorTypeHelper::Mode::Sawtooth, ScannerVectorTypeHelper::Mode::Bidirectional };
			return std::vector<ScannerVectorTypeHelper::Mode>(ret, ret + sizeof(ret) / sizeof(ret[0]) ); }
		case config::ScannerEnum::ResonantGalvo:
			{ ScannerVectorTypeHelper::Mode ret[] = {ScannerVectorTypeHelper::Mode::ResonanceBiDi, ScannerVectorTypeHelper::Mode::ResonanceHopper };
			return std::vector<ScannerVectorTypeHelper::Mode>(ret, ret + sizeof(ret) / sizeof(ret[0]) ); }
		case config::ScannerEnum::AOD:
			{ ScannerVectorTypeHelper::Mode ret[] = {ScannerVectorTypeHelper::Mode::Basic};
			return std::vector<ScannerVectorTypeHelper::Mode>(ret, ret + sizeof(ret) / sizeof(ret[0]) ); }
		case config::ScannerEnum::Fibre:
			{ ScannerVectorTypeHelper::Mode ret[] = {ScannerVectorTypeHelper::Mode::Basic};
			return std::vector<ScannerVectorTypeHelper::Mode>(ret, ret + sizeof(ret) / sizeof(ret[0]) ); }
	}
}


bool ScannerSupportedVectors::IsSupported(const ScannerVectorTypeHelper::Mode& _scanmode, const config::ScannerEnum& _scannertype) {
	auto veclist = ScannerSupportedVectors::List(_scannertype);
	return (std::end(veclist) != std::find(std::begin(veclist), std::end(veclist), _scanmode));
}

bool ScannerSupportedVectors::IsBuiltinSupported(const ScannerVectorTypeHelper::Mode& _scanmode) {
	return IsSupported(_scanmode, config::scannerselect);
}

}
