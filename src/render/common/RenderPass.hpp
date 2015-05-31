// ------------------------------------------------------------------
// render::RenderPass - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_RENDERPASS_H
#define SD_RENDER_RENDERPASS_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "render/common/FrameBuffer.hpp"
#include <array>

namespace stardazed {
namespace render {


enum class FaceCulling {
	Disabled,

	Front,
	Back,
	FrontAndBack
};


enum class DepthTestPredicate {
	Disabled,

	AllowAll,
	DenyAll,
	
	Less,
	LessOrEqual,
	Equal,
	NotEqual,
	GreaterOrEqual,
	Greater
};


enum class AttachmentLoadAction {
	None,
	Load,
	Clear
};


enum class AttachmentStoreAction {
	None,
	Store
};


namespace detail {
	struct AttachmentActions {
		AttachmentLoadAction loadAction = AttachmentLoadAction::None;
		AttachmentStoreAction storeAction = AttachmentStoreAction::None;
	};
}


struct ColourAttachmentActions : detail::AttachmentActions {
	math::Vec4 clearColour { 0, 0, 0, 1 };
};


struct DepthAttachmentActions : detail::AttachmentActions {
	float clearDepth = 1.0f;
};


struct StencilAttachmentActions : detail::AttachmentActions {
	uint32 clearStencil = 0;
};


struct RenderPassDescriptor {
	std::array<ColourAttachmentActions, maxColourAttachments()> colourActions;
	DepthAttachmentActions depthActions;
	StencilAttachmentActions stencilActions;
};


} // ns render
} // ns stardazed

#endif
