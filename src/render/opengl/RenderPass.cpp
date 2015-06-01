// ------------------------------------------------------------------
// render::opengl::RenderPass.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/RenderPass.hpp"

namespace stardazed {
namespace render {


DepthStencilTest::DepthStencilTest(const DepthStencilDescriptor& dsd) {
	depthTestEnabled_ = dsd.depthTest != DepthTest::Disabled;
	
	switch (dsd.depthTest) {
		case DepthTest::AllowAll:
			depthFunc_ = GL_ALWAYS; break;
		case DepthTest::DenyAll:
			depthFunc_ = GL_NEVER; break;
		case DepthTest::Less:
			depthFunc_ = GL_LESS; break;
		case DepthTest::LessOrEqual:
			depthFunc_ = GL_LEQUAL; break;
		case DepthTest::Equal:
			depthFunc_ = GL_EQUAL; break;
		case DepthTest::NotEqual:
			depthFunc_ = GL_NOTEQUAL; break;
		case DepthTest::GreaterOrEqual:
			depthFunc_ = GL_GEQUAL; break;
		case DepthTest::Greater:
			depthFunc_ = GL_GREATER; break;

		default:
			depthFunc_ = GL_NONE; break;
	}
}


void DepthStencilTest::apply() const {
	if (depthTestEnabled_) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(depthFunc_);
	}
	else
		glDisable(GL_DEPTH_TEST);
}


RenderPass::RenderPass(const RenderPassDescriptor& descriptor, const FrameBuffer& fb)
: fbo_(fb)
, descriptor_(descriptor)
{
	// FIXME: run load actions on all attachments
}


RenderPass::~RenderPass() {
	// FIXME: run store actions on all attachments
	// FIXME: restore all render state to defaults?
}


void RenderPass::setPipeline(const Pipeline& pipeline) {
	// FIXME: validate Pipeline against FrameBuffer
	pipeline_ = &pipeline;
}


void RenderPass::setDepthStencilTest(const DepthStencilTest& dst) {
	dst.apply();
}


void RenderPass::setFaceCulling(FaceCulling faceCulling) {
	if (faceCulling == FaceCulling::Disabled) {
		glDisable(GL_CULL_FACE);
	}
	else {
		glEnable(GL_CULL_FACE);
		auto mode = (faceCulling == FaceCulling::Back) ? GL_BACK : GL_FRONT;
		glCullFace(mode);
	}
}


void RenderPass::setFrontFaceWinding(FrontFaceWinding winding) {
	auto mode = (winding == FrontFaceWinding::Clockwise) ? GL_CW : GL_CCW;
	glFrontFace(mode);
}


void RenderPass::setTriangleFillMode(TriangleFillMode fillMode) {
	auto mode = (fillMode == TriangleFillMode::Fill) ? GL_FILL : GL_LINE;
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}


void RenderPass::setViewPort(const Viewport& viewport) {
	// FIXME: treat width, height == 0 as alias for full viewport
	glViewport(viewport.originX, viewport.originY, viewport.width, viewport.height);
	glDepthRangef(viewport.nearZ, viewport.farZ);
}


void RenderPass::setScissorRect(const ScissorRect& rect) {
	glScissor(rect.originX, rect.originY, rect.width, rect.height);

	if (rect.originX > 0 || rect.originY > 0 || rect.width < fbo_.width() || rect.height < fbo_.height())
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}


void RenderPass::setConstantBlendColour(const math::Vec4& colour) {
	glBlendColor(colour.r, colour.g, colour.b, colour.a);
}


} // ns render
} // ns stardazed
