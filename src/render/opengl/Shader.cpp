// ------------------------------------------------------------------
// render::opengl::Shader.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Shader.hpp"
#include "system/Logging.hpp"

namespace stardazed {
namespace render {


//  ____
// |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___
// | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \
// |  __/| | | (_) | (_| | | | (_| | | | | | |
// |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|
//                  |___/

Program::Program()
: glProgram_(glCreateProgram())
{}


Program::~Program() {
	glDeleteProgram(glProgram_);
}


void Program::attach(const Shader& shader) {
	glAttachShader(name(), shader.name());
}


void Program::setSeparable() {
	GLint status;
	glGetProgramiv(name(), GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
		assert(!"setSeparable() must be called before link()");

	glProgramParameteri(name(), GL_PROGRAM_SEPARABLE, GL_TRUE);
}


void Program::link() {
	glLinkProgram(name());
	
	GLint status;
	glGetProgramiv(name(), GL_LINK_STATUS, &status);
	if (! status) {
		GLint logLength;
		glGetProgramiv(name(), GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			std::vector<char> errors(logLength + 1);
			glGetProgramInfoLog(name(), logLength, NULL, &errors[0]);
			log("Link Errors:\n", errors.data());
		}
	}
}


void Program::bind() {
	glUseProgram(name());
}


//  ____  _               _
// / ___|| |__   __ _  __| | ___ _ __
// \___ \| '_ \ / _` |/ _` |/ _ \ '__|
//  ___) | | | | (_| | (_| |  __/ |
// |____/|_| |_|\__,_|\__,_|\___|_|
//

constexpr GLenum glForSDShaderType(ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:   return GL_VERTEX_SHADER;
		case ShaderType::Hull:     return GL_TESS_CONTROL_SHADER;
		case ShaderType::Domain:   return GL_TESS_EVALUATION_SHADER;
		case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
		case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	}
	
	assert(!"Unknown ShaderType");
}


Shader::Shader(ShaderType type)
: glShader_(glCreateShader(glForSDShaderType(type)))
, type_(type)
{}


Shader::~Shader() {
	glDeleteShader(glShader_);
}


void Shader::compileSource(const std::string& source) {
	const auto sourcePtr = source.c_str();
	glShaderSource(name(), 1, &sourcePtr, nullptr);
	
	//	glCompileShaderIncludeARB(glShader_, 1, paths, nullptr);
	glCompileShader(name());
	GLint success;
	glGetShaderiv(name(), GL_COMPILE_STATUS, &success);
	if (! success) {
		GLint logLength;
		glGetShaderiv(name(), GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			std::vector<char> errors(logLength + 1);
			glGetShaderInfoLog(name(), logLength, NULL, &errors[0]);
			log("GLSL Errors:\n", errors.data());
		}
	}
}


} // ns render
} // ns stardazed
