// ------------------------------------------------------------------
// render::Texture.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/Texture.hpp"
#include "render/common/PNGFile.hpp"
#include "filesystem/FileSystem.hpp"

#include "jpgd.h"

#include <algorithm>

namespace stardazed {
namespace render {


std::unique_ptr<TextureDataProvider> makeDataProviderForPath(const std::string& resourcePath) {
	fs::Path path { resourcePath };
	auto extension = path.extension(); // guaranteed lowercase
	
	if (extension == "dds")
		return std::make_unique<DDSDataProvider>(resourcePath);
	else if (extension == "bmp")
		return std::make_unique<BMPDataProvider>(resourcePath);
	else if (extension == "png")
		return std::make_unique<PNGDataProvider>(resourcePath);
	else if (extension == "tga")
		return std::make_unique<TGADataProvider>(resourcePath);
	else if ((extension == "jpg") || (extension == "jpeg"))
		return std::make_unique<JPGDataProvider>(resourcePath);
	
	return { nullptr };
}
	

//  ____  ____  ____    _____ _ _
// |  _ \|  _ \/ ___|  |  ___(_) | ___  ___
// | | | | | | \___ \  | |_  | | |/ _ \/ __|
// | |_| | |_| |___) | |  _| | | |  __/\__ \
// |____/|____/|____/  |_|   |_|_|\___||___/
//

struct DDS_PIXELFORMAT {
	uint32 dwSize;
	uint32 dwFlags;
	uint32 dwFourCC;
	uint32 dwRGBBitCount;
	uint32 dwRBitMask;
	uint32 dwGBitMask;
	uint32 dwBBitMask;
	uint32 dwABitMask;
};


struct DDS_HEADER {
	uint32           dwSize;
	uint32           dwFlags;
	uint32           dwHeight;
	uint32           dwWidth;
	uint32           dwPitchOrLinearSize;
	uint32           dwDepth;
	uint32           dwMipMapCount;
	uint32           dwReserved1[11];
	DDS_PIXELFORMAT  ddspf;
	uint32           dwCaps;
	uint32           dwCaps2;
	uint32           dwCaps3;
	uint32           dwCaps4;
	uint32           dwReserved2;
};


constexpr uint32 fourCharCode(char a, char b, char c, char d) {
	return (d << 24) | (c << 16) | (b << 8) | a;
}


DDSDataProvider::DDSDataProvider(const std::string& resourcePath) {
	DDS_HEADER header;
	fs::FileReadStream file{ resourcePath };
	
	char cookie[4];
	file.readBytes(cookie, 4);
	assert(strncmp(cookie, "DDS ", 4) == 0);
	
	file.readValue(&header);
	size32 dataSize = header.dwPitchOrLinearSize;
	if (header.dwMipMapCount > 1)
		dataSize *= 2;
	data_ = std::make_unique<uint8[]>(dataSize);
	file.readBytes(data_.get(), dataSize);
	
	switch (header.ddspf.dwFourCC) {
		case fourCharCode('D','X','T','1'): format_ = ImageDataFormat::DXT1; break;
		case fourCharCode('D','X','T','3'): format_ = ImageDataFormat::DXT3; break;
		case fourCharCode('D','X','T','5'): format_ = ImageDataFormat::DXT5; break;
		default:
			assert(!"unknown data format of DDS file");
	}
	
	mipMaps_ = header.dwMipMapCount;
	width_ = header.dwWidth;
	height_ = header.dwHeight;
}


size32 DDSDataProvider::dataSizeForLevel(uint8 level) const {
	size32 blockSize = (format_ == ImageDataFormat::DXT1) ? 8 : 16;
	auto mipWidth = dimensionAtMipLevel(width_, level);
	auto mipHeight = dimensionAtMipLevel(height_, level);

	return ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4) * blockSize;
}


ImageData DDSDataProvider::imageDataForLevel(uint8 level) const {
	assert(level < mipMaps_);

	size32 offset = 0;
	for (size32 lv=0; lv < level; ++lv)
		offset += dataSizeForLevel(lv);
	
	auto mipWidth = dimensionAtMipLevel(width_, level);
	auto mipHeight = dimensionAtMipLevel(height_, level);
	
	ImageData mipData {};
	mipData.width = mipWidth;
	mipData.height = mipHeight;
	mipData.format = format_;
	mipData.size = dataSizeForLevel(level);
	mipData.data = data_.get() + offset;
	return mipData;
}


//  ____  __  __ ____    _____ _ _
// | __ )|  \/  |  _ \  |  ___(_) | ___  ___
// |  _ \| |\/| | |_) | | |_  | | |/ _ \/ __|
// | |_) | |  | |  __/  |  _| | | |  __/\__ \
// |____/|_|  |_|_|     |_|   |_|_|\___||___/
//

struct BITMAPFILEHEADER {
	uint16 bfType;
	uint32 bfSize;
	uint16 bfReserved1;
	uint16 bfReserved2;
	uint32 bfOffBits;
} __attribute__((__packed__));


struct BITMAPINFOHEADER {
	uint32 biSize;
	int32  biWidth;
	int32  biHeight;
	uint16 biPlanes;
	uint16 biBitCount;
	uint32 biCompression;
	uint32 biSizeImage;
	int32  biXPelsPerMeter;
	int32  biYPelsPerMeter;
	uint32 biClrUsed;
	uint32 biClrImportant;
} __attribute__((__packed__));


BMPDataProvider::BMPDataProvider(const std::string& resourcePath) {
	fs::FileReadStream file{ resourcePath };

	BITMAPFILEHEADER header;
	file.readValue(&header);
	assert(header.bfType == 0x4D42);
	
	BITMAPINFOHEADER info;
	file.readValue(&info);
	assert(info.biCompression == 0);
	
	switch (info.biBitCount) {
		case 24: format_ = ImageDataFormat::BGR8; break;
		case 32: format_ = ImageDataFormat::BGRA8; break;
		default:
			assert(!"can only handle 24 or 32 bit BMPs");
			break;
	}
	
	width_ = info.biWidth;
	height_ = info.biHeight;
	
	auto dataSize   = imageDataFormatBytesPerPixel(format_) * width_ * height_;
	auto dataOffset = header.bfOffBits;
	auto headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	
	assert(dataOffset == headerSize);
	
	data_ = std::make_unique<uint8[]>(dataSize);
	file.readBytes(data_.get(), dataSize);
}


ImageData BMPDataProvider::imageDataForLevel(uint8 level) const {
	assert(level == 0);
	
	ImageData image {};
	image.width = width();
	image.height = height();
	image.format = format();
	image.size = imageDataFormatBytesPerPixel(format_) * width() * height();
	image.data = data_.get();
	return image;
}


//  ____  _   _  ____   _____ _ _
// |  _ \| \ | |/ ___| |  ___(_) | ___  ___
// | |_) |  \| | |  _  | |_  | | |/ _ \/ __|
// |  __/| |\  | |_| | |  _| | | |  __/\__ \
// |_|   |_| \_|\____| |_|   |_|_|\___||___/
//

PNGDataProvider::PNGDataProvider(const std::string& resourcePath) {
	PNGFile png(resourcePath);
	
	width_ = png.width();
	height_ = png.height();
	
	switch (png.bytesPerPixel()) {
		case 1: format_ = ImageDataFormat::R8; break;
		case 2: format_ = ImageDataFormat::RG8; break;
		case 3: format_ = ImageDataFormat::RGB8; break;
		case 4: format_ = ImageDataFormat::RGBA8; break;
		default:
			assert(!"bytes per pixel not in range");
	}
	
	auto size = png.rowBytes() * png.height();
	data_ = std::make_unique<uint8[]>(size);
	auto dataPtr = data_.get();

	for (auto row = 0u; row < png.height(); ++row) {
		auto line = png.rowDataPointer(row);
		std::copy(line, line + png.rowBytes(), dataPtr);
		dataPtr += png.rowBytes();
	}
}


ImageData PNGDataProvider::imageDataForLevel(uint8 level) const {
	assert(level == 0);
	
	ImageData image {};
	image.width = width();
	image.height = height();
	image.format = format();
	image.size = imageDataFormatBytesPerPixel(format()) * width() * height();
	image.data = data_.get();
	return image;
}


//  _____ ____    _      _____ _ _
// |_   _/ ___|  / \    |  ___(_) | ___  ___
//   | || |  _  / _ \   | |_  | | |/ _ \/ __|
//   | || |_| |/ ___ \  |  _| | | |  __/\__ \
//   |_| \____/_/   \_\ |_|   |_|_|\___||___/
//

enum TGAImageType : uint8 {
	TGAIT_None = 0,
	TGAIT_Paletted = 1,
	TGAIT_RGB = 2,
	TGAIT_Grayscale = 3,

