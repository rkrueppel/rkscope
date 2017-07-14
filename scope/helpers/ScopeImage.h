#pragma once

#include "pixel.h"
#include "DaqChunk.h"
#include "ScopeException.h"
#include "helpers.h"

namespace scope {

template<class T>
class ScopeImageAccess;

template<class T>
class ScopeImageConstAccess;

/** A general image class.
* Templatized for data type, in Scope usually uint16_t. Data is in one vector data order is linewise:\n
* 1234\n
* 5678\n
* 9...\n
* The can only be accessed via ScopeImageAccess and ScopeImageConstAccess. */
template<class T>
class ScopeImage {

	/** give accessor classes friend access to the data */
	friend ScopeImageAccess<T>;	

	/** give accessor classes friend access to the data */
	friend ScopeImageConstAccess<T>;

public:
	/** pair of two iterators over the data vector */
	typedef std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> datapart_t;

	/** const iterator over the data vector */
	typename typedef std::vector<T>::const_iterator citerator;

	/** the size type */
	typename typedef std::vector<T>::size_type size_type;

protected:
	/** area of the image */
	const uint32_t area;

	/** number of lines, y resolution */
	uint32_t lines;

	/** width of a line, x resolution */
	uint32_t linewidth;

	/** false if frame not complete, allows for partial display during acquisition */
	bool complete_frame;

	/** false if this is a not completely averaged frame, it is then only for display purpose and will not be stored */
	bool complete_avg;

	/** how many percent of the frame are already filled */
	double percent_complete;

	/** vector with pixel data */
	std::vector<T> data;

	/** current insertion position */
	typename std::vector<T>::iterator inserter;

	/** range of freshly inserted pixels */
	datapart_t newpart;

	/** mutex for protection of pixel operations */
	mutable std::mutex pixelmutex;

	/** >0 if someone got a pointer to the const datavector */
	mutable std::atomic<int16_t> reading_access;

	/** condition variable for read access */
	mutable std::condition_variable readcond;

public:
	/** Initialize with zeros */
	ScopeImage(const uint32_t& _lines = 128, const uint32_t& _linewidth = 128, const uint32_t& _area = 0, const bool& _complete_avg = false, const bool& _complete_frame = false)
		: area(_area)
		, lines(_lines)
		, linewidth(_linewidth)
		, complete_frame(_complete_frame)
		, percent_complete(0.0)
		, complete_avg(_complete_avg)
		, data(_lines*_linewidth, T(0))
		, inserter(data.begin())
		, newpart(data.begin(), data.begin())
		, reading_access(0) {
	}

	/** Safe copy. Inserter and newpart are not  copied! */
	ScopeImage(const ScopeImage& _si)
		: area(_si.area)
		, lines(_si.lines)
		, linewidth(_si.linewidth)
		, complete_frame(_si.complete_frame)
		, percent_complete(_si.percent_complete)
		, complete_avg(_si.complete_avg)
		, reading_access(0) {
		ScopeImageConstAccess<T> acc(_si);
		data = *acc.GetConstData();
		inserter = data.begin();
		newpart = datapart_t(data.begin(), data.begin());
	}

	/** Safe assignment. Inserter and newpart are reset to data.begin()! */
	ScopeImage& operator=(const ScopeImage& _si) {
		// Avoid self-assignment
		if ( this != &_si ) {
			assert(lines == _si.lines);
			assert(linewidth == _si.linewidth);
			area = _si.area;
			complete_frame = _si.complete_frame;
			percent_complete = _si.percent_complete;
			complete_avg = _si.complete_avg;
			ScopeImageAccess<T> acc(_si);
			data = *acc.GetData();
			inserter = data.begin();
			newpart = datapart_t(data.begin(), data.begin());
		}
		return *this;
	}

	/** @name Several accessor methods
	* @{ */
	uint32_t Area() const { return area; }
	uint32_t Lines() const { return lines; }
	uint32_t Linewidth() const { return linewidth; }
	datapart_t Newpart() const { return newpart; }
	bool IsCompleteFrame() const { return complete_frame; }
	double PercentComplete() const { return percent_complete; }
	bool IsCompleteAvg() const { return complete_avg; }
	/** @} */

	/** @name Mutator methods
	* @{ */
	void SetCompleteFrame(const bool& _complete) { complete_frame = _complete; }
	void SetPercentComplete(const double& _percent) { percent_complete = _percent; }
	void SetCompleteAvg(const bool& _complete) { complete_avg = _complete; }
	/** @} */

	/** @return return  the pixel at _column, _line */
	T Pixel (const uint32_t& _column, const uint32_t& _line) const {
		ScopeImageConstAccess<T> acc(*this);
		return acc.GetConstData()->at(_line * linewidth + _column);
	}

	/** Fills the complete image with random pixel data */
	void FillRandom() {
		std::lock_guard<std::mutex> lock(pixelmutex);
		std::mt19937 mt;
		mt.seed(static_cast<unsigned long>(GetTickCount64()));
		std::generate( std::begin(data), std::end(data), mt);
		newpart = std::make_pair(data.begin(), data.end());
	}

protected:
	/** @return a pointer to the pixel vector
	* @post pixelmutex is locked */
	std::vector<T>* GetData() {
		std::unique_lock<std::mutex> lock(pixelmutex);
		while ( reading_access > 0 )
			readcond.wait(lock);
		lock.release();			// mutex stays locked
		return &data;
	}

