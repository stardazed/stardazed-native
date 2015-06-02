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


enum class FrontFaceWinding {
	Clockwise,
	CounterClockwise
};


enum class TriangleFillMode {
	Fill,
	Line
};


enum class FaceCulling {
	Disabled,

	Front,
	Back
};


enum class DepthTest {
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


struct ScissorRect {
	uint32 originX = 0, originY = 0;
	uint32 width = 32768, height = 32768;
};


struct Viewport {
	uint32 originX = 0, originY = 0;
	uint32 width = 0, height = 0;
	float nearZ = 0, farZ = 1;
};


struct DepthStencilDescriptor {
	DepthTest depthTest = DepthTest::AllowAll;
	
	// FIXME: add Stencil setup
};


enum class AttachmentLoadAction {
	DontCare,
	Load,
	Clear
};


enum class AttachmentStoreAction {
	DontCare,
	Store
};


struct ColourAttachmentActions {
	AttachmentLoadAction loadAction = AttachmentLoadAction::DontCare;
	AttachmentStoreAction storeAction = AttachmentStoreAction::Store;
	math::Vec4 clearColour { 0, 0, 0, 1 };
};


struct DepthAttachmentActions {
	AttachmentLoadAction loadAction = AttachmentLoadAction::Clear;
	AttachmentStoreAction storeAction = AttachmentStoreAction::DontCare;
	float clearDepth = 1.0f;
};


struct StencilAttachmentActions {
	AttachmentLoadAction loadAction = AttachmentLoadAction::Clear;
	AttachmentStoreAction storeAction = AttachmentStoreAction::DontCare;
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
