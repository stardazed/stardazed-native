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
		case ImageDataFormat::R8: return GL_RED;
		case ImageDataFormat::RG8: return GL_RG;

		case ImageDataFormat::RGB8: return GL_RGB;
		case ImageDataFormat::BGR8: return GL_BGR;
		case ImageDataFormat::RGBA8: return GL_RGBA;
		case ImageDataFormat::BGRA8: return GL_BGRA;
			
		case ImageDataFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case ImageDataFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case ImageDataFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		
		default:
			assert(!"unknown texture pixel format");
	}
}


constexpr GLint glInternalFormatForImageDataFormat(ImageDataFormat format) {
	switch (format) {
		case ImageDataFormat::R8: return GL_R8;
		case ImageDataFormat::RG8: return GL_RG8;
			
		case ImageDataFormat::RGB8: return GL_RGB8;
		case ImageDataFormat::BGR8: return GL_RGB8;   // swizzled
		case ImageDataFormat::RGBA8: return GL_RGBA8;
		case ImageDataFormat::BGRA8: return GL_RGBA8; // swizzled
			
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
		case ImageDataFormat::R8: return GL_UNSIGNED_BYTE;
		case ImageDataFormat::RG8: return GL_UNSIGNED_BYTE;
		case ImageDataFormat::RGB8: return GL_UNSIGNED_BYTE;
		case ImageDataFormat::BGR8: return GL_UNSIGNED_BYTE;
		case ImageDataFormat::RGBA8: return GL_UNSIGNED_BYTE;
		case ImageDataFormat::BGRA8: return GL_UNSIGNED_BYTE;

		default:
			assert(!"unhandled image data format");
	}
}


constexpr GLenum glTargetForCubeMapFace(CubeMapFace face) {
	switch (face) {
		case CubeMapFace::PosX: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case CubeMapFace::NegX: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case CubeMapFace::PosY: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case CubeMapFace::NegY: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case CubeMapFace::PosZ: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case CubeMapFace::NegZ: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	}
}


void uploadSubImage2D(GLenum target, const ImageData& image, uint8 level, int offsetX, int offsetY) {
	auto glFormat = glImageFormatForImageDataFormat(image.format);
	
	if (imageDataFormatIsCompressed(image.format)) {
		glCompressedTexSubImage2D(target, level,
								  offsetX, offsetY, image.width, image.height,
								  glFormat, image.size, image.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForImageDataFormat(image.format);
		
		glTexSubImage2D(target, level,
						offsetX, offsetY, image.width, image.height,
						glFormat, glPixelType, image.data);
	}
}


//  _____         _                  ____  ____
// |_   _|____  _| |_ _   _ _ __ ___|___ \|  _ \
//   | |/ _ \ \/ / __| | | | '__/ _ \ __) | | | |
//   | |  __/>  <| |_| |_| | | |  __// __/| |_| |
//   |_|\___/_/\_\\__|\__,_|_|  \___|_____|____/
//
void Texture2D::allocate(size32 width, size32 height, uint8_t levels, ImageDataFormat format) {
	bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexStorage2D(GL_TEXTURE_2D, levels, glInternalFormatForImageDataFormat(format), width, height);

	width_ = width; height_ = height;
	format_ = format;
}


void Texture2D::uploadImageData(const ImageData& image, uint8 level) {
	assert(image.width == dimensionAtMipLevel(width_, level));
	assert(image.height == dimensionAtMipLevel(height_, level));
	assert(image.format == format_);
	
	uploadSubImage2D(GL_TEXTURE_2D, image, level, 0, 0);
}


void Texture2D::load(const TextureDataProvider& provider) {
	auto mipMaps = provider.mipMapCount();

	for (uint32 level = 0; level < mipMaps; ++level) {
		auto image = provider.imageDataForLevel(level);
		uploadImageData(image, level);
	 }
}


//  _____         _                   ____      _          __  __
// |_   _|____  _| |_ _   _ _ __ ___ / ___|   _| |__   ___|  \/  | __ _ _ __
//   | |/ _ \ \/ / __| | | | '__/ _ \ |  | | | | '_ \ / _ \ |\/| |/ _` | '_ \
//   | |  __/>  <| |_| |_| | | |  __/ |__| |_| | |_) |  __/ |  | | (_| | |_) |
//   |_|\___/_/\_\\__|\__,_|_|  \___|\____\__,_|_.__/ \___|_|  |_|\__,_| .__/
//                                                                     |_|
void TextureCubeMap::allocate(size32 side, uint8 levels, ImageDataFormat format) {
	bind();
	
	// FIXME: this needs to move to a Sampler object
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, glInternalFormatForImageDataFormat(format), side, side);
	
	if (format == ImageDataFormat::BGR8 || format == ImageDataFormat::BGRA8)
		setSwizzleMask(ColourComponent::Red, ColourComponent::Green, ColourComponent::Blue, ColourComponent::Alpha);
	
	side_ = side;
	format_ = format;
}


void TextureCubeMap::uploadFaceImageData(const ImageData& image, uint8 level, CubeMapFace face) {
	assert(image.width == dimensionAtMipLevel(side_, level));
	assert(image.width == image.height);
	assert(image.format == format_);

	uploadSubImage2D(glTargetForCubeMapFace(face), image, level, 0, 0);
}


void TextureCubeMap::loadFace(const TextureDataProvider& provider, CubeMapFace face) {
	for (uint32 level = 0; level < provider.mipMapCount(); ++level) {
		auto image = provider.imageDataForLevel(level);
		uploadFaceImageData(image, level, face);
	}
}


void TextureCubeMap::loadAllFaces(const TextureDataProvider& posX, const TextureDataProvider& negX,
								  const TextureDataProvider& posY, const TextureDataProvider& negY,
								  const TextureDataProvider& posZ, const TextureDataProvider& negZ)
{
	loadFace(posX, CubeMapFace::PosX);
	loadFace(negX, CubeMapFace::NegX);
	loadFace(posY, CubeMapFace::PosY);
	loadFace(negY, CubeMapFace::NegY);
	loadFace(posZ, CubeMapFace::PosZ);
	loadFace(negZ, CubeMapFace::NegZ);	
}


} // ns render
} // ns stardazed
