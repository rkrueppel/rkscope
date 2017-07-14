#include "StdAfx.h"
#include "ScopeMultiImageEncoder.h"
#include "ScopeImage.h"
#include "helpers/helpers.h"
#include "parameters/Scope.h"

namespace scope {

ScopeMultiImageEncoder::ScopeMultiImageEncoder(const bool& _dosave, const uint32_t& _channels, const bool& _compresstiff)
	: dosave(_dosave)
	, channels(_channels)
	, compresstiff(_compresstiff)
	, framecount(0)
	, factory(nullptr)
	, streams(_channels, nullptr)
	, encoders(_channels, nullptr)
	, frameencoders(_channels, nullptr) {
	if ( dosave ) {
		hr(__FUNCTION__) = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);		// Need to do this here to be in the correct thread (?!)
		hr(__FUNCTION__) = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory);
	}
}

ScopeMultiImageEncoder::~ScopeMultiImageEncoder() {
	if ( dosave ) {
		for ( size_t c = 0 ; c < channels ; c++ ) {
			// if nothing was saved , encoders were not used and the safe release would throw an exception!!
			if ( framecount != 0 ) {
				// Commit all images and close stream to disk. This finalizes the image.
				hr(encoders[c]->Commit());
			}
			SafeRelease(&encoders[c]);
			SafeRelease(&streams[c]);
		}
		SafeRelease(&factory);
		CoUninitialize();
	}
}

void ScopeMultiImageEncoder::Initialize(const std::vector<std::wstring>& _filenames) {
	if ( dosave ) {
		assert(_filenames.size() == channels);
		for ( size_t c = 0 ; c < channels ; c++ ) {
			// Make the TIFF encoders
			hr(factory->CreateEncoder(GUID_ContainerFormatTiff, NULL, &encoders[c]), __FUNCTION__);
			// Make the streams for disk writing
			hr(factory->CreateStream(&streams[c]), __FUNCTION__);
			hr(streams[c]->InitializeFromFilename(_filenames[c].c_str(), GENERIC_WRITE), __FUNCTION__);
			// Connect encoder to stream
			hr(encoders[c]->Initialize(streams[c], WICBitmapEncoderNoCache), __FUNCTION__);
		}
	}
}

void ScopeMultiImageEncoder::NewFrame() {
	if ( dosave ) {
		for ( size_t c = 0 ; c < channels ; c++ ) {
			// For detail of TIFF encoder configuration see: WIC Encoding Overview at http://msdn.microsoft.com/en-us/library/windows/desktop/ee719871%28v=vs.85%29.aspx#tiffencoding
			IPropertyBag2 *pPropertybag = NULL;
			
			// Make a new frameencoder from the encoder
			hr(encoders[c]->CreateNewFrame(&frameencoders[c], &pPropertybag), __FUNCTION__);

			// This is how you customize the TIFF output.
			PROPBAG2 option = { 0 };
			option.pstrName = L"TiffCompressionMethod";
			VARIANT varValue;
			VariantInit(&varValue);
			varValue.vt = VT_UI1;
			if ( compresstiff )
				varValue.bVal = WICTiffCompressionZIP;
			else
				varValue.bVal = WICTiffCompressionNone;
			hr(pPropertybag->Write(1, &option, &varValue));
			hr(frameencoders[c]->Initialize(pPropertybag));
		}
		if ( framecount == 0 )
			WriteMetadata();
	}
	framecount++;
}

void ScopeMultiImageEncoder::WriteMetadata() {
	// We do all this in StorageControllerImpl::FixTIFFTags !

	/*for ( size_t c = 0 ; c < channels ; c++ ) {
		IWICMetadataQueryWriter* metadatawriter = nullptr;
		hr(frameencoders[c]->GetMetadataQueryWriter(&metadatawriter));
		PROPVARIANT value;
		PropVariantInit(&value);
		value.vt = VT_LPWSTR;
		value.pwszVal = L"A Scope image";
		hr(metadatawriter->SetMetadataByName(L"/ifd/{ushort=270}", &value));
		
		// WIC does not let you overwrite tags it sets itself (i.e. resolution unit and resolution) ! -> FixTIFFFTags
		PROPVARIANT value2;
		PropVariantInit(&value2);
		value2.vt = VT_I2;					
		value2.iVal = 1;
		hr(metadatawriter->SetMetadataByName(L"/ifd/{ushort=296}", &value2));		// Unit for resolution (datatype short)

		PROPVARIANT value3;
		PropVariantInit(&value3);
		value3.vt = VT_R4;					
		value3.fltVal = 3.3;
		hr(metadatawriter->SetMetadataByName(L"/ifd/{ushort=282}", &value3));

		SafeRelease(&metadatawriter);
	}*/
}

