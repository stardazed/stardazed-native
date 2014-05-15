// ------------------------------------------------------------------
// render::Shader.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/Shader.hpp"
#include "system/Logging.hpp"
#include "util/TextFile.hpp"

#include <vector>

namespace stardazed {
namespace render {


Shader::Shader(GLenum type, const std::string& source) {
	const auto sourcePtr = source.c_str();
	program_ = glCreateShaderProgramv(type, 1, &sourcePtr);
	
	GLint logLength;
	glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> errors(logLength + 1);
		glGetProgramInfoLog(program_, logLength, NULL, &errors[0]);
		log(errors.data());
	}
	
	mvMatrix_ = glGetUniformLocation(program_, "modelViewMatrix");
	mvpMatrix_ = glGetUniformLocation(program_, "modelViewProjectionMatrix");
	normalMatrix_ = glGetUniformLocation(program_, "normalMatrix");
}


Shader::~Shader() {
	glDeleteProgram(program_);
}


Shader makeShaderWithPath(GLenum type, const std::string& filePath) {
	auto source = readTextFile(filePath);
	return { type, source };
}



} // ns render
} // ns stardazed
