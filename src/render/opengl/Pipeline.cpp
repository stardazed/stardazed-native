// ------------------------------------------------------------------
// render::opengl::Pipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Pipeline.hpp"

namespace stardazed {
namespace render {


namespace detail {

/*
ShaderConstantMapping::ShaderConstantMapping(GLuint shaderProgram) {
	program_ = shaderProgram;
	mvMat_ = glGetUniformLocation(program_, "modelViewMatrix");
	mvpMat_ = glGetUniformLocation(program_, "modelViewProjectionMatrix");
	normalMat_ = glGetUniformLocation(program_, "normalMatrix");
}


void ShaderConstantMapping::apply(const ConstantBuffer& constants) const {
	if (mvMat_ > -1)
		glProgramUniformMatrix4fv(program_, mvMat_, 1, GL_FALSE, constants.modelViewMatrix().dataBegin());
	if (mvpMat_ > -1)
		glProgramUniformMatrix4fv(program_, mvpMat_, 1, GL_FALSE, constants.modelViewProjectionMatrix().dataBegin());
	if (normalMat_ > -1)
		glProgramUniformMatrix3fv(program_, normalMat_, 1, GL_FALSE, constants.normalMatrix().dataBegin());
}
*/

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
{
	glGenProgramPipelines(1, &glPipeline_);
	
	auto attachShader = [=](const Shader* shader, GLbitfield stage) {
		if (shader) {
			auto shaderName = shader->glShader_;
			glUseProgramStages(glPipeline_, stage, shaderName);
//			shaderConstants.emplace_back(shaderName);
		}
	};
	
	attachShader(descriptor.vertexShader, GL_VERTEX_SHADER_BIT);
	attachShader(descriptor.fragmentShader, GL_FRAGMENT_SHADER_BIT);
}


Pipeline::~Pipeline() {
	glDeleteProgramPipelines(1, &glPipeline_);
}


void Pipeline::activate() {
	glBindProgramPipeline(glPipeline_);
//	for (const auto& sc : shaderConstants)
//		sc.apply(constants_);

	cullingMode_.apply();
	depthTestMode_.apply();
}


void Pipeline::deactivate() {
	glBindProgramPipeline(0);
}


} // ns render
} // ns stardazed
