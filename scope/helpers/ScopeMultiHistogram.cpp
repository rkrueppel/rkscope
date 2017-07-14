#include "StdAfx.h"
#include "ScopeMultiHistogram.h"
#include "ScopeHistogram.h"
#include "ScopeMultiImage.h"

namespace scope {

ScopeMultiHistogram::ScopeMultiHistogram(const uint32_t& _area, const uint32_t& _channels, const uint32_t& _no_of_bins, uint16_t  _range)
	: area(_area)
	, channels(_channels)
	, hists(channels) {
	std::generate(std::begin(hists), std::end(hists), [&]() {
		return std::make_shared<ScopeHistogram>(_no_of_bins, _range); } );
}

void ScopeMultiHistogram::Calculate(ScopeMultiImageCPtr const _multi, const bool& _loghistogram) {
	assert(hists.size() == _multi->Channels());
	for ( size_t c = 0 ; c < hists.size() ; c++ )
		hists.at(c)->Calculate(_multi->GetChannel(c), _loghistogram);
}

void ScopeMultiHistogram::Resize(const uint32_t& _no_of_bins) {
	std::for_each(std::begin(hists), std::end(hists), [&](ScopeHistogramPtr h) {
		h->Resize(_no_of_bins); } );
}

void ScopeMultiHistogram::UpdateRange(uint16_t&  _range) {
	std::for_each(std::begin(hists), std::end(hists), [&](ScopeHistogramPtr h) {
		h->UpdateRange(_range); } );
}

std::vector<uint32_t> ScopeMultiHistogram::MaxCounts() const {
	std::vector<uint32_t> counts(hists.size(), 0);
	std::transform(std::begin(counts), std::end(counts), std::begin(hists), std::begin(counts), [](const uint32_t& c, ScopeHistogramPtr h) {
		return h->MaxCount(); } );
	return counts;
}

std::vector<uint16_t> ScopeMultiHistogram::FirstCountPositions() const {
	std::vector<uint16_t> positions(hists.size(), 0);
	std::transform(std::begin(positions), std::end(positions), std::begin(hists), std::begin(positions), [](const uint32_t& c, ScopeHistogramPtr h) {
		return h->FirstCountPosition(); } );
	return positions;
}

std::vector<uint16_t> ScopeMultiHistogram::LastCountPositions() const {
	std::vector<uint16_t> positions(hists.size(), 0);
	std::transform(std::begin(positions), std::end(positions), std::begin(hists), std::begin(positions), [](const uint32_t& c, ScopeHistogramPtr h) {
		return h->LastCountPosition(); } );
	return positions;
}

const std::vector<uint32_t>* ScopeMultiHistogram::GetHistConst(const uint32_t& _c) const {
	return hists.at(_c)->GetHistConst();
}

void ScopeMultiHistogram::ReleaseHistConst(const uint32_t& _c) const  {
	hists.at(_c)->ReleaseHistConst();
}

}