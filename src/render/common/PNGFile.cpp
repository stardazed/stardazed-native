// ------------------------------------------------------------------
// render::PNGFile.cpp - stardazed
// (c) 2015 by Arthur Langereis
//
// inflateBuffer code based on code from Mark Adler's zpipe.c sample code: http://zlib.net/zpipe.c
// png format and filter spec defined at: http://www.fileformat.info/format/png/corion.htm
// ------------------------------------------------------------------

#include "render/common/PNGFile.hpp"
#include "system/Logging.hpp"

#include "zlib.h"
#include <fstream>

namespace stardazed {
namespace render {


struct ChunkHeader {
	uint32 dataSize;
	uint32 chunkType;
};


constexpr uint32 fourCharCode(char a, char b, char c, char d) {
	return (d << 24) | (c << 16) | (b << 8) | a;
}


enum ChunkType : uint32 {
	HeaderChunk = fourCharCode('I','H','D','R'),
	ImageDataChunk = fourCharCode('I','D','A','T'),
	EndChunk = fourCharCode('I','E','N','D')
};

	
enum class ColorType : uint8 {
	Grayscale = 0,
	RGB = 2,
	Palette = 3,
	GrayscaleAlpha = 4,
	RGBA = 6
};


struct IHDRChunk {
	uint32 Width;        // Width of image in pixels
	uint32 Height;       // Height of image in pixels
	uint8 BitDepth;      // Bits per pixel or per sample
	uint8 ColorType;     // Color interpretation indicator
	uint8 Compression;   // Compression type indicator
	uint8 Filter;        // Filter type indicator
	uint8 Interlace;     // Type of interlacing scheme used
} __attribute__((__packed__));

static_assert(sizeof(IHDRChunk) == 13, "IHDRChunk type must be packed");


static int inflateBuffer(const std::vector<uint8_t>& source, std::vector<uint8_t>& dest) {
	z_stream strm;
	strm.zalloc = nullptr;
	strm.zfree = nullptr;
	strm.opaque = nullptr;
	strm.avail_in = static_cast<uint32>(source.size());
	strm.next_in = const_cast<uint8*>(source.data());
	strm.avail_out = static_cast<uint32>(dest.size());
	strm.next_out = dest.data();

	auto ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;
	
	ret = inflate(&strm, Z_NO_FLUSH);
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

	imageData_.resize((rowBytes() + 1) * height());
	inflateBuffer(compressedData_, imageData_);
	unfilterImage(imageData_.data());
}


void PNGFile::nextChunk(std::istream& png) {
	std::vector<uint8> tempData(8192); // seems to be common IDAT data size
	auto appender = std::back_inserter(compressedData_);
	
	ChunkHeader chdr;
	png.read(reinterpret_cast<char*>(&chdr), sizeof(ChunkHeader));
	chdr.dataSize = ntohl(chdr.dataSize);
	
	switch (chdr.chunkType) {
		case HeaderChunk:
		{
			IHDRChunk ihdr;
			png.read(reinterpret_cast<char*>(&ihdr), sizeof(IHDRChunk));
			width_ = ntohl(ihdr.Width);
			height_ = ntohl(ihdr.Height);
			sd::log("Width : %d\n", width_);
			sd::log("Height: %d\n", height_);
			sd::log("Kind  : %d\n", (int)ihdr.ColorType);
			
			assert(ihdr.BitDepth == 8);
			assert((ColorType)ihdr.ColorType != ColorType::Palette);
			assert(ihdr.Filter == 0);
			assert(ihdr.Interlace == 0);
			
			switch ((ColorType)ihdr.ColorType) {
				case ColorType::RGB: bpp_ = 3; break;
				case ColorType::GrayscaleAlpha: bpp_ = 2; break;
				case ColorType::RGBA: bpp_ = 4; break;
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


void PNGFile::unfilterImage(uint8* imageDataPtr) {
	auto addv = [](uint32 a, uint32 b) {
		return (a + b) & 0xff;
	};
	
	auto rowPtr = imageDataPtr;
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
			else if (filter == LFUp) {
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
					auto rowAbove = row - rowPitch;
					auto rowLeft = row;                 // row and rowAbove are first pushed +bpp_ bytes in first loop
					auto rowAboveLeft = row - rowPitch; // placing these to row(Above) - bpp_ positions for the 2nd loop
					
					auto firstBytes = bpp_;
					bytes -= bpp_;
					
					while (firstBytes--) {
						*row = addv(*row, paethPredictor(0, *rowAbove, 0));
						++row;
						++rowAbove;
					}
					
					while (bytes--) {
						*row = addv(*row, paethPredictor(*rowLeft, *rowAbove, *rowAboveLeft));
						
						++row;
						++rowLeft;
						++rowAbove;
						++rowAboveLeft;
					}
				}
			}
		}
		
		rowPtr += rowBytes;
	}
}


} // ns render
} // ns stardazed
