// ------------------------------------------------------------------
// render::opengl::Texture.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Texture.hpp"

namespace stardazed {
namespace render {


Texture::Texture(const TextureDescriptor& td)
: textureClass_(td.textureClass)
, dim_(td.dim)
, layers_(td.layers)
, mipmaps_(td.mipmaps)
, samples_(td.samples)
, pixelFormat_(td.pixelFormat)
{
	// these are invariant across all texture types
	assert(layers_ > 0);
	assert(mipmaps_ > 0);
	assert(samples_ > 0);
	assert(width() > 0);

	auto sizedFormat = glInternalFormatForPixelFormat(pixelFormat_);

	// -- special case for RenderBuffer, all other paths gen a texture
	if (td.textureClass == TextureClass::Tex2D) {
		if (td.layers == 1) {
			if (td.usageHint == TextureUsageHint::RenderTargetOnly) {
				// use a RenderBuffer
				glTarget_ = GL_RENDERBUFFER;

				// renderbuffer textures may not use a compressed pixel format
				assert(! pixelFormatIsCompressed(pixelFormat_));
				assert(mipmaps() == 1);
				assert(layers() == 1);

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
				// multisample textures may not use a compressed pixel format
				assert(! pixelFormatIsCompressed(pixelFormat_));
				assert(mipmaps() == 1);

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
								  offX, pixels.dim.width,
								  glFormat, pixels.sizeBytes(), pixels.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixels.format);
		
		glTexSubImage1D(glTarget_, mipmapLevel,
						offX, pixels.dim.width,
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
								  offX, offY, pixels.dim.width, pixels.dim.height,
								  glFormat, pixels.sizeBytes(), pixels.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixels.format);
		
		glTexSubImage2D(pixelTarget, mipmapLevel,
						offX, offY, pixels.dim.width, pixels.dim.height,
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
								  pixels.dim.width, pixels.dim.height, pixels.dim.depth,
								  glFormat, pixels.sizeBytes(), pixels.data);
	}
	else {
		auto glPixelType = glPixelDataTypeForPixelFormat(pixels.format);
		
		glTexSubImage3D(glTarget_, mipmapLevel,
						origin.x, origin.y, origin.z,
						pixels.dim.width, pixels.dim.height, pixels.dim.depth,
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
		// FIXME: is this kind-of-mimicking of Metal setup wise?

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


// FIXME: the current setup does not deal with layers at all
// fine for now where Providers are all 2D textures, but to be fixed in the future.
void Texture::writeProviderPixels(const PixelDataProvider& provider, PixelCoordinate origin,
								  MipMapRange readRange, MipMapRange writeRange)
{
	assert(textureClass_ != TextureClass::TexCube);
	assert(readRange.baseLevel < provider.mipMapCount());
	assert(writeRange.baseLevel < mipmaps());

	auto sourceMipMaps = math::min(provider.mipMapCount() - readRange.baseLevel, readRange.numLevels),
		 destMipMaps   = math::min(mipmaps() - writeRange.baseLevel, writeRange.numLevels),
		 mipMapsToCopy = math::min(sourceMipMaps, destMipMaps);
	
	for (uint32 level = 0; level < mipMapsToCopy; ++level) {
		auto pixelBuffer = provider.pixelBufferForLevel(readRange.baseLevel + level);
		writePixels(pixelBuffer, origin, writeRange.baseLevel + level);
	}
}


// FIXME: copied source from base writeProviderPixels
void Texture::writeProviderPixels(const PixelDataProvider& provider, CubeMapFace face, PixelCoordinate origin,
						 MipMapRange readRange, MipMapRange writeRange)
{
	assert(textureClass_ == TextureClass::TexCube);
	assert(readRange.baseLevel < provider.mipMapCount());
	assert(writeRange.baseLevel < mipmaps());
	
	auto sourceMipMaps = math::min(provider.mipMapCount() - readRange.baseLevel, readRange.numLevels),
		 destMipMaps   = math::min(mipmaps() - writeRange.baseLevel, writeRange.numLevels),
		 mipMapsToCopy = math::min(sourceMipMaps, destMipMaps);
	
	auto faceTarget = glTargetForCubeMapFace(face);

	for (uint32 level = 0; level < mipMapsToCopy; ++level) {
		auto pixelBuffer = provider.pixelBufferForLevel(readRange.baseLevel + level);
		write2DPixels(faceTarget, pixelBuffer, origin.x, origin.y, writeRange.baseLevel + level);
	}
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


// FIXME: the UseMipMaps params here only _restrict_ the use of mipmaps,
// needs to be extended that it will allocate and, if necessary, generate them
// if it is set to Yes regardless of Provider data.

	
Texture* textureFromProvider(const PixelDataProvider& provider, TextureClass textureClass, UseMipMaps useMipMaps) {
	assert(textureClass != TextureClass::TexCube);

	auto texDesc = makeTexDescFromPixelDataProvider(provider, textureClass);
	if (useMipMaps == UseMipMaps::No)
		texDesc.mipmaps = 1;
	
	auto texture = new Texture(texDesc);
	texture->writeProviderPixels(provider);
	
	return texture;
}


Texture* textureCubeMapFromProviders(const PixelDataProvider& posX, const PixelDataProvider& negX,
									 const PixelDataProvider& posY, const PixelDataProvider& negY,
									 const PixelDataProvider& posZ, const PixelDataProvider& negZ,
									 UseMipMaps useMipMaps)
{
	auto texDesc = makeTexDescFromPixelDataProvider(posX, TextureClass::TexCube);
	if (useMipMaps == UseMipMaps::No)
		texDesc.mipmaps = 1;

	auto texture = new Texture(texDesc);
	texture->writeProviderPixels(posX, CubeMapFace::PosX);
	texture->writeProviderPixels(negX, CubeMapFace::NegX);
	texture->writeProviderPixels(posY, CubeMapFace::PosY);
	texture->writeProviderPixels(negY, CubeMapFace::NegY);
	texture->writeProviderPixels(posZ, CubeMapFace::PosZ);
	texture->writeProviderPixels(negZ, CubeMapFace::NegZ);

	return texture;
}


//  ____                        _
// / ___|  __ _ _ __ ___  _ __ | | ___ _ __
// \___ \ / _` | '_ ` _ \| '_ \| |/ _ \ '__|
//  ___) | (_| | | | | | | |_) | |  __/ |
// |____/ \__,_|_| |_| |_| .__/|_|\___|_|
//                       |_|

constexpr GLint glTextureRepeatMode(TextureRepeatMode repeat) {
	switch (repeat) {
		case TextureRepeatMode::Repeat: return GL_REPEAT;
		case TextureRepeatMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
		case TextureRepeatMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case TextureRepeatMode::ClampToConstColour: return GL_CLAMP_TO_BORDER;
	}
}


static float maxAllowedAnisotropy() {
	static float maxAnisotropy = 0;
	if (maxAnisotropy == 0) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	}
	
	return maxAnisotropy;
}


Sampler::Sampler(const SamplerDescriptor& desc) {
	glGenSamplers(1, &glSampler_);
	
	// -- wrapping
	glSamplerParameteri(glSampler_, GL_TEXTURE_WRAP_S, glTextureRepeatMode(desc.repeatS));
	glSamplerParameteri(glSampler_, GL_TEXTURE_WRAP_T, glTextureRepeatMode(desc.repeatT));
	glSamplerParameteri(glSampler_, GL_TEXTURE_WRAP_R, glTextureRepeatMode(desc.repeatR));
	glSamplerParameterfv(glSampler_, GL_TEXTURE_BORDER_COLOR, desc.constColour.data);

	// -- minification
	GLint glSizingFilter;
	if (desc.mipFilter == TextureMipFilter::None) {
		if (desc.minFilter == TextureSizingFilter::Nearest)
			glSizingFilter = GL_NEAREST;
		else
			glSizingFilter = GL_LINEAR;
	}
	else if (desc.mipFilter == TextureMipFilter::Nearest) {
		if (desc.minFilter == TextureSizingFilter::Nearest)
			glSizingFilter = GL_NEAREST_MIPMAP_NEAREST;
		else
			glSizingFilter = GL_LINEAR_MIPMAP_NEAREST;
	}
	else {
		if (desc.minFilter == TextureSizingFilter::Nearest)
			glSizingFilter = GL_NEAREST_MIPMAP_LINEAR;
		else
			glSizingFilter = GL_LINEAR_MIPMAP_LINEAR;
	}
	glSamplerParameteri(glSampler_, GL_TEXTURE_MIN_FILTER, glSizingFilter);

	// -- magnification
	if (desc.magFilter == TextureSizingFilter::Nearest)
		glSizingFilter = GL_NEAREST;
	else
		glSizingFilter = GL_LINEAR;
	glSamplerParameteri(glSampler_, GL_TEXTURE_MAG_FILTER, glSizingFilter);
	
	// -- lod clamp
	glSamplerParameterf(glSampler_, GL_TEXTURE_MIN_LOD, desc.lodMinClamp);
	glSamplerParameterf(glSampler_, GL_TEXTURE_MAX_LOD, desc.lodMaxClamp);
	
	// -- anisotropy
	float anisotropy = math::clamp((float)desc.maxAnisotropy, 1.0f, maxAllowedAnisotropy());
	glSamplerParameterf(glSampler_, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}


Sampler::~Sampler() {
	if (glSampler_ > 0)
		glDeleteSamplers(1, &glSampler_);
}


} // ns render
} // ns stardazed
