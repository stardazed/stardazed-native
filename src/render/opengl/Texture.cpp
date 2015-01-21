// ------------------------------------------------------------------
// render::opengl::Texture.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Texture.hpp"
#include <fstream>

namespace stardazed {
namespace render {


constexpr GLint glImageFormatForImageDataFormat(ImageDataFormat format) {
	switch (format) {
		case ImageDataFormat::RGBA8: return GL_RGBA;
			
		case ImageDataFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case ImageDataFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case ImageDataFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		
		default:
			assert(!"unknown texture pixel format");
	}
}


constexpr GLint glSizedImageFormatForImageDataFormat(ImageDataFormat format) {
	switch (format) {
		case ImageDataFormat::RGBA8: return GL_RGBA8;
			
		case ImageDataFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case ImageDataFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case ImageDataFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

		default:
			assert(!"unknown texture pixel format");
	}
}


constexpr GLenum glPixelDataTypeForImageDataFormat(ImageDataFormat format) {
	assert(! imageDataFormatIsCompressed(format));
	
	switch (format) {
		case ImageDataFormat::RGBA8: return GL_UNSIGNED_BYTE;

		default:
			assert(!"unhandled image data format");
	}
}


constexpr GLenum glTargetForCubeMapFace(CubeMapFace face) {
	switch (face) {
		case CubeMapFace::NegX: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case CubeMapFace::PosX: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case CubeMapFace::NegY: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case CubeMapFace::PosY: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case CubeMapFace::NegZ: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		case CubeMapFace::PosZ: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	}
}


void Texture2D::allocate(size32 width, size32 height, uint8_t levels, ImageDataFormat format) {
	bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexStorage2D(GL_TEXTURE_2D, levels, glSizedImageFormatForImageDataFormat(format), width, height);

	width_ = width; height_ = height;
	format_ = format;
}


void Texture2D::uploadImageData(const ImageData& image, uint8 level) {
	assert(image.width == width_ >> level);
	assert(image.height == height_ >> level);
	assert(image.format == format_);
	
	auto glFormat = glImageFormatForImageDataFormat(image.format);

	if (imageDataFormatIsCompressed(image.format)) {
		glCompressedTexSubImage2D(GL_TEXTURE_2D, level,
								  0, 0, image.width, image.height,
								  glFormat, image.size, image.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForImageDataFormat(image.format);

		glTexSubImage2D(GL_TEXTURE_2D, level,
						0, 0, image.width, image.height,
						glFormat, glPixelType, image.data);
	}
}


void Texture2D::load(const TextureDataProvider& provider) {
	auto width    = provider.width();
	auto height   = provider.height();
	auto mipMaps  = provider.mipMapCount();

	allocate(width, height, mipMaps, provider.format());

	for (uint32 level = 0; level < mipMaps; ++level) {
		auto image = provider.imageDataForLevel(level);
		uploadImageData(image, level);
	 }
}


} // ns render
} // ns stardazed
