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

	
} // ns render
} // ns stardazed
