// ------------------------------------------------------------------
// render::RenderPass - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_RENDERPASS_H
#define SD_RENDER_RENDERPASS_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include <array>

namespace stardazed {
namespace render {


class Texture;


enum class AttachmentLoadAction {
	None,
	Load,
	Clear
};


enum class AttachmentStoreAction {
	None,
	Store
};


struct AttachmentDescriptor {
	Texture* texture = nullptr;
	uint32 level = 0;	// mipmap
	uint32 layer = 0;	// array textures only
	uint32 depth = 0;	// 3D textures only
	
	AttachmentLoadAction loadAction = AttachmentLoadAction::None;
	AttachmentStoreAction storeAction = AttachmentStoreAction::None;
};


struct RenderPassDescriptor {
	std::array<AttachmentDescriptor, 8> colourAttachments;
	std::array<math::Vec4, 8> clearColours;

	AttachmentDescriptor depthAttachment;
	float clearDepth = 1.0f;

	AttachmentDescriptor stencilAttachment;
	uint32 clearStencil = 0;

	RenderPassDescriptor() {
		for (auto& colour : clearColours)
			colour = { 0, 0, 0, 1 };
	}
};


} // ns render
} // ns stardazed

#endif
