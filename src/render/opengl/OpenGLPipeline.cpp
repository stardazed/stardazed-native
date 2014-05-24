// ------------------------------------------------------------------
// render::OpenGLPipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/OpenGLPipeline.hpp"

namespace stardazed {
namespace render {


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
	glGenProgramPipelines(1, &glPipeline_.name());
}


OpenGLPipeline::~OpenGLPipeline() {
	glDeleteProgramPipelines(1, &glPipeline_.name());
}


void OpenGLPipeline::attachShader(OpenGLShader* vs) {
	glUseProgramStages(glPipeline_, glStageBitForShaderType(vs->type()), vs->glShader_);
}


void OpenGLPipeline::activate() {
	glBindProgramPipeline(glPipeline_);
}


void OpenGLPipeline::deactivate() {
	glBindProgramPipeline(0);
}


} // ns render
} // ns stardazed
