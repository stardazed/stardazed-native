// ------------------------------------------------------------------
// render::opengl::Pipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Pipeline.hpp"

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



Pipeline::Pipeline(const PipelineDescriptor& descriptor)
: cullingMode_(descriptor.faceCulling)
, depthTestMode_(descriptor.depthTest)
, usesPipeline(false)
{
	new (&pipeProgram_) Program();

	if (descriptor.vertexShader) {
		vertexProgram_ = &pipeProgram_;
		pipeProgram_.attach(*descriptor.vertexShader);
	}
	
	if (descriptor.geometryShader) {
		geometryProgram_ = &pipeProgram_;
		pipeProgram_.attach(*descriptor.geometryShader);
	}
	
	if (descriptor.fragmentShader) {
		fragmentProgram_ = &pipeProgram_;
		pipeProgram_.attach(*descriptor.fragmentShader);
	}
	
	pipeProgram_.link();
}


Pipeline::Pipeline(const SSOPipelineDescriptor& descriptor)
: cullingMode_(descriptor.faceCulling)
, depthTestMode_(descriptor.depthTest)
, usesPipeline(true)
{
	glGenProgramPipelines(1, &glPipeline_);
	
	if (descriptor.vertexProgram) {
		vertexProgram_ = descriptor.vertexProgram;
		glUseProgramStages(glPipeline_, GL_VERTEX_SHADER_BIT, vertexProgram_->name());
	}

	if (descriptor.geometryProgram) {
		geometryProgram_ = descriptor.geometryProgram;
		glUseProgramStages(glPipeline_, GL_GEOMETRY_SHADER_BIT, geometryProgram_->name());
	}
	
	if (descriptor.fragmentProgram) {
		fragmentProgram_ = descriptor.fragmentProgram;
		glUseProgramStages(glPipeline_, GL_FRAGMENT_SHADER_BIT, fragmentProgram_->name());
	}
}


Pipeline::~Pipeline() {
	if (usesPipeline)
		glDeleteProgramPipelines(1, &glPipeline_);
	else
		pipeProgram_.~Program();
}


void Pipeline::bind() {
	if (usesPipeline)
		glBindProgramPipeline(glPipeline_);
	else
		pipeProgram_.bind();

	cullingMode_.apply();
	depthTestMode_.apply();
}


} // ns render
} // ns stardazed
