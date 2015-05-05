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

#include <string>

namespace stardazed {
namespace render {


namespace detail {

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
	void load(const PixelDataProvider&);
	void setupWithDataProvider(const PixelDataProvider&);
	
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
	
	void loadFace(const PixelDataProvider&, CubeMapFace);
	void loadAllFaces(const PixelDataProvider&, const PixelDataProvider&,
					  const PixelDataProvider&, const PixelDataProvider&,
					  const PixelDataProvider&, const PixelDataProvider&);

	void setupWithDataProviders(const PixelDataProvider&, const PixelDataProvider&,
					  const PixelDataProvider&, const PixelDataProvider&,
					  const PixelDataProvider&, const PixelDataProvider&);

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
