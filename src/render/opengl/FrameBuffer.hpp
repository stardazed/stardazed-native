// ------------------------------------------------------------------
// render::opengl::FrameBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLFRAMEBUFFER_H
#define SD_RENDER_OPENGLFRAMEBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

#include "render/opengl/OpenGL.hpp"
#include "render/opengl/Texture.hpp"

namespace stardazed {
namespace render {


class RenderBuffer {
	size32 width_, height_;
	PixelFormat format_;
	uint8 samples_;
	GLuint glRBO_ = 0;
	
public:
	RenderBuffer(size32 width, size32 height, PixelFormat format, uint8 samples = 0);
	~RenderBuffer() {
		if (glRBO_)
			glDeleteRenderbuffers(1, &glRBO_);
	}
	SD_DEFAULT_MOVE_OPS(RenderBuffer)
	
	size32 width() const { return width_; }
	size32 height() const { return height_; }
	PixelFormat pixelFormat() const { return format_; }
	uint8 samples() const { return samples_; }
	
	GLuint name() const { return glRBO_; }
	
	void bind() const {
		glBindRenderbuffer(GL_RENDERBUFFER, glRBO_);
	}
};


class FrameBuffer {
	size32 width_, height_;
	uint8 samples_;
	GLuint glFBO_ = 0;

public:
	FrameBuffer(size32 width, size32 height, uint8 samples = 0)
	: width_(width)
	, height_(height)
	, samples_(samples)
	{
		glGenFramebuffers(1, &glFBO_);
	}
	~FrameBuffer() {
		if (glFBO_)
			glDeleteFramebuffers(1, &glFBO_);
	}
	SD_DEFAULT_MOVE_OPS(FrameBuffer)
	
	GLuint name() const { return glFBO_; }
	
	
};
	
		
} // ns render
} // ns stardazed

#endif
