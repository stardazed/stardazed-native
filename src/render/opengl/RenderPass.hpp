// ------------------------------------------------------------------
// render::opengl::RenderPass - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_RENDERPASS_H
#define SD_RENDER_OPENGL_RENDERPASS_H

#include "util/ConceptTraits.hpp"
#include "render/common/RenderPass.hpp"
#include "render/opengl/FrameBuffer.hpp"
#include "render/opengl/Pipeline.hpp"

namespace stardazed {
namespace render {


class DepthStencilTest {
	bool32 depthTestEnabled_;
	GLenum depthFunc_;

public:
	explicit DepthStencilTest(const DepthStencilDescriptor&);
	void apply() const;
};


class RenderPass {
	const FrameBuffer& fbo_;
	const RenderPassDescriptor& descriptor_;
	const Pipeline* pipeline_ = nullptr;
	
public:
	RenderPass(const RenderPassDescriptor&, const FrameBuffer&);
	~RenderPass();
	SD_DEFAULT_MOVE_OPS(RenderPass)
	
	// -- observers
	const FrameBuffer& frameBuffer() const { return fbo_; }

	// -- render state
	void setPipeline(const Pipeline&);
	void setDepthStencilTest(const DepthStencilTest&);

	void setFaceCulling(FaceCulling);
	void setFrontFaceWinding(FrontFaceWinding);
	void setTriangleFillMode(TriangleFillMode);

	void setViewPort(const Viewport&);
	void setScissorRect(const ScissorRect&);
	
	void setConstantBlendColour(const math::Vec4&);
};


} // ns render
} // ns stardazed

#endif
