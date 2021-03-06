// ------------------------------------------------------------------
// render::FrameBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_FRAMEBUFFER_H
#define SD_RENDER_FRAMEBUFFER_H

#include "system/Config.hpp"
#include "render/common/PixelBuffer.hpp"
#include "render/common/Texture.hpp"
#include <array>

namespace stardazed {
namespace render {


constexpr uint32 maxColourAttachments() {
	// FIXME: while this can be queried at runtime, we want to use
	// a dependable absolute maximum per platform at compile time
	// e.g. iOS -> 4, Mac OS X -> 8
	return 8;
}


struct AttachmentDescriptor {
	Texture* texture = nullptr;
	uint32 level = 0;	// mipmap
	uint32 layer = 0;	// array textures only
	uint32 depth = 0;	// 3D textures only
};


struct FrameBufferDescriptor {
	std::array<AttachmentDescriptor, maxColourAttachments()> colourAttachments;
	AttachmentDescriptor depthAttachment;
	AttachmentDescriptor stencilAttachment;
};


// This structure facilitates easy creation of all required
// textures for a FrameBuffer in case they need to be allocated anyway
// The implementation is free to allocate the textures as fit for the
// platform (2D array, multiple 2D textures, etc.) so no assumptions should
// be made about the type or organization of the textures.

struct FrameBufferAllocationDescriptor {
	// properties shared by all textures for the FrameBuffer
	uint32 width = 0, height = 0;
	uint32 samples = 0;
	
	std::array<PixelFormat, maxColourAttachments()> colourFormats {};
	std::array<TextureUsageHint, maxColourAttachments()> colourUsageHints {};

	// The implementation may create a combined depth/stencil texture if it
	// fits the profile of the provided texture formats, or you can make it
	// explicit by setting both to the same DepthStencil PixelFormat.
	PixelFormat depthFormat = PixelFormat::None;
	PixelFormat stencilFormat = PixelFormat::None;
	
	// As depth and stencil buffers are almost exclusively used in
	// FBO environments, their default hint is set accordingly.
	TextureUsageHint depthUsageHint = TextureUsageHint::RenderTargetOnly;
	TextureUsageHint stencilUsageHint = TextureUsageHint::RenderTargetOnly;
};


class FrameBuffer;


} // ns render
} // ns stardazed

#endif
