// ------------------------------------------------------------------
// render::opengl::RenderPass.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/RenderPass.hpp"

namespace stardazed {
namespace render {


namespace detail {


OpenGLFaceCulling::OpenGLFaceCulling(FaceCulling fc) {
	enabled_ = fc != FaceCulling::Disabled;

	switch (fc) {
		case FaceCulling::Front:
			mode_ = GL_FRONT; break;
		case FaceCulling::Back:
			mode_ = GL_BACK; break;
		case FaceCulling::FrontAndBack:
			mode_ = GL_FRONT_AND_BACK; break;
		default:
			mode_ = GL_NONE; break;
	}
}


inline void OpenGLFaceCulling::apply() const {
	if (enabled_) {
		glEnable(GL_CULL_FACE);
		glCullFace(mode_);
	}
	else
		glDisable(GL_CULL_FACE);
}


OpenGLDepthTest::OpenGLDepthTest(DepthTestPredicate	dt) {
	enabled_ = dt != DepthTestPredicate::Disabled;
	
	switch (dt) {
		case DepthTestPredicate::AllowAll:
			mode_ = GL_ALWAYS; break;
		case DepthTestPredicate::DenyAll:
			mode_ = GL_NEVER; break;
		case DepthTestPredicate::Less:
			mode_ = GL_LESS; break;
		case DepthTestPredicate::LessOrEqual:
			mode_ = GL_LEQUAL; break;
		case DepthTestPredicate::Equal:
			mode_ = GL_EQUAL; break;
		case DepthTestPredicate::NotEqual:
			mode_ = GL_NOTEQUAL; break;
		case DepthTestPredicate::GreaterOrEqual:
			mode_ = GL_GEQUAL; break;
		case DepthTestPredicate::Greater:
			mode_ = GL_GREATER; break;

		default:
			mode_ = GL_NONE; break;
	}
}


inline void OpenGLDepthTest::apply() const {
	if (enabled_) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(mode_);
	}
	else
		glDisable(GL_DEPTH_TEST);
}


} // ns detail


RenderPass::RenderPass(const RenderPassDescriptor& desc) {
	
}


RenderPass::~RenderPass() {
	
}


} // ns render
} // ns stardazed
