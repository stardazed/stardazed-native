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


enum class TextureKind {
	Single2D,
	CubeMap
};


enum class ImageDataFormat {
	RGBA8,
	
	DXT1,
	DXT3,
	DXT5
};


struct ImageData {
	void* data;
	ImageDataFormat format;
	size32 width, height, size;
};


class TextureDataProvider {
public:
	virtual ~TextureDataProvider() {}
	
	virtual size32 width() const = 0;
	virtual size32 height() const = 0;
	virtual size32 mipMapCount() const = 0;
	virtual ImageDataFormat format() const = 0;
	
	virtual ImageData imageDataForLevel(size32 level) const = 0;
};


class DDSDataProvider : public TextureDataProvider {
	size32 width_, height_, mipMaps_;
	ImageDataFormat format_;
	std::unique_ptr<char[]> data_;
	
public:
	DDSDataProvider(const std::string& resourcePath);
	
	size32 width() const override { return width_; }
	size32 height() const override { return height_; }
	size32 mipMapCount() const override { return mipMaps_; }
	ImageDataFormat format() const override { return format_; }
	
	ImageData imageDataForLevel(size32 level) const override;
};


} // ns render
} // ns stardazed

#endif
