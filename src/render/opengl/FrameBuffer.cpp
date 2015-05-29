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


static void attachColourTexture(GLenum glAttachment, const AttachmentDescriptor& attachment) {
	assert(attachment.level < attachment.texture->mipmaps());
	assert(attachment.depth < attachment.texture->depth());
	assert(attachment.layer < attachment.texture->layers());

	if (attachment.texture->target() == GL_RENDERBUFFER) {
		// -- render buffer (GL optimalization)
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, glAttachment, GL_RENDERBUFFER, attachment.texture->name());
	}
	else {
		// -- normal texture (3D has depth, others can have layers)
		GLint layer = layerForTextureType(attachment);
		
		// -- bind texture layer
		glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, glAttachment,
								  attachment.texture->name(), attachment.level, layer);
	}
}


FrameBuffer::FrameBuffer(const FrameBufferDescriptor& desc) {
	glGenFramebuffers(1, &glFBO_);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glFBO_);
	
	// -- colour
	uint32 colourAttachmentIndex = 0;
	std::array<GLenum, sizeof(desc.colourAttachments)> drawBuffers;

	for (const auto& attachment : desc.colourAttachments) {
		if (attachment.texture) {
			GLenum glAttachment = GL_COLOR_ATTACHMENT0 + colourAttachmentIndex;
			attachColourTexture(colourAttachmentIndex, attachment);
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

		glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
								  depthTex->name(), 0, desc.depthAttachment.layer);
	}
	else {
		if (depthTex) {
			assert(pixelFormatIsDepthFormat(depthTex->pixelFormat()));
			glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
									  depthTex->name(), 0, desc.depthAttachment.layer);
		}

		if (stencilTex) {
			assert(pixelFormatIsStencilFormat(stencilTex->pixelFormat()));
			glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
									  stencilTex->name(), 0, desc.stencilAttachment.layer);
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


FrameBufferDescriptor allocateTexturesForFrameBuffer(const FrameBufferAllocationDescriptor& desc) {
	FrameBufferDescriptor fbDesc {};
	
	// -- colour
	for (auto colourAttIndex = 0u; colourAttIndex < desc.colourFormats.size(); ++colourAttIndex) {
		TextureDescriptor texDesc {};
		texDesc.textureClass = TextureClass::Tex2D;
		texDesc.dim.width = desc.width;
		texDesc.dim.height = desc.height;
		texDesc.samples = desc.samples;
		texDesc.pixelFormat = desc.colourFormats[colourAttIndex];
		texDesc.usageHint = desc.colourUsageHints[colourAttIndex];

		auto& attachment = fbDesc.colourAttachments[colourAttIndex];
		attachment.texture = new Texture(texDesc);
	}

	// -- depth & stencil
	PixelFormat combinedFormat = PixelFormat::None;
	
	assert(desc.depthFormat == PixelFormat::None ||
		   pixelFormatIsDepthFormat(desc.depthFormat) ||
		   pixelFormatIsDepthStencilFormat(desc.depthFormat));
	assert(desc.stencilFormat == PixelFormat::None ||
		   pixelFormatIsStencilFormat(desc.stencilFormat) ||
		   pixelFormatIsDepthStencilFormat(desc.stencilFormat));

	// -- check if we can use a combined depth/stencil format
	if (pixelFormatIsDepthStencilFormat(desc.depthFormat)) {
		// explicit combined format
		assert(desc.depthFormat == desc.stencilFormat);
		combinedFormat = desc.depthFormat;
	}
	else {
		// if depth is not a DS format, then stencil cannot be a DS format either
		assert(! pixelFormatIsDepthStencilFormat(desc.stencilFormat));

		if (desc.stencilFormat == PixelFormat::Stencil8) {
			if (desc.depthFormat == PixelFormat::Depth24I)
				combinedFormat = PixelFormat::Depth24_Stencil8;
			else if (desc.depthFormat == PixelFormat::Depth32F)
				combinedFormat = PixelFormat::Depth32F_Stencil8;
		}
	}

	// -- create the texture(s)
	TextureDescriptor dsTex {};
	dsTex.textureClass = TextureClass::Tex2D;
	dsTex.dim.width = desc.width;
	dsTex.dim.height = desc.height;

	if (combinedFormat != PixelFormat::None) {
		dsTex.pixelFormat = combinedFormat;
		auto depthStencil = new Texture(dsTex);
		fbDesc.depthAttachment.texture = depthStencil;
		fbDesc.stencilAttachment.texture = depthStencil;
	}
	else {
		if (desc.depthFormat != PixelFormat::None) {
			dsTex.pixelFormat = desc.depthFormat;
			fbDesc.depthAttachment.texture = new Texture(dsTex);
		}
		if (desc.stencilFormat != PixelFormat::None) {
			dsTex.pixelFormat = desc.stencilFormat;
			fbDesc.stencilAttachment.texture = new Texture(dsTex);
		}
	}
	
	return fbDesc;
}

	
} // ns render
} // ns stardazed
