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


enum class ColourAttachmentBinding {
	Texture2D,
	TextureCube
};


struct FrameBufferColourAttachmentDescriptor {
	PixelFormat pixelFormat;
	
};


struct FrameBufferDescriptor {
	int width, height;
	
	std::array<PixelFormat, 8> colourAttachments;
	PixelFormat depthStencilAttachment;
};


} // ns render
} // ns stardazed

#endif
