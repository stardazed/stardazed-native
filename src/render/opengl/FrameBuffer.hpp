// ------------------------------------------------------------------
// render::opengl::FrameBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_FRAMEBUFFER_H
#define SD_RENDER_OPENGL_FRAMEBUFFER_H

#include "render/common/FrameBuffer.hpp"
#include "render/opengl/Texture.hpp"

namespace stardazed {
namespace render {


class FrameBuffer {
	GLuint glFBO_ = 0;

public:
	explicit FrameBuffer(const FrameBufferDescriptor&);
	~FrameBuffer();

	SD_DEFAULT_MOVE_OPS(FrameBuffer)
	
	void bindForDrawing() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glFBO_);
	}

	void bindForReading(uint32 colourAttachmentIndex) const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, glFBO_);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + colourAttachmentIndex);
	}
	
	GLuint name() const { return glFBO_; }
};
	
		
} // ns render
} // ns stardazed

#endif
