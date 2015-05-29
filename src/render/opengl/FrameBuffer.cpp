// ------------------------------------------------------------------
// render::opengl::FrameBuffer.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/FrameBuffer.hpp"
#include "system/Logging.hpp"

namespace stardazed {
namespace render {


static GLint layerForTextureType(const AttachmentDescriptor& attachment) {
	if (attachment.texture->textureClass() == TextureClass::Tex3D) {
		return attachment.depth;
	}
	else {
		return attachment.layer;
	}
}


static void attachTexture(GLenum glAttachment, const AttachmentDescriptor& attachment) {
	// -- normal texture (3D has depth, others can have layers)
	GLint layer = layerForTextureType(attachment);

	if (attachment.texture->layers() > 1 || attachment.texture->textureClass() == TextureClass::TexCube) {
		glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, glAttachment,
								  attachment.texture->name(), attachment.level, layer);
	}
	else {
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, glAttachment, attachment.texture->name(), attachment.level);
	}

	auto status = glGetError();
	assert(status == GL_NO_ERROR);
}


static void attachColourTexture(GLenum glAttachment, const AttachmentDescriptor& attachment) {
	assert(attachment.level < attachment.texture->mipmaps());
	assert(attachment.depth < attachment.texture->depth());
	assert(attachment.layer < attachment.texture->layers());

	if (attachment.texture->target() == GL_RENDERBUFFER) {
		// -- render buffer (GL optimalization)
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, glAttachment, GL_RENDERBUFFER, attachment.texture->name());
	}
	else {
		attachTexture(glAttachment, attachment);
	}
}


FrameBuffer::FrameBuffer(const FrameBufferDescriptor& desc) {
	glGenFramebuffers(1, &glFBO_);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glFBO_);
	
	// -- colour
	uint32 colourAttachmentIndex = 0;
	std::array<GLenum, maxColourAttachments()> drawBuffers;

	for (const auto& attachment : desc.colourAttachments) {
		if (attachment.texture) {
			GLenum glAttachment = GL_COLOR_ATTACHMENT0 + colourAttachmentIndex;
			attachColourTexture(glAttachment, attachment);
			drawBuffers[colourAttachmentIndex] = glAttachment;
		}
		else {
			drawBuffers[colourAttachmentIndex] = GL_NONE;
		}

		++colourAttachmentIndex;
	}

	// -- setup the draw buffers to mimic the colour attachments
	// -- as this is the wished behaviour in almost all cases
	glDrawBuffers(drawBuffers.size(), drawBuffers.data());

	// -- depth and/or stencil
	auto depthTex = desc.depthAttachment.texture;
	auto stencilTex = desc.stencilAttachment.texture;

	if (depthTex) {
		assert(desc.depthAttachment.level == 0);
		assert(desc.depthAttachment.depth == 0);
		assert(desc.depthAttachment.layer < depthTex->layers());
	}

	if (stencilTex) {
		assert(desc.stencilAttachment.level == 0);
		assert(desc.stencilAttachment.depth == 0);
		assert(desc.stencilAttachment.layer < stencilTex->layers());
	}

	if ((depthTex && stencilTex) && (depthTex == stencilTex)) {
		// -- combined depth/stencil texture
		assert(desc.depthAttachment.layer == desc.stencilAttachment.layer);
		assert(pixelFormatIsDepthStencilFormat(depthTex->pixelFormat()));

		attachTexture(GL_DEPTH_STENCIL_ATTACHMENT, desc.depthAttachment);
	}
	else {
		if (depthTex) {
			assert(pixelFormatIsDepthFormat(depthTex->pixelFormat()));
			attachTexture(GL_DEPTH_ATTACHMENT, desc.depthAttachment);
		}

		if (stencilTex) {
			assert(pixelFormatIsStencilFormat(stencilTex->pixelFormat()));
			attachTexture(GL_STENCIL_ATTACHMENT, desc.depthAttachment);
		}
	}

	// -- beg for approval to the GL gods
	auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		assert(!"FrameBuffer not complete");
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


FrameBuffer::~FrameBuffer() {
	if (glFBO_)
		glDeleteFramebuffers(1, &glFBO_);
}

	
} // ns render
} // ns stardazed
