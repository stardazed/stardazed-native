// ------------------------------------------------------------------
// render::OpenGLPipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/OpenGLPipeline.hpp"

namespace stardazed {
namespace render {


namespace detail {

	ShaderConstantMapping::ShaderConstantMapping(GLuint shaderProgram) {
		program_ = shaderProgram;
		mvMat_ = glGetUniformLocation(program_, "modelViewMatrix");
		mvpMat_ = glGetUniformLocation(program_, "modelViewProjectionMatrix");
		normalMat_ = glGetUniformLocation(program_, "normalMatrix");
	}

	void ShaderConstantMapping::apply(const OpenGLConstantBuffer& constants) const {
		if (mvMat_ > -1)
			glProgramUniformMatrix4fv(program_, mvMat_, 1, GL_FALSE, constants.modelViewMatrix().dataBegin());
		if (mvpMat_ > -1)
			glProgramUniformMatrix4fv(program_, mvpMat_, 1, GL_FALSE, constants.modelViewProjectionMatrix().dataBegin());
		if (normalMat_ > -1)
			glProgramUniformMatrix3fv(program_, normalMat_, 1, GL_FALSE, constants.normalMatrix().dataBegin());
	}

} // ns detail



constexpr GLbitfield glStageBitForShaderType(ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:   return GL_VERTEX_SHADER_BIT;
		case ShaderType::Hull:     return GL_TESS_CONTROL_SHADER_BIT;
		case ShaderType::Domain:   return GL_TESS_EVALUATION_SHADER_BIT;
		case ShaderType::Geometry: return GL_GEOMETRY_SHADER_BIT;
		case ShaderType::Fragment: return GL_FRAGMENT_SHADER_BIT;
		case ShaderType::Compute:  return GL_NONE; // FIXME: use 4.3 headers
		case ShaderType::None:     return GL_NONE;
	}
	
	assert(false && "Unknown ShaderType");
}


OpenGLPipeline::OpenGLPipeline() {
	glGenProgramPipelines(1, &glPipeline_);
}


OpenGLPipeline::~OpenGLPipeline() {
	glDeleteProgramPipelines(1, &glPipeline_);
}


void OpenGLPipeline::attachShader(const OpenGLShader& s) {
	glUseProgramStages(glPipeline_, glStageBitForShaderType(s.type()), s.glShader_);
	shaderConstants.emplace_back(s.glShader_);
}


void OpenGLPipeline::activate() {
	glBindProgramPipeline(glPipeline_);
	for (const auto& sc : shaderConstants)
		sc.apply(constants_);
}


void OpenGLPipeline::deactivate() {
	glBindProgramPipeline(0);
}


} // ns render
} // ns stardazed