	TGAIT_RLEBit = 8,
	TGAIT_CompressedBit = 32
};

struct TGAFileHeader {
	uint8  identLengthUnused;
	uint8  usePalette;
	TGAImageType imageType;
	uint16 firstPaletteIndex;
	uint16 paletteEntryCount;
	uint8  paletteBits;
	uint16 originX;
	uint16 originY;
	uint16 width;
	uint16 height;
	uint8  bitDepth;
	uint8  flagsUnused;
} __attribute__((__packed__));

static_assert(sizeof(TGAFileHeader) == 18, "");


TGADataProvider::TGADataProvider(const std::string& resourcePath) {
	fs::FileReadStream file{ resourcePath };
	
	TGAFileHeader header;
	file.readValue(&header);

	assert(header.identLengthUnused == 0);
	assert(header.usePalette == 0);
	assert(! (header.imageType & TGAIT_RLEBit));
	assert(! (header.imageType & TGAIT_CompressedBit));
	
	width_ = header.width;
	height_ = header.height;

	if (header.imageType == TGAIT_RGB) {
		if (header.bitDepth == 24)
			format_ = ImageDataFormat::BGR8;
		else if (header.bitDepth == 32)
			format_ = ImageDataFormat::BGRA8;
		else
			assert(!"for RGB image types, only 24 and 32 bit depths are supported");
	}
	else if (header.imageType == TGAIT_Grayscale) {
		format_ = ImageDataFormat::R8;
		assert(header.bitDepth == 8);
	}
	else {
		assert(!"unknown or inconsistent image type");
	}
	
	auto dataSize = imageDataFormatBytesPerPixel(format_) * width_ * height_;
	data_ = std::make_unique<uint8[]>(dataSize);
	file.readBytes(data_.get(), dataSize);
}


ImageData TGADataProvider::imageDataForLevel(uint8 level) const {
	assert(level == 0);
	
	ImageData image {};
	image.width = width();
	image.height = height();
	image.format = format();
	image.size = imageDataFormatBytesPerPixel(format()) * width() * height();
	image.data = data_.get();
	return image;
}


//      _ ____   ____   _____ _ _
//     | |  _ \ / ___| |  ___(_) | ___  ___
//  _  | | |_) | |  _  | |_  | | |/ _ \/ __|
// | |_| |  __/| |_| | |  _| | | |  __/\__ \
//  \___/|_|    \____| |_|   |_|_|\___||___/
//

class SDJPGDecoderStream : public jpgd::jpeg_decoder_stream {
	fs::Path path_;
	fs::FileReadStream file_;
	int fileSize_;

public:
	SDJPGDecoderStream(const std::string resourcePath)
	: path_ { resourcePath }
	, file_ { path_ }
	, fileSize_{ (int)path_.fileSize() }
	{}
	
	int read(uint8 *buffer, int maxBytesToRead, bool *outEOFFlag) override {
		int offset = (int)file_.offset(),
			remaining = fileSize_ - offset;
		
		int toRead = math::min(remaining, maxBytesToRead);
		file_.readBytes(buffer, toRead);
		if (toRead == remaining)
			*outEOFFlag = true;
		
		return toRead;
	}
};


JPGDataProvider::JPGDataProvider(const std::string& resourcePath) {
	SDJPGDecoderStream stream{ resourcePath };
	
	int width, height, components;
	auto data = jpgd::decompress_jpeg_image_from_stream(&stream, &width, &height, &components, 4);

	// adopt the data pointer for auto-disposal
	data_.reset(data);
	width_ = width;
	height_ = height;
}


ImageData JPGDataProvider::imageDataForLevel(uint8 level) const {
	assert(level == 0);
	
	ImageData image {};
	image.width = width();
	image.height = height();
	image.format = format();
	image.size = imageDataFormatBytesPerPixel(format()) * width() * height();
	image.data = data_.get();
	return image;
}

	
} // ns render
} // ns stardazed
