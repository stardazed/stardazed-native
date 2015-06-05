// ------------------------------------------------------------------
// render::opengl::RenderPass.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/RenderPass.hpp"

namespace stardazed {
namespace render {


DepthStencilTest::DepthStencilTest(const DepthStencilTestDescriptor& dsd) {
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
	frameBuffer().bindForDrawing();
	
	// -- run load actions on the attachments
	// In this case (GL 4.1) we will clear the buffers unless they explicitly state Load
	// as the LoadAction. DontCare is interpreted for this engine as Clear.

	// -- colour
	uint32 colourIndex = 0;
	for (auto& colourAction : descriptor.colourActions) {
		if (frameBuffer().hasColourAttachment(colourIndex)) {
			if (colourAction.loadAction != AttachmentLoadAction::Load) {
				glClearBufferfv(GL_COLOR, colourIndex, colourAction.clearColour.data);
			}
		}
		
		++colourIndex;
	}
	
	// -- depth & stencil
	if (frameBuffer().hasDepthAttachment() || frameBuffer().hasStencilAttachment()) {
		if (descriptor.depthActions.loadAction != AttachmentLoadAction::Load &&
			descriptor.stencilActions.loadAction != AttachmentLoadAction::Load)
		{
			// if either buffer doesn't exist, that clear part will just be ignored
			glClearBufferfi(GL_DEPTH_STENCIL, 0, descriptor.depthActions.clearDepth, descriptor.stencilActions.clearStencil);
		}
		else {
			if (frameBuffer().hasDepthAttachment() && descriptor_.depthActions.loadAction == AttachmentLoadAction::Clear) {
				glClearBufferfv(GL_DEPTH, 0, &descriptor.depthActions.clearDepth);
			}
			if (frameBuffer().hasStencilAttachment() && descriptor_.stencilActions.loadAction == AttachmentLoadAction::Clear) {
				GLint stencilVal = static_cast<GLint>(descriptor.stencilActions.clearStencil);
				glClearBufferiv(GL_STENCIL, 0, &stencilVal);
			}
		}
	}
}


RenderPass::~RenderPass() {
	// -- run store actions on the attachments
	// In this particular case (GL 4.1) that means doing nothing
	// in 4.3+ or GL ES we could use glInvalidateFrameBuffer(…) for potential gains

	// -- restore render state
	if (mesh_)
		glBindVertexArray(0);
	if (pipeline_)
		pipeline_->unbind();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void RenderPass::setPipeline(const Pipeline& pipeline) {
	if (pipeline_)
		pipeline_->unbind();

	// FIXME: validate Pipeline against FrameBuffer
	pipeline_ = &pipeline;
	pipeline_->bind();
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


void RenderPass::setMesh(const Mesh& mesh) {
	mesh_ = &mesh;
	mesh_->bind();
}


void RenderPass::drawIndexedPrimitives(uint32 startIndex, uint32 indexCount) {
	assert(mesh_);

	size32 offsetBytes = startIndex * mesh_->indexElementSizeBytes();
	glDrawElements(mesh_->glPrimitiveType(), indexCount, mesh_->glIndexElementType(), reinterpret_cast<void*>(offsetBytes));
}


} // ns render
} // ns stardazed
