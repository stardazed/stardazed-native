// ------------------------------------------------------------------
// render::Texture.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/Texture.hpp"
#include "render/common/PNGFile.hpp"

#include <fstream>
#include <algorithm>

namespace stardazed {
namespace render {
	

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
	std::ifstream file{ resourcePath, std::ios::binary };
	assert(file.is_open());
	
	char cookie[4];
	file.read(cookie, 4);
	assert(strncmp(cookie, "DDS ", 4) == 0);
	
	file.read(reinterpret_cast<char*>(&header), sizeof(DDS_HEADER));
	size32 dataSize = header.dwPitchOrLinearSize;
	if (header.dwMipMapCount > 1)
		dataSize *= 2;
	data_ = std::make_unique<char[]>(dataSize);
	file.read(data_.get(), dataSize);
	
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
	std::ifstream file{ resourcePath, std::ios::binary };
	assert(file.is_open());

	BITMAPFILEHEADER header;
	file.read(reinterpret_cast<char*>(&header), sizeof(BITMAPFILEHEADER));
	assert(file && header.bfType == 0x4D42);
	
	BITMAPINFOHEADER info;
	file.read(reinterpret_cast<char*>(&info), sizeof(BITMAPINFOHEADER));
	assert(file && info.biBitCount == 24 && info.biCompression == 0);
	
	width_ = info.biWidth;
	height_ = info.biHeight;
	
	auto dataSize   = 3u * width_ * height_;
	auto dataOffset = header.bfOffBits;
	auto headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	
	assert(dataOffset == headerSize);
	
	data_ = std::make_unique<char[]>(dataSize);
	file.read(data_.get(), dataSize);
}


ImageData BMPDataProvider::imageDataForLevel(uint8 level) const {
	assert(level == 0);
	
	ImageData image {};
	image.width = width();
	image.height = height();
	image.format = format();
	image.size = 3u * width() * height();
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
	
	switch (png.bytesPerPixel()) {
		case 1: format_ = ImageDataFormat::R8; break;
		case 2: format_ = ImageDataFormat::RG8; break;
		case 3: format_ = ImageDataFormat::RGB8; break;
		case 4: format_ = ImageDataFormat::RGBA8; break;
		default:
			assert(!"bytes per pixel not in range");
	}
	
	size_ = png.rowBytes() * png.height();
	data_ = std::make_unique<uint8[]>(size_);
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
	image.size = size_;
	image.data = data_.get();
	return image;
}

	
} // ns render
} // ns stardazed
