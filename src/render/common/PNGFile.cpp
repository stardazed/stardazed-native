// ------------------------------------------------------------------
// render::PNGFile.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/PNGFile.hpp"
#include "system/Logging.hpp"

#include "zlib.h"
#include <fstream>

namespace stardazed {
namespace render {


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

static_assert(sizeof(IHDRChunk) == 13, "IHDRChunk type must be packed");


int inflateBuffer(const std::vector<uint8_t>& source, std::vector<uint8_t>& dest) {
	int ret;
	unsigned have;
	z_stream strm;
	
	// allocate inflate state
	strm.zalloc = nullptr;
	strm.zfree = nullptr;
	strm.opaque = nullptr;
	strm.avail_in = static_cast<uint32_t>(source.size());
	strm.next_in = const_cast<uint8*>(source.data());
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


enum LineFilter : uint8 {
	LFNone = 0,
	LFSub = 1,
	LFUp = 2,
	LFAverage = 3,
	LFPaeth = 4
};


PNGFile::PNGFile(const std::string& resourcePath) {
	std::ifstream png { resourcePath, std::ios::binary };
	
	uint8 realSig[8], expectedSig[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	png.read(reinterpret_cast<char*>(realSig), 8);
	assert(std::equal(realSig, realSig + 8, expectedSig, expectedSig + 8));
	
	while (png)
		nextChunk(png);
	
	inflateBuffer(compressedData_, imageData_);
	unfilterImage();
}


void PNGFile::nextChunk(std::istream& png) {
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
			sd::log("Width : %d\n", width_);
			sd::log("Height: %d\n", height_);
			sd::log("Bits  : %d\n", (int)ihdr.BitDepth);
			sd::log("Kind  : %d\n", (int)ihdr.ColorType);
			sd::log("Compression : %d\n", (int)ihdr.Compression);
			sd::log("Filter : %d\n", (int)ihdr.Filter);
			sd::log("Interlace : %d\n", (int)ihdr.Interlace);
			
			assert(ihdr.BitDepth == 8);
			assert(ihdr.Interlace == 0);
			
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
			// some other chunk, ignore
			png.seekg(chdr.dataSize, std::ios::cur);
			break;
	}
	
	// skip crc
	png.seekg(4, std::ios::cur);
}


void PNGFile::unfilterImage() {
	auto addv = [](uint8_t a, uint8_t b) {
		uint32_t s32 = (uint32_t)a + (uint32_t)b;
		return s32 & 0xff;
	};
	
	auto rowPtr = imageData_.data();
	auto rowBytes = width_ * bpp_;
	auto rowPitch = rowBytes + 1;
	
	for (auto lineIx = 0u; lineIx < height_; ++lineIx) {
		LineFilter filter = (LineFilter)(*rowPtr++);
		assert(filter <= LFPaeth);

		auto row = rowPtr;
		auto bytes = rowBytes;
		
		if (filter != LFNone) {
			if (filter == LFSub) {
				// the first pixel has no left neighbor, so we skip it
				row += bpp_;
				bytes -= bpp_;

				while (bytes--) {
					*row = addv(*row, *(row - bpp_));
					++row;
				}
			}
			else if (filter == LFUp && lineIx > 0) {
				while (bytes--) {
					*row = addv(*row, *(row - rowPitch));
					++row;
				}
			}
			else if (filter == LFAverage) {
				if (lineIx == 0) {
					rowPtr += bpp_;
					bytes -= bpp_;
					
					while (bytes--) {
						*row = addv(*row, *(row - bpp_) >> 1);
						++row;
					}
				}
				else {
					while (bytes) {
						if (bytes + bpp_ <= rowBytes)
							*row = addv(*row, (*(row - bpp_) + *(row - rowPitch)) >> 1);
						else
							*row = addv(*row, (*(row - rowPitch)) >> 1);
						++row;
						--bytes;
					}
				}
			}
			else { // filter == LFPaeth
				auto paethPredictor = [](int a, int b, int c) -> int {
					using std::abs;
					// a = left, b = above, c = upper left
					auto p = a + b - c;    // initial estimate
					auto pa = abs(p - a);  // distances to a, b, c
					auto pb = abs(p - b);
					auto pc = abs(p - c);
					// return nearest of a,b,c,
					// breaking ties in order a,b,c.
					if (pa <= pb && pa <= pc)
						return a;
					if (pb <= pc)
						return b;
					return c;
				};
				
				if (lineIx == 0) {
					rowPtr += bpp_;
					bytes -= bpp_;
					
					while (bytes--) {
						*row = addv(*row, paethPredictor(*(row - bpp_), 0, 0));
						++row;
					}
				}
				else {
					while (bytes) {
						if (bytes + bpp_ <= rowBytes)
							*row = addv(*row, paethPredictor(*(row - bpp_), *(row - rowPitch), *(row - rowPitch - bpp_)));
						else
							*row = addv(*row, paethPredictor(0, *(row - rowPitch), 0));
						++row;
						--bytes;
					}
				}
			}
		}
		
		rowPtr += rowBytes;
	}
}


} // ns render
} // ns stardazed
