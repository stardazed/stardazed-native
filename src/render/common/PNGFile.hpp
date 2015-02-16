// ------------------------------------------------------------------
// render::PNGFile - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PNGFILE_H
#define SD_RENDER_PNGFILE_H

#include "system/Config.hpp"
#include "filesystem/FileSystem.hpp"

#include <string>
#include <vector>

namespace stardazed {
namespace render {


// PNGFile implements a reader for a subset of the PNG file format's
// supported bit-depths, etc. It can read 8-bit component grayscale,
// grayscale + alpha, RGB and RGBA pixel formats. Palettes, 16-bit
// wide colour components, interlacing and modifying or informative
// chunks are not supported.

class PNGFile {
	uint32 width_ = 0, height_ = 0, bpp_ = 0;
	std::vector<uint8> compressedData_, imageData_;
	
	void nextChunk(fs::FileReadStream&);
	void unfilterImage(uint8* imageDataPtr);
	
public:
	PNGFile(const std::string& resourcePath);
	
	uint32 width() const { return width_; }
	uint32 height() const { return height_; }
	uint32 bytesPerPixel() const { return bpp_; }
	uint32 rowBytes() const { return width_ * bpp_; }
	
	uint8* rowDataPointer(uint32 row) {
		return imageData_.data() + (row * (rowBytes() + 1)) + 1;
	}
};


} // ns render
} // ns stardazed

#endif
