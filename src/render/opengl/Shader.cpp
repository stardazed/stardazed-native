// ------------------------------------------------------------------
// render::opengl::Shader.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Shader.hpp"
#include "system/Logging.hpp"

#include <vector>

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

	assert(!"Unknown ShaderType");
	return GL_NONE;
}


Shader::Shader(ShaderType type, const std::string& source, TransformFeedbackDescriptor* xfbDesc)
: type_(type)
, resource_(glCreateProgram())
{
	const GLuint shader = glCreateShader(glForSDShaderType(type_));

	assert(name());
	assert(shader);

	const auto sourcePtr = source.c_str();
	glShaderSource(shader, 1, &sourcePtr, NULL);
	glCompileShader(shader);

	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (! success) {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			std::vector<char> errors(logLength + 1);
			glGetShaderInfoLog(shader, logLength, NULL, &errors[0]);
			log("GLSL Errors:\n", errors.data());
			assert(!"Shader failed to compile");
		}
	}

	glProgramParameteri(name(), GL_PROGRAM_SEPARABLE, GL_TRUE);

	if (xfbDesc) {
		// FIXME: do the transform feedback varying thing
		usesXFB_ = true;
	}

	glAttachShader(name(), shader);
	glLinkProgram(name());
	glDetachShader(name(), shader);

	GLint status;
	glGetProgramiv(name(), GL_LINK_STATUS, &status);
	if (! status) {
		GLint logLength;
		glGetProgramiv(name(), GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			std::vector<char> errors(logLength + 1);
			glGetProgramInfoLog(name(), logLength, NULL, &errors[0]);
			log("Link Errors:\n", errors.data());
			assert(!"ShaderProgram failed to link");
		}
	}

	glDeleteShader(shader);
}


Shader::~Shader() {
	if (name() > 0) {
		glDeleteProgram(name());
		resource_.clear();
	}
}


void Shader::setTexture(const Texture* texture, uint32 bindPoint, GLint samplerUniformName) {
	glActiveTexture(GL_TEXTURE0 + bindPoint);

	if (texture) {
		assert(! texture->renderTargetOnly());
		bind(texture);
	}
	else {
		// bind a null texture to an arbitrary texture target
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	setUniform(samplerUniformName, static_cast<int32>(bindPoint));
}


void Shader::setSampler(const Sampler* sampler, uint32 bindPoint) {
	GLuint samplerName = 0;
	if (sampler)
		samplerName = sampler->name();

	glBindSampler(bindPoint, samplerName);
}


} // ns render
} // ns stardazed
