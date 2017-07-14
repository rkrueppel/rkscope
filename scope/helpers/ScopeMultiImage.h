#pragma once

// Forward declarations
namespace scope {
template<class T>class ScopeImage;
typedef std::shared_ptr<ScopeImage<uint16_t>> ScopeImageU16Ptr;
}

namespace scope {

/** A multichannel image */
class ScopeMultiImage {

protected:
	/** the area from which the multi image comes */
	const uint32_t area;

	/** number of channels */
	const size_t nochannels;

	/** number of lines (y-resolution) */
	const uint32_t lines;

	/** the linewidth (x-resolution) */
	const uint32_t linewidth;

	/** the ScopeImages that contain the data for each channel */
	std::vector<ScopeImageU16Ptr> channels;

	/** this image is the xth average */
	uint32_t avg_count;

	/** ... of this many to average */
	uint32_t avg_max;

	/** false if this is a not completely averaged frame, this is then only for display purpose and will not be stored */
	bool complete_avg;

	/** number of this image */
	uint32_t imagenumber;

	/** false if frame not complete, allows for partial display during acquisition */
	bool complete_frame;

	/** how many percent of the frame are already filled */
	double percent_complete;

public:
	/** Initializes and generate blank images for each channel */
	ScopeMultiImage(const uint32_t& _area = 0, const size_t& _nochannels = 1, const uint32_t& _lines = 256, const uint32_t& _linewidth = 256);

	/** @name Several accessor methods */
	/** @{ */
	uint32_t Area() const { return area; }
	size_t Channels() const { return nochannels; }
	uint32_t Lines() const { return lines; }
	uint32_t Linewidth() const { return linewidth; }
	uint32_t Pixels() const { return lines*linewidth; }

	/** @param[in] _ch which channel
	* @param[in] _x,_y which pixel
	* @return value of pixel of channel */
	uint16_t GetPixel(const size_t& _ch, const uint32_t& _x, const uint32_t& _y) const;
	
	/** @param[in] _x,_y which pixel
	* @return vector with pixel value of every channel */
	std::vector<uint16_t> GetMultiPixel(const uint32_t& _x, const uint32_t& _y) const;

	/** @return pointer to one channel image */
	ScopeImageU16Ptr GetChannel(const size_t& chan) const;

	uint32_t GetAvgCount() const { return avg_count; }

	uint32_t GetAvgMax() const { return avg_max; }

	uint32_t GetImageNumber() const { return imagenumber; }

	bool IsCompleteFrame() const { return complete_frame; }

	bool IsCompleteAvg() const { return complete_avg; }

	double PercentComplete() const { return percent_complete; }
	/** @} */

	/** @name Several mutator methods */
	/** @{ */

	/** Replaces one channel of the multiimage
	* @param[in] _chan channel to be replaced
	* @param[in] _newimg pointer to the new channel image */
	void SetChannel(const size_t& _chan, ScopeImageU16Ptr const _newimg);

	/** Sets the average count of this image */
	void SetAvgCount(const uint32_t& _avg_count);

	/** Sets the maximum average count */
	void SetAvgMax(const uint32_t& _avg_max);

	/** Sets the number of this image */
	void SetImageNumber(const uint32_t& _imagenumber) { imagenumber = _imagenumber; }

	/** Sets frame complete */
	void SetCompleteFrame(const bool& _complete);

	/** Sets complete average */
	void SetCompleteAvg(const bool& _complete);

	/** Sets percent complete */
	void SetPercentComplete(const double& _percent);
	/** @} */

	/** Fills the multi image with random data */
	void FillRandom();
};

/** Shared pointer to a ScopeMultiImage */
typedef std::shared_ptr<ScopeMultiImage> ScopeMultiImagePtr;
/** Shared pointer to a const ScopeMultiImage */
typedef std::shared_ptr<const ScopeMultiImage> ScopeMultiImageCPtr;
}