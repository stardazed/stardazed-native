// ------------------------------------------------------------------
// render::opengl::Texture.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Texture.hpp"

namespace stardazed {
namespace render {


Texture::Texture(const TextureDescriptor& td)
: textureClass_(td.textureClass)
, size_(td.size)
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
				glRenderbufferStorageMultisample(glTarget_, td.samples, sizedFormat, width(), height());
				glBindRenderbuffer(glTarget_, 0);
				
				return;
			}
		}
	}

	// -- normal texture
	glGenTextures(1, &glTex_);
	
	if (td.textureClass == TextureClass::Tex2D) {
		// -- 2D textures

		if (td.layers == 1) {
			if (td.samples == 1) {
				glTarget_ = GL_TEXTURE_2D;
				glBindTexture(glTarget_, glTex_);
				glTexStorage2D(glTarget_, td.mipmaps, sizedFormat, width(), height());
			}
			else {
				glTarget_ = GL_TEXTURE_2D_MULTISAMPLE;
				glBindTexture(glTarget_, glTex_);
				glTexImage2DMultisample(glTarget_, td.samples, sizedFormat, width(), height(), GL_TRUE);
			}
		}
		else {
			glTarget_ = GL_TEXTURE_2D_ARRAY;
			glBindTexture(glTarget_, glTex_);
			glTexStorage3D(glTarget_, td.mipmaps, sizedFormat, width(), height(), td.layers);
		}
	}
	else if (td.textureClass == TextureClass::TexCube) {
		// -- Cube-map textures
		
		glTarget_ = GL_TEXTURE_CUBE_MAP;
		glBindTexture(glTarget_, glTex_);
		glTexStorage2D(glTarget_, td.mipmaps, sizedFormat, width(), height());
	}
	else if (td.textureClass == TextureClass::Tex1D) {
		// -- 1D textures
		
		if (td.layers == 1) {
			glTarget_ = GL_TEXTURE_1D;
			glBindTexture(glTarget_, glTex_);
			glTexStorage1D(glTarget_, td.mipmaps, sizedFormat, width());
		}
		else {
			glTarget_ = GL_TEXTURE_1D_ARRAY;
			glBindTexture(glTarget_, glTex_);
			glTexStorage2D(glTarget_, td.mipmaps, sizedFormat, width(), td.layers);
		}
	}
	else if (td.textureClass == TextureClass::Tex3D) {
		// -- 3D textures
		
		glTarget_ = GL_TEXTURE_3D;
		glBindTexture(glTarget_, glTex_);
		glTexStorage3D(glTarget_, td.mipmaps, sizedFormat, width(), height(), depth());
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



void Texture::write1DPixels(const PixelBuffer& pixels, uint32 offX, uint32 mipmapLevel) {
	glBindTexture(glTarget_, glTex_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, pixels.requiredRowAlignment());

	auto glFormat = glImageFormatForPixelFormat(pixels.format);
	
	if (pixelFormatIsCompressed(pixels.format)) {
		glCompressedTexSubImage1D(glTarget_, mipmapLevel,
								  offX, pixels.size.width,
								  glFormat, pixels.sizeBytes(), pixels.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixels.format);
		
		glTexSubImage1D(glTarget_, mipmapLevel,
						offX, pixels.size.width,
						glFormat, glPixelType, pixels.data);
	}
	
	glBindTexture(glTarget_, 0);
}


void Texture::write2DPixels(GLenum pixelTarget, const PixelBuffer& pixels, uint32 offX, uint32 offY, uint32 mipmapLevel) {
	glBindTexture(glTarget_, glTex_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, pixels.requiredRowAlignment());

	auto glFormat = glImageFormatForPixelFormat(pixels.format);
	
	if (pixelFormatIsCompressed(pixels.format)) {
		glCompressedTexSubImage2D(pixelTarget, mipmapLevel,
								  offX, offY, pixels.size.width, pixels.size.height,
								  glFormat, pixels.sizeBytes(), pixels.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixels.format);
		
		glTexSubImage2D(pixelTarget, mipmapLevel,
						offX, offY, pixels.size.width, pixels.size.height,
						glFormat, glPixelType, pixels.data);
	}

	glBindTexture(glTarget_, 0);
}


void Texture::write3DPixels(const PixelBuffer& pixels, PixelCoordinate origin, uint32 mipmapLevel) {
	glBindTexture(glTarget_, glTex_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, pixels.requiredRowAlignment());

	auto glFormat = glImageFormatForPixelFormat(pixels.format);
	
	if (pixelFormatIsCompressed(pixels.format)) {
		glCompressedTexSubImage3D(glTarget_, mipmapLevel,
								  origin.x, origin.y, origin.z,
								  pixels.size.width, pixels.size.height, pixels.size.depth,
								  glFormat, pixels.sizeBytes(), pixels.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixels.format);
		
		glTexSubImage3D(glTarget_, mipmapLevel,
						origin.x, origin.y, origin.z,
						pixels.size.width, pixels.size.height, pixels.size.depth,
						glFormat, glPixelType, pixels.data);
	}
	
	glBindTexture(glTarget_, 0);
}


void Texture::writePixels(const PixelBuffer& pixels, PixelCoordinate origin, uint32 mipmapLevel, uint32 baseLayer) {
	if (frameBufferOnly()) {
		assert(!"Tried to write pixel data to an attachment-only texture");
		return;
	}
	
	assert(mipmapLevel < mipmaps_);
	
	if (glTarget_ == GL_TEXTURE_2D || glTarget_ == GL_TEXTURE_1D_ARRAY) {
		assert(origin.z == 0);

		uint32 offX = origin.x,
			   offY = origin.y;

		if (glTarget_ == GL_TEXTURE_1D_ARRAY)
			offY = baseLayer;

		write2DPixels(glTarget_, pixels, offX, offY, mipmapLevel);
	}
	else if (glTarget_ == GL_TEXTURE_CUBE_MAP) {
		assert(baseLayer < 6);

		writePixels(pixels, origin, mipmapLevel, (CubeMapFace)baseLayer);
	}
	else if (glTarget_ == GL_TEXTURE_1D) {
		assert(origin.y == 0);
		assert(origin.z == 0);

		write1DPixels(pixels, origin.x, mipmapLevel);
	}
	else /* glTarget_ in [GL_TEXTURE_2D_ARRAY, GL_TEXTURE_3D] */ {
		if (glTarget_ == GL_TEXTURE_2D_ARRAY)
			origin.z = baseLayer;

		write3DPixels(pixels, origin, mipmapLevel);
	}
}


void Texture::writePixels(const PixelBuffer& pixels, PixelCoordinate origin, uint32 mipmapLevel, CubeMapFace face) {
	assert(textureClass_ == TextureClass::TexCube);
	assert(origin.z == 1);

	auto faceTarget = glTargetForCubeMapFace(face);
	write2DPixels(faceTarget, pixels, origin.x, origin.y, mipmapLevel);
}


void writeProviderPixels(const PixelDataProvider& provider, Texture& texture, PixelCoordinate origin, MipMapRange readRange, MipMapRange writeRange) {
	assert(readRange.baseLevel < provider.mipMapCount());
	assert(writeRange.baseLevel < texture.mipmaps());

	auto sourceMipMaps = math::min(provider.mipMapCount() - readRange.baseLevel, readRange.numLevels),
		 destMipMaps   = math::min(texture.mipmaps() - writeRange.baseLevel, writeRange.numLevels),
		 mipMapsToCopy = math::min(sourceMipMaps, destMipMaps);
	
	for (uint32 level = 0; level < mipMapsToCopy; ++level) {
		auto pixelBuffer = provider.pixelBufferForLevel(readRange.baseLevel + level);
		texture.writePixels(pixelBuffer, origin, writeRange.baseLevel + level);
	}
}


// FIXME: the current setup does not deal with layers at all
// fine for now where Providers are all 2D textures, but to be fixed in the future.
Texture* textureFromProvider(const PixelDataProvider& provider, TextureClass textureClass, UseMipMaps useMipMaps) {
	auto texDesc = makeTexDescFromPixelBuffer(provider.pixelBufferForLevel(0), textureClass);
	if (useMipMaps == UseMipMaps::No)
		texDesc.mipmaps = 1;

	auto texture = new Texture(texDesc);
	writeProviderPixels(provider, *texture);

	return texture;
}


//PixelBuffer Texture::readPixels(PixelCoordinate origin, PixelDimensions size, uint32 mipmapLevel, uint32 layer) {
//	if (frameBufferOnly()) {
//		assert(!"Tried to read pixel data from an attachment-only texture");
//		return {};
//	}
//
//	// FIXME: NYI
//	return {};
//}


bool Texture::frameBufferOnly() const {
	return glTarget_ == GL_TEXTURE_2D_MULTISAMPLE || glTarget_ == GL_RENDERBUFFER;
}


bool Texture::renderTargetOnly() const {
	return glTarget_ == GL_RENDERBUFFER;
}














void uploadSubImage2D(GLenum target, const PixelBuffer& pixelBuffer, uint32 level, int offsetX, int offsetY) {
	auto glFormat = glImageFormatForPixelFormat(pixelBuffer.format);
	
	if (pixelFormatIsCompressed(pixelBuffer.format)) {
		glCompressedTexSubImage2D(target, level,
								  offsetX, offsetY, pixelBuffer.size.width, pixelBuffer.size.height,
								  glFormat, pixelBuffer.sizeBytes(), pixelBuffer.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixelBuffer.format);
		
		glTexSubImage2D(target, level,
						offsetX, offsetY, pixelBuffer.size.width, pixelBuffer.size.height,
						glFormat, glPixelType, pixelBuffer.data);
	}
}


//  _____         _                  ____  ____
// |_   _|____  _| |_ _   _ _ __ ___|___ \|  _ \
//   | |/ _ \ \/ / __| | | | '__/ _ \ __) | | | |
//   | |  __/>  <| |_| |_| | | |  __// __/| |_| |
//   |_|\___/_/\_\\__|\__,_|_|  \___|_____|____/
//
void Texture2D::allocate(size32 width, size32 height, uint32 levels, PixelFormat format) {
	bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexStorage2D(target(), levels, glInternalFormatForPixelFormat(format), width, height);

	width_ = width; height_ = height;
	format_ = format;
}


void Texture2D::uploadPixelBuffer(const PixelBuffer& pixelBuffer, uint32 level) {
	assert(pixelBuffer.size.width == dimensionAtMipLevel(width_, level));
	assert(pixelBuffer.size.height == dimensionAtMipLevel(height_, level));
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
	allocate(provider.size().width, provider.size().height, provider.mipMapCount(), provider.format());
	load(provider);
}


//  _____         _                  ____  ____  __  __       _ _   _                           _
// |_   _|____  _| |_ _   _ _ __ ___|___ \|  _ \|  \/  |_   _| | |_(_)___  __ _ _ __ ___  _ __ | | ___
//   | |/ _ \ \/ / __| | | | '__/ _ \ __) | | | | |\/| | | | | | __| / __|/ _` | '_ ` _ \| '_ \| |/ _ \
//   | |  __/>  <| |_| |_| | | |  __// __/| |_| | |  | | |_| | | |_| \__ \ (_| | | | | | | |_) | |  __/
//   |_|\___/_/\_\\__|\__,_|_|  \___|_____|____/|_|  |_|\__,_|_|\__|_|___/\__,_|_| |_| |_| .__/|_|\___|
//                                                                                       |_|

void Texture2DMultisample::allocate(size32 width, size32 height, uint32 samples, PixelFormat format) {
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
void TextureCubeMap::allocate(size32 side, uint32 levels, PixelFormat format) {
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


void TextureCubeMap::uploadFacePixelBuffer(const PixelBuffer& pixelBuffer, uint32 level, CubeMapFace face) {
	assert(pixelBuffer.size.width == dimensionAtMipLevel(side_, level));
	assert(pixelBuffer.size.width == pixelBuffer.size.height);
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
	allocate(posX.size().width, posX.mipMapCount(), posX.format());
	loadAllFaces(posX, negX, posY, negY, posZ, negZ);
}


} // ns render
} // ns stardazed
