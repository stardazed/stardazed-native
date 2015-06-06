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
	FrameBufferDescriptor attachmentDesc_;
	GLResource resource_ {};
	uint32 width_ = 0, height_ = 0;

public:
	explicit FrameBuffer(const FrameBufferDescriptor&);
	~FrameBuffer();

	SD_DEFAULT_MOVE_OPS(FrameBuffer)
	
	void bindForDrawing() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, name());
	}

	void bindForReading(uint32 colourAttachmentIndex) const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, name());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + colourAttachmentIndex);
	}
	
	// -- observers
	uint32 width() const { return width_; }
	uint32 height() const { return height_; }
	GLuint name() const { return resource_.name(); }
	
	bool hasColourAttachment(uint32 atIndex) const;
	bool hasDepthAttachment() const;
	bool hasStencilAttachment() const;
};


} // ns render
} // ns stardazed

#endif
