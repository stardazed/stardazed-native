// ------------------------------------------------------------------
// render::FrameBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_FRAMEBUFFER_H
#define SD_RENDER_FRAMEBUFFER_H

#include "system/Config.hpp"
#include "render/common/PixelBuffer.hpp"
#include <array>

namespace stardazed {
namespace render {


class Texture;


struct AttachmentDescriptor {
	Texture* texture = nullptr;
	uint32 level = 0;	// mipmap
	uint32 layer = 0;	// array textures only
	uint32 depth = 0;	// 3D textures only
};


struct FrameBufferDescriptor {
	std::array<AttachmentDescriptor, 8> colourAttachments;
	AttachmentDescriptor depthAttachment;
	AttachmentDescriptor stencilAttachment;
};


} // ns render
} // ns stardazed

#endif
