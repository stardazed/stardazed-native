// ------------------------------------------------------------------
// render::opengl::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLTEXTURE_H
#define SD_RENDER_OPENGLTEXTURE_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "render/common/Texture.hpp"
#include "render/opengl/OpenGL.hpp"

#include <string>

namespace stardazed {
namespace render {


namespace detail {

	constexpr GLint glColourMaskForColourComponent(ColourComponent c) {
		switch (c) {
			case ColourComponent::Red:   return GL_RED;
			case ColourComponent::Green: return GL_GREEN;
			case ColourComponent::Blue:  return GL_BLUE;
			case ColourComponent::Alpha: return GL_ALPHA;
			case ColourComponent::Zero:  return GL_ZERO;
			case ColourComponent::One:   return GL_ONE;
		}
	}

	template <GLenum Target>
	class GLTexture {
		int32 textureUnit_ = 0;
		GLuint glTex_;
	public:
		GLTexture() { glGenTextures(1, &glTex_); }
		~GLTexture() {
			glDeleteTextures(1, &glTex_);
		}
		SD_DEFAULT_MOVE_OPS(GLTexture)

		GLuint name() const { return glTex_; }
		GLenum target() const { return Target; }

		// -- attributes
		void setSwizzleMask(ColourComponent r, ColourComponent g, ColourComponent b, ColourComponent a) const {
			GLint mask[] = {
				glColourMaskForColourComponent(r),
				glColourMaskForColourComponent(g),
				glColourMaskForColourComponent(b),
				glColourMaskForColourComponent(a)
			};
			glTexParameteriv(Target, GL_TEXTURE_SWIZZLE_RGBA, mask);
		}

		// -- binding
		void bind() const {
			glActiveTexture(GL_TEXTURE0 + textureUnit_);
			glBindTexture(Target, glTex_);
		}

		int32 textureUnit() const { return textureUnit_; }
		void setTextureUnit(int32 unit) { textureUnit_ = unit; }
	};
	
} // ns detail


class Texture2D : public detail::GLTexture<GL_TEXTURE_2D> {
	size32 width_ = 0, height_ = 0;
	PixelFormat format_ = PixelFormat::None;

public:
	void allocate(size32 width, size32 height, uint8 levels, PixelFormat format);
	
	void uploadPixelBuffer(const PixelBuffer& image, uint8 level);
	void load(const TextureDataProvider&);
	void setupWithDataProvider(const TextureDataProvider&);
	
	size32 width() const { return width_; }
	size32 height() const { return height_; }
	PixelFormat pixelFormat() const { return format_; }
};


class Texture2DMultisample : public detail::GLTexture<GL_TEXTURE_2D_MULTISAMPLE> {
	size32 width_ = 0, height_ = 0;
	uint8 samples_ = 0;
	PixelFormat format_ = PixelFormat::None;
	
public:
	void allocate(size32 width, size32 height, uint8 numSamples, PixelFormat format);

	size32 width() const { return width_; }
	size32 height() const { return height_; }
	uint8 samples() const { return samples_; }
	PixelFormat pixelFormat() const { return format_; }
};


class TextureCubeMap : public detail::GLTexture<GL_TEXTURE_CUBE_MAP> {
	size32 side_ = 0;
	PixelFormat format_ = PixelFormat::None;

public:
	void allocate(size32 side, uint8 levels, PixelFormat);
	
	void uploadFacePixelBuffer(const PixelBuffer&, uint8 level, CubeMapFace);
	
	void loadFace(const TextureDataProvider&, CubeMapFace);
	void loadAllFaces(const TextureDataProvider&, const TextureDataProvider&,
					  const TextureDataProvider&, const TextureDataProvider&,
					  const TextureDataProvider&, const TextureDataProvider&);

	void setupWithDataProviders(const TextureDataProvider&, const TextureDataProvider&,
					  const TextureDataProvider&, const TextureDataProvider&,
					  const TextureDataProvider&, const TextureDataProvider&);

	size32 side() const { return side_; }
	PixelFormat pixelFormat() const { return format_; }
};


// ---- Texture Binding Specializations

template <>
inline GLuint saveAndBind(const Texture2D& t2) {
	GLuint currentlyBound;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&currentlyBound));
	if (currentlyBound != t2.name())
		t2.bind();
	
	return currentlyBound;
}

template <>
inline void unbindAndRestore(const Texture2D& t2, GLuint savedT2Name) {
	if (savedT2Name != t2.name())
		glBindTexture(GL_TEXTURE_2D, savedT2Name);
}