void ScopeMultiImageEncoder::WriteFrame(ScopeMultiImagePtr const _multiimage) {
	if ( dosave ) {
		assert(channels == _multiimage->Channels());
		for ( size_t c = 0 ; c < channels ; c++ ) {
			// Set pixel format and check if supported by encoder
			WICPixelFormatGUID formatGUID = GUID_WICPixelFormat16bppGray;
			hr(frameencoders[c]->SetPixelFormat(&formatGUID), __FUNCTION__);
			hr((IsEqualGUID(formatGUID, GUID_WICPixelFormat16bppGray) ? S_OK : E_FAIL), __FUNCTION__);

			/* Some advice about the WIC buffer stuff
			Taken from Windows Imaging Component Basics by Kenny Kerr http://msdn.microsoft.com/en-us/magazine/cc500647.aspx)
			"The stride can be one of the more confusing aspects of bitmaps.
			Stride is the count of bytes between scanlines. Generally speaking, the bits that make up the pixels of a bitmap are packed into rows.
			A single row should be long enough to store one row of the bitmap's pixels.
			The stride is the length of a row measured in bytes, rounded up to the nearest DWORD (4 bytes).
			This allows bitmaps with fewer than 32 bits per pixel (bpp) to consume less memory while still providing good performance.
			You can use the following function to calculate the stride for a given bitmap:
			const UINT byteCount = bitCount / 8;
			const UINT stride = (width * byteCount + 3) & ~3; "
			*/

			// RESONANCE CODE
			// if in resonance scanner mode, the forward and backward lines are stored in one row, i.e. the image has other dimensions
			
			// I do not see why this is necessary, if  the image is anyway  postprocessed the lines can be  stored the usual way..., RKr 7/1/15
			
			/*if ( _resonance ) {
				hr(frameencoders[c]->SetSize(_multiimage->Linewidth() * 2, _multiimage->Lines() / 2), __FUNCTION__);

				const UINT cbStride = (_multiimage->Linewidth() * 2 * 2 + 3) & ~3;
				const UINT cbBufferSize = _multiimage->Lines() / 2 * cbStride;

				ScopeImageAccessU16 imagedata(*_multiimage->GetChannel(c));
			
				// Have to reinterpret since WritePixels wants a byte pointer to the data (which in our case is acutally 16bit uint16_t
				//const std::vector<uint16_t>* dataptr(imagedata.GetConstData());
				//uint16_t* dataptr = imagedata.GetConstData()->data();
				hr(frameencoders[c]->WritePixels(_multiimage->Lines() / 2, cbStride, cbBufferSize, reinterpret_cast<BYTE*>(imagedata.GetPointer())), __FUNCTION__);
			}
			else {*/
				hr(frameencoders[c]->SetSize(_multiimage->Linewidth(), _multiimage->Lines()), __FUNCTION__);

				const UINT cbStride = (_multiimage->Linewidth() * 2 + 3) & ~3;
				const UINT cbBufferSize = _multiimage->Lines() * cbStride;

				ScopeImageAccessU16 imagedata(*_multiimage->GetChannel(c));
			
				// Have to reinterpret since WritePixels wants a byte pointer to the data (which in our case is acutally 16bit uint16_t
				hr(frameencoders[c]->WritePixels(_multiimage->Lines(), cbStride, cbBufferSize, reinterpret_cast<BYTE*>(imagedata.GetPointer())), __FUNCTION__);
			//}

			// Commit frame to image
			hr(frameencoders[c]->Commit(), __FUNCTION__);
			SafeRelease(&frameencoders[c]);
		}
	}
}

void ScopeMultiImageEncoder::WriteFrameNewPart(ScopeMultiImagePtr const _multiimage) {
	if ( dosave ) {
		assert(channels < _multiimage->Channels());
		for ( size_t c = 0 ; c < channels ; c++ ) {
			hr(frameencoders[c]->SetSize(_multiimage->Linewidth(), _multiimage->Lines()), __FUNCTION__);

			WICPixelFormatGUID formatGUID = GUID_WICPixelFormat16bppGray;
			hr(frameencoders[c]->SetPixelFormat(&formatGUID), __FUNCTION__);
			hr((IsEqualGUID(formatGUID, GUID_WICPixelFormat16bppGray) ? S_OK : E_FAIL), __FUNCTION__);

			UINT cbStride = (_multiimage->Linewidth() * 2 + 3) & ~3;
			ScopeImage<uint16_t>::datapart_t part = _multiimage->GetChannel(c)->Newpart();
			UINT cbBufferSize = static_cast<UINT>((part.second - part.first)/_multiimage->Linewidth() * cbStride);

			ScopeImageAccessU16 imagedata(*_multiimage->GetChannel(c));
			hr(frameencoders[c]->WritePixels(_multiimage->Lines(), cbStride, cbBufferSize, reinterpret_cast<BYTE*>(imagedata.GetPointer())), __FUNCTION__);

			hr(frameencoders[c]->Commit(), __FUNCTION__);
			SafeRelease(&frameencoders[c]);
		}
	}
}

}