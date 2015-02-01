// ------------------------------------------------------------------
// render::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_TEXTURE_H
#define SD_RENDER_TEXTURE_H

#include "system/Config.hpp"

#include <string>
#include <memory>

namespace stardazed {
namespace render {


enum class ColourComponent {
	Red,
	Green,
	Blue,
	Alpha,
	Zero,
	One
};


enum class ImageDataFormat {
	None,
	
	R8,
	RG8,

	RGB8,
	BGR8,
	RGBA8,
	BGRA8,
	
	DXT1,
	DXT3,
	DXT5
};


struct ImageData {
	void* data;
	ImageDataFormat format;
	size32 width, height, size;
};


constexpr bool imageDataFormatIsCompressed(ImageDataFormat format) {
	return	format == ImageDataFormat::DXT1 ||
			format == ImageDataFormat::DXT3 ||
			format == ImageDataFormat::DXT5;
}


class TextureDataProvider {
public:
	virtual ~TextureDataProvider() {}
	
	virtual size32 width() const = 0;
	virtual size32 height() const = 0;
	virtual uint8 mipMapCount() const = 0;
	virtual ImageDataFormat format() const = 0;
	
	virtual ImageData imageDataForLevel(uint8 level) const = 0;
};


class DDSDataProvider : public TextureDataProvider {
	size32 width_, height_, mipMaps_;
	ImageDataFormat format_;
	std::unique_ptr<char[]> data_;
	
	size32 dataSizeForLevel(uint8 level) const;
	
public:
	DDSDataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return mipMaps_; }
	ImageDataFormat format() const override { return format_; }
	
	ImageData imageDataForLevel(uint8 level) const override;
};
	
	
class BMPDataProvider : public TextureDataProvider {
	size32 width_, height_;
	std::unique_ptr<char[]> data_;
	
public:
	BMPDataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return 1; }
	ImageDataFormat format() const override { return ImageDataFormat::BGR8; }
	
	ImageData imageDataForLevel(uint8 level) const override;
};


class PNGDataProvider : public TextureDataProvider {
	size32 width_, height_, size_;
	ImageDataFormat format_;
	std::unique_ptr<uint8[]> data_;

public:
	PNGDataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	uint8 mipMapCount() const override { return 1; }
	ImageDataFormat format() const override { return format_; }
	
	ImageData imageDataForLevel(uint8 level) const override;
};


enum class CubeMapFace {
	PosX,
	NegX,
	PosY,
	NegY,
	PosZ,
	NegZ
};


inline size32 dimensionAtMipLevel(size32 dim, uint8 level) {
	return std::max(1u, dim >> level);
}


} // ns render
} // ns stardazed

#endif
