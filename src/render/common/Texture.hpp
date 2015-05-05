// ------------------------------------------------------------------
// render::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_TEXTURE_H
#define SD_RENDER_TEXTURE_H

#include "system/Config.hpp"
#include "math/Algorithm.hpp"
#include "render/common/PixelBuffer.hpp"

#include <string>
#include <memory>

namespace stardazed {
namespace render {


class PixelDataProvider {
public:
	virtual ~PixelDataProvider() {}
	
	virtual size32 width() const = 0;
	virtual size32 height() const = 0;
	virtual uint8 mipMapCount() const = 0;
	virtual PixelFormat format() const = 0;
	
	virtual PixelBuffer pixelBufferForLevel(uint8 level) const = 0;
};


class DDSDataProvider : public PixelDataProvider {
	size32 width_, height_, mipMaps_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;
	
	size32 dataSizeForLevel(uint8 level) const;
	
public:
	DDSDataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return mipMaps_; }
	PixelFormat format() const override { return format_; }
	
	PixelBuffer pixelBufferForLevel(uint8 level) const override;
};
	
	
class BMPDataProvider : public PixelDataProvider {
	size32 width_, height_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;
	
public:
	BMPDataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return 1; }
	PixelFormat format() const override { return format_; }
	
	PixelBuffer pixelBufferForLevel(uint8 level) const override;
};


class PNGDataProvider : public PixelDataProvider {
	size32 width_, height_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;

public:
	PNGDataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return 1; }
	PixelFormat format() const override { return format_; }
	
	PixelBuffer pixelBufferForLevel(uint8 level) const override;
};


class TGADataProvider : public PixelDataProvider {
	size32 width_, height_;
	PixelFormat format_;
	std::unique_ptr<uint8[]> data_;
	
public:
	TGADataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return 1; }
	PixelFormat format() const override { return format_; }
	
	PixelBuffer pixelBufferForLevel(uint8 level) const override;
};


class JPGDataProvider : public PixelDataProvider {
	size32 width_, height_;
	std::unique_ptr<uint8[]> data_;

public:
	JPGDataProvider(const std::string& resourcePath);

	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return 1; }
	PixelFormat format() const override { return PixelFormat::RGBA8; }
	
	PixelBuffer pixelBufferForLevel(uint8 level) const override;
};


// -- utility function to return the proper provider based on
// -- the file extension of the resource path.
std::unique_ptr<PixelDataProvider> makePixelDataProviderForPath(const std::string& resourcePath);


enum class CubeMapFace {
	PosX,
	NegX,
	PosY,
	NegY,
	PosZ,
	NegZ
};


inline size32 dimensionAtMipLevel(size32 dim, uint8 level) {
	return math::max(1u, dim >> level);
}


} // ns render
} // ns stardazed

#endif
