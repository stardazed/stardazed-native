// ------------------------------------------------------------------
// render::opengl::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_TEXTURE_H
#define SD_RENDER_OPENGL_TEXTURE_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "render/common/PixelBuffer.hpp"
#include "render/common/Texture.hpp"
#include "render/opengl/PixelFormat.hpp"

namespace stardazed {
namespace render {


class Texture {
	TextureClass textureClass_;
	PixelDimensions dim_;
	size32 layers_, mipmaps_, samples_;
	PixelFormat pixelFormat_;
	GLuint glTex_ = 0;
	GLenum glTarget_ = GL_NONE;

	void write1DPixels(const PixelBuffer& pixels, uint32 offX, uint32 mipmapLevel);
	void write2DPixels(GLenum pixelTarget, const PixelBuffer& pixels, uint32 offX, uint32 offY, uint32 mipmapLevel);
	void write3DPixels(const PixelBuffer& pixels, PixelCoordinate origin, uint32 mipmapLevel);

public:
	Texture(const TextureDescriptor&);
	~Texture();
	SD_DEFAULT_MOVE_OPS(Texture)
	
	// -- pixel access
	void writePixels(const PixelBuffer&, PixelCoordinate origin, uint32 mipmapLevel, uint32 baseLayer = 0);
	void writePixels(const PixelBuffer&, PixelCoordinate origin, uint32 mipmapLevel, CubeMapFace face);

	void writeProviderPixels(const PixelDataProvider& provider, PixelCoordinate origin = {},
							 MipMapRange readRange = {}, MipMapRange writeRange = {});
	void writeProviderPixels(const PixelDataProvider& provider, CubeMapFace face, PixelCoordinate origin = {},
							 MipMapRange readRange = {}, MipMapRange writeRange = {});


//	PixelBuffer readPixels(PixelCoordinate origin, PixelDimensions size, uint32 mipmapLevel, uint32 layer = 1);

	// -- observers
	PixelDimensions dim() const { return dim_; }
	size32 width() const { return dim_.width; }
	size32 height() const { return dim_.height; }
	size32 depth() const { return dim_.depth; }

	size32 layers() const { return layers_; }
	size32 mipmaps() const { return mipmaps_; }
	size32 samples() const { return samples_; }

	bool isArray() const { return layers_ > 1; }
	bool isMultiSample() const { return samples_ > 1; }
	bool isMipMapped() const { return mipmaps_ > 1; }

	PixelFormat pixelFormat() const { return pixelFormat_; }
	
	TextureClass textureClass() const { return textureClass_; };
	bool frameBufferOnly() const;
	bool renderTargetOnly() const;
	
	// -- gl-specific observers
	GLuint name() const { return glTex_; }
	GLenum target() const { return glTarget_; }
};


// -- convenience functions to quickly deal with Textures and texture data using PixelDataProviders
Texture* textureFromProvider(const PixelDataProvider&, TextureClass, UseMipMaps);

Texture* textureCubeMapFromProviders(const PixelDataProvider& posX, const PixelDataProvider& negX,
									 const PixelDataProvider& posY, const PixelDataProvider& negY,
									 const PixelDataProvider& posZ, const PixelDataProvider& negZ,
									 UseMipMaps useMipMaps);


// ---- Texture Binding Specializations

template <>
inline void bind(const Texture& texture) {
	auto target = texture.target();
	if (target != GL_RENDERBUFFER)
		glBindTexture(target, texture.name());
	else
		glBindRenderbuffer(target, texture.name());
}


//template <>
//inline GLuint saveAndBind(const Texture& tex) {
//	GLuint currentlyBound;
//	glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&currentlyBound));
//	if (currentlyBound != tex.name())
//		bind(tex);
//	
//	return currentlyBound;
//}
//
//template <>
//inline void unbindAndRestore(const Texture& tex, GLuint savedTexName) {
//	if (savedTexName != tex.name())
//		glBindTexture(tex.target(), savedTexName);
//}


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


// ---- Sampler

class Sampler {
	GLuint glSampler_ = 0;

public:
	Sampler(const SamplerDescriptor&);
	~Sampler();
	
	void bindToTextureUnit(uint32 textureUnit) const {
		glBindSampler(textureUnit, glSampler_);
	}
	
	GLuint name() const {
		return glSampler_;
	}
};


} // ns render
} // ns stardazed

#endif
