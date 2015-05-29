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
	FrameBuffer(const FrameBufferDescriptor&);
	~FrameBuffer();

	SD_DEFAULT_MOVE_OPS(FrameBuffer)
	
	GLuint name() const { return glFBO_; }
};
	
		
} // ns render
} // ns stardazed

#endif
