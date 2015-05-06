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
	size32 width_, height_, depth_;
	size32 layers_, mipmaps_, samples_;
	PixelFormat pixelFormat_;
	GLuint glTex_ = 0;
	GLenum glTarget_ = GL_NONE;

public:
	Texture(const TextureDescriptor&);
	~Texture();
	
	// -- pixel access
	void writePixels(const PixelBuffer&, uint32 x, uint32 y, uint32 mipmapLevel, uint32 layer = 1);
	PixelBuffer readPixels(uint32 x, uint32 y, uint32 width, uint32 height, uint32 mipmapLevel, uint32 layer = 1);

	// -- observers
	size32 width() const { return width_; }
	size32 height() const { return height_; }
	size32 depth() const { return depth_; }

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
};


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
	void allocate(size32 width, size32 height, uint32 levels, PixelFormat format);
	
	void uploadPixelBuffer(const PixelBuffer& image, uint32 level);
	void load(const PixelDataProvider&);
	void setupWithDataProvider(const PixelDataProvider&);
	
	size32 width() const { return width_; }
	size32 height() const { return height_; }
	PixelFormat pixelFormat() const { return format_; }
};


class Texture2DMultisample : public detail::GLTexture<GL_TEXTURE_2D_MULTISAMPLE> {
	size32 width_ = 0, height_ = 0;
	uint32 samples_ = 0;
	PixelFormat format_ = PixelFormat::None;
	
public:
	void allocate(size32 width, size32 height, uint32 numSamples, PixelFormat format);

	size32 width() const { return width_; }
	size32 height() const { return height_; }
	uint32 samples() const { return samples_; }
	PixelFormat pixelFormat() const { return format_; }
};


class TextureCubeMap : public detail::GLTexture<GL_TEXTURE_CUBE_MAP> {
	size32 side_ = 0;
	PixelFormat format_ = PixelFormat::None;

public:
	void allocate(size32 side, uint32 levels, PixelFormat);
	
	void uploadFacePixelBuffer(const PixelBuffer&, uint32 level, CubeMapFace);
	
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
