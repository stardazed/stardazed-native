// ------------------------------------------------------------------
// render::opengl::Texture.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Texture.hpp"

namespace stardazed {
namespace render {


Texture::Texture(const TextureDescriptor& td)
: width_(td.width)
, height_(td.height)
, depth_(td.height)
, layers_(td.layers)
, mipmaps_(td.mipmaps)
, samples_(td.samples)
, pixelFormat_(td.pixelFormat)
{
	auto sizedFormat = glInternalFormatForPixelFormat(pixelFormat_);

	// -- special case for RenderBuffer, all other paths gen a texture
	if (td.textureClass == TextureClass::Tex2D) {
		if (td.layers == 1) {
			if (td.usageHint == TextureUsageHint::RenderTargetOnly) {
				// use a RenderBuffer
				glTarget_ = GL_RENDERBUFFER;
				glGenRenderbuffers(1, &glTex_);
				glBindRenderbuffer(glTarget_, glTex_);
				glRenderbufferStorageMultisample(glTarget_, td.samples, sizedFormat, td.width, td.height);
				glBindRenderbuffer(glTarget_, 0);
				
				return;
			}
		}
	}

	// -- normal texture
	glGenTextures(1, &glTex_);
	
	if (td.textureClass == TextureClass::Tex2D) {
		if (td.layers == 1) {
			if (td.samples == 1) {
				glTarget_ = GL_TEXTURE_2D;
				glBindTexture(glTarget_, glTex_);
				glTexStorage2D(glTarget_, td.mipmaps, sizedFormat, td.width, td.height);
			}
			else {
				glTarget_ = GL_TEXTURE_2D_MULTISAMPLE;
				glBindTexture(glTarget_, glTex_);
				glTexImage2DMultisample(glTarget_, td.samples, sizedFormat, td.width, td.height, GL_TRUE);
			}
		}
		else {
			glTarget_ = GL_TEXTURE_2D_ARRAY;
			glBindTexture(glTarget_, glTex_);
			glTexStorage3D(glTarget_, td.mipmaps, sizedFormat, td.width, td.height, td.layers);
		}
	}
	else if (td.textureClass == TextureClass::TexCube) {
		glTarget_ = GL_TEXTURE_CUBE_MAP;
		glBindTexture(glTarget_, glTex_);
		glTexStorage2D(glTarget_, td.mipmaps, sizedFormat, td.width, td.height);
	}
	else if (td.textureClass == TextureClass::Tex1D) {
		if (td.layers == 1) {
			glTarget_ = GL_TEXTURE_1D;
			glBindTexture(glTarget_, glTex_);
			glTexStorage1D(glTarget_, td.mipmaps, sizedFormat, td.width);
		}
		else {
			glTarget_ = GL_TEXTURE_1D_ARRAY;
			glBindTexture(glTarget_, glTex_);
			glTexStorage2D(glTarget_, td.mipmaps, sizedFormat, td.width, td.layers);
		}
	}
	else if (td.textureClass == TextureClass::Tex3D) {
		glTarget_ = GL_TEXTURE_3D;
		glBindTexture(glTarget_, glTex_);
		glTexStorage3D(glTarget_, td.mipmaps, sizedFormat, td.width, td.height, td.depth);
	}
	
	glBindTexture(glTarget_, 0);
}


Texture::~Texture() {
	if (glTex_ > 0) {
		if (glTarget_ == GL_RENDERBUFFER)
			glDeleteRenderbuffers(1, &glTex_);
		else
			glDeleteTextures(1, &glTex_);
	}
}


void Texture::writePixels(const PixelBuffer&, uint32 x, uint32 y, uint32 mipmapLevel, uint32 layer) {
	
}


PixelBuffer Texture::readPixels(uint32 x, uint32 y, uint32 width, uint32 height, uint32 mipmapLevel, uint32 layer) {
	return {};
}


TextureClass Texture::textureClass() const {
	if (glTarget_ == GL_TEXTURE_CUBE_MAP)
		return TextureClass::TexCube;
	if (glTarget_ == GL_TEXTURE_3D)
		return TextureClass::Tex3D;
	if (glTarget_ == GL_TEXTURE_1D || glTarget_ == GL_TEXTURE_1D_ARRAY)
		return TextureClass::Tex1D;
	return TextureClass::Tex2D;
}


bool Texture::frameBufferOnly() const {
	return glTarget_ == GL_TEXTURE_2D_MULTISAMPLE || glTarget_ == GL_RENDERBUFFER;
}


bool Texture::renderTargetOnly() const {
	return glTarget_ == GL_RENDERBUFFER;
}














void uploadSubImage2D(GLenum target, const PixelBuffer& pixelBuffer, uint8 level, int offsetX, int offsetY) {
	auto glFormat = glImageFormatForPixelFormat(pixelBuffer.format);
	
	if (pixelFormatIsCompressed(pixelBuffer.format)) {
		glCompressedTexSubImage2D(target, level,
								  offsetX, offsetY, pixelBuffer.width, pixelBuffer.height,
								  glFormat, pixelBuffer.sizeBytes, pixelBuffer.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixelBuffer.format);
		
		glTexSubImage2D(target, level,
						offsetX, offsetY, pixelBuffer.width, pixelBuffer.height,
						glFormat, glPixelType, pixelBuffer.data);
	}
}


//  _____         _                  ____  ____
// |_   _|____  _| |_ _   _ _ __ ___|___ \|  _ \
//   | |/ _ \ \/ / __| | | | '__/ _ \ __) | | | |
//   | |  __/>  <| |_| |_| | | |  __// __/| |_| |
//   |_|\___/_/\_\\__|\__,_|_|  \___|_____|____/
//
void Texture2D::allocate(size32 width, size32 height, uint8 levels, PixelFormat format) {
	bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexStorage2D(target(), levels, glInternalFormatForPixelFormat(format), width, height);

	width_ = width; height_ = height;
	format_ = format;
}


void Texture2D::uploadPixelBuffer(const PixelBuffer& pixelBuffer, uint8 level) {
	assert(pixelBuffer.width == dimensionAtMipLevel(width_, level));
	assert(pixelBuffer.height == dimensionAtMipLevel(height_, level));
	assert(pixelBuffer.format == format_);
	
	uploadSubImage2D(target(), pixelBuffer, level, 0, 0);
}


void Texture2D::load(const PixelDataProvider& provider) {
	auto mipMaps = provider.mipMapCount();

	for (uint32 level = 0; level < mipMaps; ++level) {
		auto pixelBuffer = provider.pixelBufferForLevel(level);
		uploadPixelBuffer(pixelBuffer, level);
	 }
}


void Texture2D::setupWithDataProvider(const PixelDataProvider& provider) {
	allocate(provider.width(), provider.height(), provider.mipMapCount(), provider.format());
	load(provider);
}


//  _____         _                  ____  ____  __  __       _ _   _                           _
// |_   _|____  _| |_ _   _ _ __ ___|___ \|  _ \|  \/  |_   _| | |_(_)___  __ _ _ __ ___  _ __ | | ___
//   | |/ _ \ \/ / __| | | | '__/ _ \ __) | | | | |\/| | | | | | __| / __|/ _` | '_ ` _ \| '_ \| |/ _ \
//   | |  __/>  <| |_| |_| | | |  __// __/| |_| | |  | | |_| | | |_| \__ \ (_| | | | | | | |_) | |  __/
//   |_|\___/_/\_\\__|\__,_|_|  \___|_____|____/|_|  |_|\__,_|_|\__|_|___/\__,_|_| |_| |_| .__/|_|\___|
//                                                                                       |_|

void Texture2DMultisample::allocate(size32 width, size32 height, uint8 samples, PixelFormat format) {
	// multisample textures may not be a compressed format
	assert(! pixelFormatIsCompressed(format));

	bind();
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2DMultisample(target(), samples, glInternalFormatForPixelFormat(format), width, height, GL_TRUE);
	
	width_ = width; height_ = height;
	format_ = format;
	samples_ = samples;
}


//  _____         _                   ____      _          __  __
// |_   _|____  _| |_ _   _ _ __ ___ / ___|   _| |__   ___|  \/  | __ _ _ __
//   | |/ _ \ \/ / __| | | | '__/ _ \ |  | | | | '_ \ / _ \ |\/| |/ _` | '_ \
//   | |  __/>  <| |_| |_| | | |  __/ |__| |_| | |_) |  __/ |  | | (_| | |_) |
//   |_|\___/_/\_\\__|\__,_|_|  \___|\____\__,_|_.__/ \___|_|  |_|\__,_| .__/
//                                                                     |_|
void TextureCubeMap::allocate(size32 side, uint8 levels, PixelFormat format) {
	bind();
	
	// FIXME: this needs to move to a Sampler object
	glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexStorage2D(target(), levels, glInternalFormatForPixelFormat(format), side, side);
	
	if (format == PixelFormat::BGR8 || format == PixelFormat::BGRA8)
		setSwizzleMask(ColourComponent::Red, ColourComponent::Green, ColourComponent::Blue, ColourComponent::Alpha);
	
	side_ = side;
	format_ = format;
}


void TextureCubeMap::uploadFacePixelBuffer(const PixelBuffer& pixelBuffer, uint8 level, CubeMapFace face) {
	assert(pixelBuffer.width == dimensionAtMipLevel(side_, level));
	assert(pixelBuffer.width == pixelBuffer.height);
	assert(pixelBuffer.format == format_);

	uploadSubImage2D(glTargetForCubeMapFace(face), pixelBuffer, level, 0, 0);
}


void TextureCubeMap::loadFace(const PixelDataProvider& provider, CubeMapFace face) {
	for (uint32 level = 0; level < provider.mipMapCount(); ++level) {
		auto pixelBuffer = provider.pixelBufferForLevel(level);
		uploadFacePixelBuffer(pixelBuffer, level, face);
	}
}


void TextureCubeMap::loadAllFaces(const PixelDataProvider& posX, const PixelDataProvider& negX,
								  const PixelDataProvider& posY, const PixelDataProvider& negY,
								  const PixelDataProvider& posZ, const PixelDataProvider& negZ)
{
	loadFace(posX, CubeMapFace::PosX);
	loadFace(negX, CubeMapFace::NegX);
	loadFace(posY, CubeMapFace::PosY);
	loadFace(negY, CubeMapFace::NegY);
	loadFace(posZ, CubeMapFace::PosZ);
	loadFace(negZ, CubeMapFace::NegZ);	
}


void TextureCubeMap::setupWithDataProviders(const PixelDataProvider& posX, const PixelDataProvider& negX,
											const PixelDataProvider& posY, const PixelDataProvider& negY,
											const PixelDataProvider& posZ, const PixelDataProvider& negZ)
{
	allocate(posX.width(), posX.mipMapCount(), posX.format());
	loadAllFaces(posX, negX, posY, negY, posZ, negZ);
}


} // ns render
} // ns stardazed
