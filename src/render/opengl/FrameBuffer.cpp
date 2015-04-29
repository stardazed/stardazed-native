// ------------------------------------------------------------------
// render::opengl::FrameBuffer.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/FrameBuffer.hpp"
#include "system/Logging.hpp"

namespace stardazed {
namespace render {


RenderBuffer::RenderBuffer(size32 width, size32 height, PixelFormat format, uint8 samples)
: width_(width)
, height_(height)
, format_(format)
, samples_(samples)
{
	glGenRenderbuffers(1, &glRBO_);
	bind();

	// samples defaults to 0, which is effectively a non-ms buffer
	auto glFormat = glInternalFormatForPixelFormat(format);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples_, glFormat, width_, height_);
}


void FrameBuffer::addShadowDepthMap(uint32 width, uint32 height) {
	depthAttachment.allocate(width, height, 1, PixelFormat::DepthShadow);
	
	// FIXME: move this to a sampler
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, glFBO_);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment.name(), 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		sd::log("fbo not complete: ", status);
	}
}

	
} // ns render
} // ns stardazed
