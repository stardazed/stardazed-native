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


class FrameBuffer {
	GLuint glFBO_ = 0;
	Texture2D depthAttachment;

public:
	FrameBuffer() {
		glGenFramebuffers(1, &glFBO_);
	}
	~FrameBuffer() {
		glDeleteFramebuffers(1, &glFBO_);
	}
	SD_DEFAULT_MOVE_OPS(FrameBuffer)
	
	
	void addShadowDepthMap(uint32 width, uint32 height);
	
	GLuint name() const { return glFBO_; }
};
	
		
} // ns render
} // ns stardazed

#endif
