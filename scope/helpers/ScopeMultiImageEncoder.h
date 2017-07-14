#pragma once
#include "hresult_exception.h"
#include "ScopeMultiImage.h"

namespace scope {

/** Encodes multi images to TIFF using Windows Imaging Components. For more information about WIC see e.g.
* "Windows Imaging Component Basics" by Kenny Kerr at http://msdn.microsoft.com/en-us/magazine/cc500647.aspx) */
class ScopeMultiImageEncoder {

protected:
	/** error handler */
	HResult hr;

	/** do we actually save (true) or only count the frames (false) */
	const bool dosave;

	/** how many channels to encode */
	const uint32_t channels;

	/** do TIFF compressiong? */
	const bool compresstiff;

	/** keeping track of how many frames we encoded */
	uint32_t framecount;

	/** our Windows Imaging Component factory */
	IWICImagingFactory* factory;

	/** a stream for each channel */
	std::vector<IWICStream*> streams;

	/** an encoder for each channel */
	std::vector<IWICBitmapEncoder*> encoders;

	/** a frameencoder for each channel (remade for each frame) */
	std::vector<IWICBitmapFrameEncode*> frameencoders;

protected:
	/** disable copy and assignment */
	ScopeMultiImageEncoder(const ScopeMultiImageEncoder&);

	/** because we want RAII but not frequent calling of CoInitialize etc... */
	ScopeMultiImageEncoder operator=(const ScopeMultiImageEncoder&);

public:
	/** Initializes COM and creates WIC imaging factory
	* @param[in] _dosave if false images are not actually saved only counted
	* @param[in] _channels number of channels for saving
	* @param[in] _compresstiff true if you want to write ZIP compressed TIFF, false for uncompressed TIFF */
	ScopeMultiImageEncoder(const bool& _dosave, const uint32_t& _channels, const bool& _compresstiff = true);

	/** Releases resources and uninitializes COM */
	~ScopeMultiImageEncoder();

	/** Creates and initializes encoders and streams for disk writing
	* @param[in] _filenames vector with filenames for each channel (since each channel will be written to a separate file) */
	void Initialize(const std::vector<std::wstring>& _filenames);

	/** Creates a new frame, initializes its frameencoder, and writes metadata */
	void NewFrame();

	/** Writes metadata via the current frameencoder into a frame
	* Is not functional, due to limitations in WIC TIFF encoder. Therefore StorageControllerImpl::FixTIFFFlags. */
	void WriteMetadata();

	/** Writes a complete multi image into the current frames
	* @param[in] _multiimage the multi image to write to disk */
	void WriteFrame(ScopeMultiImagePtr const _multiimage);

	/** Writes the new part of a multiimage into the current frames
	* @param[in] _multiimage the multi image whose new part to write to disk */
	void WriteFrameNewPart(ScopeMultiImagePtr const _multiimage);

	/** @return current framecount */
	uint32_t Framecount() const { return framecount; }
};

}