template <>
inline GLuint saveAndBind(const Texture2DMultisample& t2ms) {
	GLuint currentlyBound;
	glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, reinterpret_cast<GLint*>(&currentlyBound));
	if (currentlyBound != t2ms.name())
		t2ms.bind();
	
	return currentlyBound;
}

template <>
inline void unbindAndRestore(const Texture2DMultisample& t2ms, GLuint savedT2MSName) {
	if (savedT2MSName != t2ms.name())
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, savedT2MSName);
}


template <>
inline GLuint saveAndBind(const TextureCubeMap& tcm) {
	GLuint currentlyBound;
	glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, reinterpret_cast<GLint*>(&currentlyBound));
	if (currentlyBound != tcm.name())
		tcm.bind();
	
	return currentlyBound;
}

template <>
inline void unbindAndRestore(const TextureCubeMap& tcm, GLuint savedTCMName) {
	if (savedTCMName != tcm.name())
		glBindTexture(GL_TEXTURE_CUBE_MAP, savedTCMName);
}


// ---------


constexpr GLint glImageFormatForPixelFormat(PixelFormat format) {
	switch (format) {
		case PixelFormat::R8: return GL_RED;
		case PixelFormat::RG8: return GL_RG;
			
		case PixelFormat::RGB8: return GL_RGB;
		case PixelFormat::BGR8: return GL_BGR;
		case PixelFormat::RGBA8: return GL_RGBA;
		case PixelFormat::BGRA8: return GL_BGRA;
			
		case PixelFormat::RGB32F: return GL_RGB;
		case PixelFormat::RGBA32F: return GL_RGBA;
			
		case PixelFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case PixelFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case PixelFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			
		case PixelFormat::Depth16I:
		case PixelFormat::Depth24I:
		case PixelFormat::Depth32I:
		case PixelFormat::Depth32F:
			return GL_DEPTH_COMPONENT;
			
		case PixelFormat::Stencil8: return GL_STENCIL_INDEX;
			
		case PixelFormat::Depth24_Stencil8:
		case PixelFormat::Depth32F_Stencil8:
			return GL_DEPTH_STENCIL;
			
		default:
			assert(!"unhandled texture pixel format");
			return GL_NONE;
	}
}


constexpr GLint glInternalFormatForPixelFormat(PixelFormat format) {
	switch (format) {
		case PixelFormat::R8: return GL_R8;
		case PixelFormat::RG8: return GL_RG8;
			
		case PixelFormat::RGB8: return GL_RGB8;
		case PixelFormat::BGR8: return GL_RGB8;   // swizzled
		case PixelFormat::RGBA8: return GL_RGBA8;
		case PixelFormat::BGRA8: return GL_RGBA8; // swizzled
			
		case PixelFormat::RGB32F: return GL_RGB32F;
		case PixelFormat::RGBA32F: return GL_RGBA32F;
			
		case PixelFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case PixelFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case PixelFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			
		case PixelFormat::Depth16I: return GL_DEPTH_COMPONENT16;
		case PixelFormat::Depth24I: return GL_DEPTH_COMPONENT24;
		case PixelFormat::Depth32I: return GL_DEPTH_COMPONENT32;
		case PixelFormat::Depth32F: return GL_DEPTH_COMPONENT32F;
			
		case PixelFormat::Stencil8: return GL_STENCIL_INDEX8;
			
		case PixelFormat::Depth24_Stencil8: return GL_DEPTH24_STENCIL8;
		case PixelFormat::Depth32F_Stencil8: return GL_DEPTH32F_STENCIL8;
			
		default:
			assert(!"unhandled texture pixel format");
			return GL_NONE;
	}
}


constexpr GLenum glPixelDataTypeForPixelFormat(PixelFormat format) {
	assert(! pixelFormatIsCompressed(format));
	
	switch (format) {
		case PixelFormat::R8:
		case PixelFormat::RG8:
		case PixelFormat::RGB8:
		case PixelFormat::BGR8:
		case PixelFormat::RGBA8:
		case PixelFormat::BGRA8:
		case PixelFormat::Stencil8:
			return GL_UNSIGNED_BYTE;
			
		case PixelFormat::RGB32F:
		case PixelFormat::RGBA32F:
		case PixelFormat::Depth32F:
			return GL_FLOAT;
			
		case PixelFormat::Depth16I:
			return GL_UNSIGNED_SHORT;
		case PixelFormat::Depth24I:
		case PixelFormat::Depth32I:
			return GL_UNSIGNED_INT;
			
		case PixelFormat::Depth24_Stencil8:
			return GL_UNSIGNED_INT_24_8;
		case PixelFormat::Depth32F_Stencil8:
			return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
			
		default:
			assert(!"unhandled pixel buffer format");
			return GL_NONE;
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


} // ns render
} // ns stardazed

#endif
