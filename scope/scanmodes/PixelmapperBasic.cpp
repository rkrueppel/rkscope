#include "stdafx.h"
#include "parameters/Scope.h"
#include "PixelmapperBasic.h"
#include "PixelmapperFrameSaw.h"
#include "PixelmapperFrameBiDi.h"
#include "PixelmapperFrameResonanceSW.h"
#include "PixelmapperFrameResonanceHW.h"
#include "helpers/DaqChunk.h"
#include "ScopeDatatypes.h"
#include "helpers/ScopeMultiImage.h"

namespace scope {

PixelmapperBasic::PixelmapperBasic(const ScannerType& _scanner, const ScannerVectorType& _type)
	: type(_type)
	, current_frame(nullptr)
	, svparameters(parameters::ScannerVectorFrameBasic::Factory(_type).release())
	, lookup(nullptr) {
}

PixelmapperBasic::~PixelmapperBasic() {

}

std::unique_ptr<PixelmapperBasic> PixelmapperBasic::Factory(const ScannerType& _scanner, const ScannerVectorType& _type) {
	switch ( (ScannerTypeHelper::Mode)_scanner ) {
	case ScannerTypeHelper::Regular:
		switch ( (ScannerVectorTypeHelper::Mode)_type ) {
		case ScannerVectorTypeHelper::Bidirectional:
			return std::unique_ptr<PixelmapperFrameBiDi>(new PixelmapperFrameBiDi());
		// Not yet implemented
		//case ScannerVectorTypeHelper::Planehopper:
		//	return std::unique_ptr<PixelmapperBasic>(new PixelmapperBasic());
		
		default:
			return std::unique_ptr<PixelmapperFrameSaw>(new PixelmapperFrameSaw());
		}
		break;
	case ScannerTypeHelper::Resonance:
		switch ( (ScannerVectorTypeHelper::Mode)_type ) {
		case ScannerVectorTypeHelper::ResonanceBiDi:
			return std::unique_ptr<SCOPE_RESONANCEPIXELMAPPER_T>(new SCOPE_RESONANCEPIXELMAPPER_T());
		}
		break;
	}
}

void PixelmapperBasic::SetCurrentFrame(ScopeMultiImagePtr const _current_frame) {
	current_frame = _current_frame;
}

void PixelmapperBasic::SetLookupVector(const std::vector<std::size_t>* const _lookup) {
	lookup = _lookup;
	lastlookup = std::begin(*lookup);
}

void PixelmapperBasic::SetParameters(parameters::ScannerVectorFrameBasic* const _svparameters) {
	this->svparameters = _svparameters;
}

PixelmapperResult PixelmapperBasic::LookupChunk(DaqChunkPtr const _chunk, const uint16_t& _currentavgcount) {
	PixelmapperResult result(Nothing);
	return result;
}

PixelmapperResult PixelmapperBasic::LookupChunk(DaqChunkResonancePtr const _chunk, const uint16_t& _currentavgcount) {
	PixelmapperResult result(Nothing);
	return result;
}

}