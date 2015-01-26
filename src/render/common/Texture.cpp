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

	
} // ns render
} // ns stardazed

/*
 #include "zlib.h"
 
 #include <netinet/in.h>
 
 #include <vector>
 #include <fstream>
 #include <iostream>
 #include <string>
 #include <algorithm>
 #include <cassert>
 
 struct ChunkHeader {
	uint32_t dataSize;
	uint32_t chunkType;
 };
 
 constexpr uint32_t fourCharCode(char a, char b, char c, char d) {
	return (d << 24) | (c << 16) | (b << 8) | a;
 }
 
 
 enum ChunkType : uint32_t {
	HeaderChunk = fourCharCode('I','H','D','R'),
	ImageDataChunk = fourCharCode('I','D','A','T'),
	EndChunk = fourCharCode('I','E','N','D')
 };
 
 
 struct IHDRChunk {
	uint32_t Width;        // Width of image in pixels
	uint32_t Height;       // Height of image in pixels
	uint8_t BitDepth;      // Bits per pixel or per sample
	uint8_t ColorType;     // Color interpretation indicator
	uint8_t Compression;   // Compression type indicator
	uint8_t Filter;        // Filter type indicator
	uint8_t Interlace;     // Type of interlacing scheme used
 } __attribute__((__packed__));
 
 static_assert(sizeof(IHDRChunk) == 13, "bleh");
 
 
 int inflateBuffer(const std::vector<uint8_t>& source, std::vector<uint8_t>& dest) {
	int ret;
	unsigned have;
	z_stream strm;
 
	// allocate inflate state
	strm.zalloc = nullptr;
	strm.zfree = nullptr;
	strm.opaque = nullptr;
	strm.avail_in = static_cast<uint32_t>(source.size());
	strm.next_in = source.data();
	ret = inflateInit(&strm);
	if (ret != Z_OK)
 return ret;
 
	const uint32_t tempBufSize = 512 * 1024;
	std::vector<uint8_t> tempBuf(tempBufSize);
	auto appender = std::back_inserter(dest);
 
	do {
 strm.avail_out = tempBufSize;
 strm.next_out = tempBuf.data();
 
 ret = inflate(&strm, Z_NO_FLUSH);
 assert(ret != Z_STREAM_ERROR);  // state not clobbered
 
 if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
 inflateEnd(&strm);
 return ret;
 }
 
 have = tempBufSize - strm.avail_out;
 std::copy(tempBuf.begin(), tempBuf.begin() + have, appender);
	} while (strm.avail_out == 0);
 
	// clean up and return
	inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
 }
 
 
 enum LineFilter : uint8_t {
	LFNone = 0,
	LFSub = 1,
	LFUp = 2,
	LFAverage = 3,
	LFPaeth = 4
 };
 
 
 class Ping {
	uint32_t width_ = 0, height_ = 0, bpp_ = 0;
	std::vector<uint8_t> compressedData_, imageData_;
 
	void nextChunk(std::istream& png) {
 std::vector<uint8_t> tempData(8192); // seems to be common IDAT data size
 auto appender = std::back_inserter(compressedData_);
 
 ChunkHeader chdr;
 png.read(reinterpret_cast<char*>(&chdr), sizeof(ChunkHeader));
 chdr.dataSize = ntohl(chdr.dataSize);
 
 auto typeName = std::string { reinterpret_cast<char*>(&chdr.chunkType), reinterpret_cast<char*>(&chdr.chunkType) + 4 };
 // std::cout << "Chunk: " << typeName << '\n';
 // std::cout << "Size : " << chdr.dataSize << '\n';
 
 switch (chdr.chunkType) {
 case HeaderChunk:
 {
 IHDRChunk ihdr;
 png.read(reinterpret_cast<char*>(&ihdr), sizeof(IHDRChunk));
 width_ = ntohl(ihdr.Width);
 height_ = ntohl(ihdr.Height);
 std::cout << "Width : " << width_ << '\n';
 std::cout << "Height: " << height_ << '\n';
 std::cout << "Bits  : " << (int)ihdr.BitDepth << '\n';
 std::cout << "Kind  : " << (int)ihdr.ColorType << '\n';
 std::cout << "Compression : " << (int)ihdr.Compression << '\n';
 std::cout << "Filter : " << (int)ihdr.Filter << '\n';
 std::cout << "Interlace : " << (int)ihdr.Interlace << '\n';
 
 assert(ihdr.BitDepth == 8);
 
 switch (ihdr.ColorType) {
 case 2: bpp_ = 3; break;
 case 4: bpp_ = 2; break;
 case 6: bpp_ = 4; break;
 default: bpp_ = 1; break;
 }
 
 compressedData_.reserve(width_ * height_); // reasonable amount of mem for deflated data
 break;
 }
 
 case ImageDataChunk:
 {
 if (chdr.dataSize > tempData.size())
 tempData.resize(chdr.dataSize);
 png.read(reinterpret_cast<char*>(tempData.data()), chdr.dataSize);
 std::copy(tempData.begin(), tempData.begin() + chdr.dataSize, appender);
 break;
 }
 
 default:
 png.seekg(chdr.dataSize, std::ios::cur);
 break;
 }
 
 // skip crc
 png.seekg(4, std::ios::cur);
	}
 
	void unfilterImage() {
 auto addv = [](uint8_t a, uint8_t b) {
 uint32_t s32 = (uint32_t)a + (uint32_t)b;
 return s32 & 0xff;
 };
 
 auto rowPtr = imageData_.data();
 auto rowBytes = width_ * bpp_;
 auto rowPitch = rowBytes + 1;
 
 for (auto lineIx = 0u; lineIx < height_; ++lineIx) {
 LineFilter filter = (LineFilter)(*rowPtr++);
 
 if (filter != LFNone) {
 
 if (filter == LFSub) {
 auto row = rowPtr + bpp_;
 auto bytes = rowBytes - bpp_;
 while (bytes--) {
 *row = addv(*row, *(row - bpp_));
 ++row;
 }
 }
 else if (filter == LFUp && lineIx > 0) {
 auto row = rowPtr;
 auto bytes = rowBytes;
 while (bytes--) {
 *row = addv(*row, *(row - rowPitch));
 ++row;
 }
 }
 }
 
 rowPtr += rowBytes;
 }
	}
 
 public:
	Ping(const std::string& resourcePath) {
 std::ifstream png { resourcePath, std::ios::binary };
 
 uint8_t realSig[8], expectedSig[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
 png.read(reinterpret_cast<char*>(realSig), 8);
 assert(std::equal(realSig, realSig + 8, expectedSig, expectedSig + 8));
 
 while (png)
 nextChunk(png);
 
 inflateBuffer(compressedData_, imageData_);
 std::cout << compressedData_.size() << '\n';
 std::cout << imageData_.size() << '\n';
 
 unfilterImage();
	}
 
	const auto& imageData() const { return imageData_; }
	const uint32_t width() const { return width_; }
	const uint32_t height() const { return height_; }
 
	uint8_t* rowDataPointer(uint32_t row) {
 return imageData_.data() + (row * ((width_ * 3) + 1)) + 1;
	}
 };
 
 
 int main() {
	Ping png { "main.png" };
	std::ofstream out { "out.dat", std::ios::out | std::ios::binary };
 
	// out.write(reinterpret_cast<const char*>(png.imageData().data()), png.imageData().size());
 
	for (int32_t row = 0; row < png.height(); ++row)
 out.write(reinterpret_cast<const char*>(png.rowDataPointer(row)), png.width() * 3);
 }

*/