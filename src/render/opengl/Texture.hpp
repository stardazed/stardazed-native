// ------------------------------------------------------------------
// render::opengl::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLTEXTURE_H
#define SD_RENDER_OPENGLTEXTURE_H

#include "system/Config.hpp"
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
		GLuint glTex_;
	public:
		GLTexture() { glGenTextures(1, &glTex_); }
		~GLTexture() { glDeleteTextures(1, &glTex_); }

		GLuint name() const { return glTex_; }

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
			glBindTexture(GL_TEXTURE_2D, glTex_);
		}

		void useTextureUnit(uint8 unit) {
			glActiveTexture(GL_TEXTURE0 + unit);
		}
	};
	
} // ns detail


class Texture2D : public detail::GLTexture<GL_TEXTURE_2D> {
	size32 width_ = 0, height_ = 0;
	ImageDataFormat format_ = ImageDataFormat::None;

public:
	void allocate(size32 width, size32 height, uint8 levels, ImageDataFormat format);
	
	void uploadImageData(const ImageData& image, uint8 level);

	void load(const TextureDataProvider&);
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


} // ns render
} // ns stardazed

#endif
