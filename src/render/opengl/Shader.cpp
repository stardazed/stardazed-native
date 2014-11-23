// ------------------------------------------------------------------
// render::opengl::Shader.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Shader.hpp"
#include "system/Logging.hpp"

#include <cassert>

namespace stardazed {
namespace render {


constexpr GLenum glForSDShaderType(ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:   return GL_VERTEX_SHADER;
		case ShaderType::Hull:     return GL_TESS_CONTROL_SHADER;
		case ShaderType::Domain:   return GL_TESS_EVALUATION_SHADER;
		case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
		case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	}

	assert(false && "Unknown ShaderType");
}


Shader::Shader(ShaderType type, const std::string& source)
: type_(type)
{
	const auto sourcePtr = source.c_str();
	glShader_ = glCreateShaderProgramv(glForSDShaderType(type), 1, &sourcePtr);
	
	GLint logLength;
	glGetProgramiv(glShader_, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> errors(logLength + 1);
		glGetProgramInfoLog(glShader_, logLength, NULL, &errors[0]);
		log("GLSL Errors:\n%s", errors.data());
	}
}


Shader::~Shader() {
	if (glShader_)
		glDeleteProgram(glShader_);
}


} // ns render
} // ns stardazed
