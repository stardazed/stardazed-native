// ------------------------------------------------------------------
// render::Texture.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/Texture.hpp"
#include <fstream>

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
	
//	for (uint32 level = 0; level < mipMaps; ++level) {
//		size32 size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
//		
//		offset += size;
//		width  /= 2;
//		height /= 2;
//	}
}


ImageData DDSDataProvider::imageDataForLevel(size32 level) const {
	assert(level < mipMaps_);

	size32 blockSize = (format_ == ImageDataFormat::DXT1) ? 8 : 16;
	size32 offset = 0;
	
	auto mipWidth = width_ >> level;
	auto mipHeight = height_ >> level;
	
	ImageData mipData {};
	mipData.width = mipWidth;
	mipData.height = mipHeight;
	mipData.format = format_;
	mipData.size = ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4) * blockSize;
	mipData.data = nullptr;
	return mipData;
}
	
	
} // ns render
} // ns stardazed