	/** @return a const pointer to the pixel vector. Increase reading_access counter */
	const std::vector<T>* GetDataConst() const {
		pixelmutex.lock();				// waits until a write access is finished
		reading_access++;
		pixelmutex.unlock();
		return &data;
	}

	/** @post pixelmutex is released from lock */
	void ReleaseData() {
		pixelmutex.unlock();
	}

	/** Decreases reading_access counter. Notifies waiting threads. */
	void ReleaseDataConst() const {
		assert(reading_access > 0);
		reading_access--;
		readcond.notify_all();
	}
};

/** Gives RAII safe access (read&write) to the pixeldata of a ScopeImage. ScopeImage pixelmutex is locked on construction and unlocked on destruction of
the ScopeImageAccess object. */
template<class T>
class ScopeImageAccess {
protected:
	/** pointer to the ScopeImage */
	ScopeImage<T>* const image;
	/** Pointer to the ScopeImage's data vector */
	std::vector<T>* const pData;

public:
	/** Get the data, locks mutex inside ScopeImage */
	ScopeImageAccess(ScopeImage<T>& _image)
		: image(&_image)
		, pData(image->GetData()) {
	}

	/** Release data on destruction, unlocks mutex */
	~ScopeImageAccess() {
		image->ReleaseData();
	}
	
	/** @return the pointer to the vector */
	std::vector<T>* GetData() const {
		return pData;
	}

	/** @return pointer to the first vector element */
	T* GetPointer() const {
		return pData->data();
	}
};

/** Gives RAII safe const access (read-only) to the pixeldata of a ScopeImage. A reading access counter is increased on construction and decreased on destruction of
the ScopeImageConstAccess object. This allows concurrent read access. */
template<class T>
class ScopeImageConstAccess {
protected:
	/** pointer to the ScopeImage */
	const ScopeImage<T>* const image;
	/** Pointer to the ScopeImage's data vector */
	const std::vector<T>* const pData;

public:
	/** Get the data, locks mutex inside ScopeImage */
	ScopeImageConstAccess(const ScopeImage<T>& _image)
	: image(&_image)
	, pData(image->GetDataConst()) {
	}

	/** Release data on destruction, unlocks mutex */
	~ScopeImageConstAccess() {
		image->ReleaseDataConst();
	}
	
	/** @return the data pointer */
	const std::vector<T>* GetConstData() const {
		return pData;
	}
};

/** shared pointer to a ScopeImage of 16 bit values */
typedef std::shared_ptr<ScopeImage<uint16_t>> ScopeImageU16Ptr;
/** shared pointer to a const ScopeImage of 16 bit values */
typedef std::shared_ptr<const ScopeImage<uint16_t>> ScopeImageU16CPtr;
/** ScopeImageAccess to a ScopeImage of 16 bit values */
typedef ScopeImageAccess<uint16_t> ScopeImageAccessU16;
/** ScopeImageConstAccess to a ScopeImage of 16 bit values */
typedef ScopeImageConstAccess<uint16_t> ScopeImageConstAccessU16;

/** Helper function to insert a pixel range into a ScopeImage
* Inserts a pixel range from a DaqChunk into the image (replaces existing pixel values)
* @param[in] _img the image to insert into
* @param[in,out] _where in: the position in the image to start inserting, out: end of inserted range in image or end of image
* @param[in] _from start of the pixel range to be inserted
* @param[in] _to end of the pixel range to be inserted
* @return image filled until the end? */
bool InsertPixels(ScopeImageU16Ptr const _img, std::vector<uint16_t>::iterator& _where, const DaqChunk::iterator& _from, const DaqChunk::iterator& _to);

/** Helper function to insert a pixel range into a ScopeImage and averaging the new pixels with the existing ones (for a running averaging)
* If current pixel is an average of e.g. two, i=(i0+i1)/2, multiply the pixel by two, add the current daq pixel and divide by three to get the current average value
*	i3 = ( [(i0+i1)/2]*2 + i3 ) / 3		-> in general: in = ( [(i0+...+in-1)/(n-1)] * (n-1) + in ) / n
*	the disadvantage with this method is the accumulating rounding error, since we do integer division. Avoid this with the check residual trick (see also DaqChunk::Downsample)
* @param[in] _img the image to insert into
* @param[in,out] _where in: the position in the image to start inserting, out: end of inserted range in image or end of image
* @param[in] _from start of the pixel range to be inserted
* @param[in] _to end of the pixel range to be inserted
* @param[in] _currentavgcount the current average number (see above)
* @return image filled until the end? */
bool InsertAndAveragePixels(ScopeImageU16Ptr const _img, std::vector<uint16_t>::iterator& _where, const DaqChunk::iterator _from, const DaqChunk::iterator _to, const uint16_t& _currentavgcount);

}