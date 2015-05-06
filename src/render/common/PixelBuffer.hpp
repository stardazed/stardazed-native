// ------------------------------------------------------------------
// render::PixelBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIXELBUFFER_H
#define SD_RENDER_PIXELBUFFER_H

#include "system/Config.hpp"
#include "math/Algorithm.hpp"
#include "render/common/PixelFormat.hpp"

#include <memory>

namespace stardazed {
namespace render {


struct PixelCoordinate {
	uint32 x = 0, y = 0, z = 0;
};


struct PixelDimensions {
	// These defaults force you to enter at least 1 dimension (width)
	// but can leave the others as-is when they're not relevant.
	uint32 width = 0, height = 1, depth = 1;
};


constexpr size32 dataSizeBytesForPixelFormatAndDimensions(PixelFormat format, PixelDimensions size) {
	size32 elementSize = pixelFormatBytesPerElement(format);
	uint32 columns = size.width;
	uint32 rows = size.height;
	
	if (pixelFormatIsCompressed(format)) {
		// DXT 1, 3, 5
		columns = ((size.width + 3) / 4);
		rows    = ((size.height + 3) / 4);
	}
	
	return size.depth * rows * columns * elementSize;
}


struct PixelBuffer {
	void* data = nullptr;
	PixelFormat format = PixelFormat::None;
	PixelDimensions size;

	constexpr size32 bytesPerRow() const {
		return dataSizeBytesForPixelFormatAndDimensions(format, { size.width });
	}

	constexpr size32 bytesPerLayer() const {
		return dataSizeBytesForPixelFormatAndDimensions(format, { size.width, size.height });
	}

	constexpr size32 sizeBytes() const {
		return dataSizeBytesForPixelFormatAndDimensions(format, size);
	}
};


class PixelDataProvider {
public:
	virtual ~PixelDataProvider() = default;
	
	virtual PixelFormat format() const = 0;
	virtual PixelDimensions size() const = 0;
	virtual uint32 mipMapCount() const = 0;
	
	virtual PixelBuffer pixelBufferForLevel(uint32 level) const = 0;
};


class DDSDataProvider : public PixelDataProvider {
	uint32 width_, height_, mipMaps_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;
	
	size32 dataSizeForLevel(uint32 level) const;
	
public:
	DDSDataProvider(const std::string& resourcePath);

	PixelFormat format() const override { return format_; }
	PixelDimensions size() const override { return { width_, height_ }; }
	uint32 mipMapCount() const override { return mipMaps_; }
	
	PixelBuffer pixelBufferForLevel(uint32 level) const override;
};
	
	
class BMPDataProvider : public PixelDataProvider {
	uint32 width_, height_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;
	
public:
	BMPDataProvider(const std::string& resourcePath);
	
	PixelFormat format() const override { return format_; }
	PixelDimensions size() const override { return { width_, height_ }; }
	uint32 mipMapCount() const override { return 1; }
	
	PixelBuffer pixelBufferForLevel(uint32 level) const override;
};


class PNGDataProvider : public PixelDataProvider {
	uint32 width_, height_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;

public:
	PNGDataProvider(const std::string& resourcePath);
	
	PixelFormat format() const override { return format_; }
	PixelDimensions size() const override { return { width_, height_ }; }
	uint32 mipMapCount() const override { return 1; }
	
	PixelBuffer pixelBufferForLevel(uint32 level) const override;
};


class TGADataProvider : public PixelDataProvider {
	uint32 width_, height_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;
	
public:
	TGADataProvider(const std::string& resourcePath);
	
	PixelFormat format() const override { return format_; }
	PixelDimensions size() const override { return { width_, height_ }; }
	uint32 mipMapCount() const override { return 1; }
	
	PixelBuffer pixelBufferForLevel(uint32 level) const override;
};


class JPGDataProvider : public PixelDataProvider {
	uint32 width_, height_;
	std::unique_ptr<uint8[]> data_;

public:
	JPGDataProvider(const std::string& resourcePath);

	PixelFormat format() const override { return PixelFormat::RGBA8; }
	PixelDimensions size() const override { return { width_, height_ }; }
	uint32 mipMapCount() const override { return 1; }
	
	PixelBuffer pixelBufferForLevel(uint32 level) const override;
};


// -- utility function to return the proper provider based on
// -- the file extension of the resource path.
std::unique_ptr<PixelDataProvider> makePixelDataProviderForPath(const std::string& resourcePath);


// -- mip level size calc
constexpr uint32 dimensionAtMipLevel(size32 dim, uint8 level) {
	return math::max(1u, dim >> level);
}


} // ns render
} // ns stardazed

#endif
