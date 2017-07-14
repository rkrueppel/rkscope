#include "stdafx.h"
#include "ScopeMultiImage.h"
#include "ScopeImage.h"

namespace scope {

ScopeMultiImage::ScopeMultiImage(const uint32_t& _area, const size_t& _nochannels, const uint32_t& _lines, const uint32_t& _linewidth)
	: area(_area)
	, nochannels(_nochannels)
	, lines(_lines)
	, linewidth(_linewidth) 
	, channels(_nochannels)
	, avg_count(0)
	, avg_max(1)
	, complete_avg(false)
	, imagenumber(0)
	, complete_frame(false)
	, percent_complete(0.0) {
	// Generate the (blank) images for each channel
	std::generate(channels.begin(), channels.end(), [&]()
		{ return std::make_shared<ScopeImage<uint16_t>>(lines, linewidth, area); });
}

uint16_t ScopeMultiImage::GetPixel(const size_t& _ch, const uint32_t& _x, const uint32_t& _y) const {
	assert( _ch < nochannels );
	return channels[_ch]->Pixel(_x, _y);
}

std::vector<uint16_t> ScopeMultiImage::GetMultiPixel(const uint32_t& _x, const uint32_t& _y) const {
	std::vector<uint16_t> multipix(0);
	for ( auto img : channels )
		multipix.push_back(img->Pixel(_x, _y));
	return multipix;
}

ScopeImageU16Ptr ScopeMultiImage::GetChannel(const size_t& _chan) const {
	assert( _chan < nochannels );
	return channels.at(_chan);
}

void ScopeMultiImage::SetChannel(const size_t& _chan, ScopeImageU16Ptr const _newimg) {
	assert( (_newimg->Lines() == lines) && (_newimg->Linewidth() == linewidth) );
	channels.at(_chan) = _newimg;
}

void ScopeMultiImage::SetAvgCount(const uint32_t& _avg_count) {
	avg_count = _avg_count;
}

void ScopeMultiImage::SetAvgMax(const uint32_t& _avg_max) {
	assert(_avg_max!=0);
	avg_max = _avg_max;
}

void ScopeMultiImage::SetCompleteFrame(const bool& _complete) {
	complete_frame = _complete;
	for ( auto ch : channels )
		ch->SetCompleteFrame(_complete);
}

void ScopeMultiImage::SetPercentComplete(const double& _percent) {
	assert(_percent>=0);
	percent_complete = _percent;
	for ( auto ch : channels )
		ch->SetPercentComplete(_percent);
}

void ScopeMultiImage::SetCompleteAvg(const bool& _complete) {
	complete_avg = _complete;
	for ( auto ch : channels )
		ch->SetCompleteAvg(_complete);
}

void ScopeMultiImage::FillRandom() {
	for ( auto ch : channels )
		ch->FillRandom();
}

}