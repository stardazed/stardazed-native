// ------------------------------------------------------------------
// render::OpenGLShader.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/OpenGLShader.hpp"

namespace stardazed {
namespace render {


constexpr GLenum glForSDShaderType(ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:   return GL_VERTEX_SHADER;
		case ShaderType::Hull:     return GL_TESS_CONTROL_SHADER;
		case ShaderType::Domain:   return GL_TESS_EVALUATION_SHADER;
		case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
		case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
		case ShaderType::Compute:  return GL_NONE; // FIXME: use 4.3 headers
	}

	assert(false && "Unknown ShaderType");
}


OpenGLShader::OpenGLShader(ShaderType type, const std::string& source)
: type_(type)
{
	const auto sourcePtr = source.c_str();
	glName_ = glCreateShaderProgramv(glForSDShaderType(type), 1, &sourcePtr);
	
	GLint logLength;
	glGetProgramiv(glName_, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> errors(logLength + 1);
		glGetProgramInfoLog(glName_, logLength, NULL, &errors[0]);
		// log(errors.data());
	}
}


OpenGLShader::~OpenGLShader() {
	if (glName_)
		glDeleteProgram(glName_);
}


} // ns render
} // ns stardazed
