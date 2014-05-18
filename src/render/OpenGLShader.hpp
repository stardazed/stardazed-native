// ------------------------------------------------------------------
// render::OpenGLShader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLSHADER_H
#define SD_RENDER_OPENGLSHADER_H

#include "render/Shader.hpp"
#include "render/OpenGL.hpp"

#include <string>
#include <vector>

namespace stardazed {
namespace render {

class OpenGLPipeline;


template <ShaderType Type>
GLenum glForSDShaderType();
template <>
GLenum glForSDShaderType<ShaderType::Vertex>() { return GL_VERTEX_SHADER; }
template <>
GLenum glForSDShaderType<ShaderType::TesselationControl>() { return GL_TESS_CONTROL_SHADER; }
template <>
GLenum glForSDShaderType<ShaderType::TesselationEval>() { return GL_TESS_EVALUATION_SHADER; }
template <>
GLenum glForSDShaderType<ShaderType::Geometry>() { return GL_GEOMETRY_SHADER; }
template <>
GLenum glForSDShaderType<ShaderType::Fragment>() { return GL_FRAGMENT_SHADER; }

	
template <ShaderType Type>
class OpenGLShader : public Shader<Type> {
	GLuint glName = 0;

public:
	friend OpenGLPipeline;

	OpenGLShader(const std::string& source) {
		const auto sourcePtr = source.c_str();
		glName = glCreateShaderProgramv(glForSDShaderType<Type>(), 1, &sourcePtr);
		
		GLint logLength;
		glGetProgramiv(glName, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			std::vector<char> errors(logLength + 1);
			glGetProgramInfoLog(glName, logLength, NULL, &errors[0]);
			// log(errors.data());
		}
	}

	~OpenGLShader() {
		if (glName)
			glDeleteProgram(glName);
	}
};


template <ShaderType Type>
using OpenGLShaderRef = std::shared_ptr<OpenGLShader<Type>>;


} // ns render
} // ns stardazed

#endif
