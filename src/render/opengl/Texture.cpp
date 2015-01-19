// ------------------------------------------------------------------
// render::opengl::Texture.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Texture.hpp"
#include <fstream>

namespace stardazed {
namespace render {


constexpr GLenum glTargetForTextureKind(const TextureKind kind) {
	switch (kind) {
		case TextureKind::Single2D: return GL_TEXTURE_2D;
		case TextureKind::CubeMap:  return GL_TEXTURE_CUBE_MAP;
	}
	
	assert(!"unknown texture kind");
}


constexpr GLint glImageFormatForTexturePixelFormat(const TexturePixelFormat format) {
	switch (format) {
		case TexturePixelFormat::RGBA8: return GL_RGBA8;
			
		case TexturePixelFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case TexturePixelFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case TexturePixelFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}
	
	assert(!"unknown texture pixel format");
}


Texture::Texture(TextureKind kind)
: kind_(kind)
, glTexTarget_(glTargetForTextureKind(kind))
{
	glGenTextures(1, &glTex_);
}


Texture::~Texture() {
	if (glTex_)
		glDeleteTextures(1, &glTex_);
}


void Texture::allocate(size32 width, size32 height, uint8_t levels, TexturePixelFormat format) {
	glBindTexture(glTexTarget_, glTex_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexStorage2D(glTexTarget_, levels, glImageFormatForTexturePixelFormat(format), width, height);
}


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


void Texture::loadDDS(const std::string& resourcePath) {
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
	auto buffer = std::make_unique<uint8[]>(dataSize);
	file.read(reinterpret_cast<char*>(buffer.get()), dataSize);
	
	TexturePixelFormat format;
	switch (header.ddspf.dwFourCC) {
		case 'DXT1': format = TexturePixelFormat::DXT1; break;
		case 'DXT3': format = TexturePixelFormat::DXT3; break;
		case 'DXT5': format = TexturePixelFormat::DXT5; break;
		default:
			assert(!"unknown pixel format of DDS file");
	}
	auto glFormat = glImageFormatForTexturePixelFormat(format);
	
	size32 blockSize = (format == TexturePixelFormat::DXT1) ? 8 : 16;
	size32 offset = 0;
	auto mipMaps = header.dwMipMapCount;
	auto width = header.dwWidth;
	auto height = header.dwHeight;
	
	allocate(width, height, mipMaps, format);
	
	for (uint32 level = 0; level < mipMaps; ++level) {
		size32 size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexSubImage2D(glTexTarget_, level,
								  0, 0, width, height,
								  glFormat, size, buffer.get() + offset);
	 
		offset += size;
		width  /= 2;
		height /= 2;
	}

	if (mipMaps == 1)
		glGenerateMipmap(glTexTarget_);
}


} // ns render
} // ns stardazed
