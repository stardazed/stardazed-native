// ------------------------------------------------------------------
// render::OpenGLShader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLSHADER_H
#define SD_RENDER_OPENGLSHADER_H

#include "render/Shader.hpp"
#include "render/opengl/OpenGL.hpp"
#include "util/ConceptTraits.hpp"

#include <string>
#include <vector>

namespace stardazed {
namespace render {


class OpenGLPipeline;


constexpr GLenum glForSDShaderType(ShaderType type);


class OpenGLShader : public Shader {
	GLName glShader_;
	ShaderType type_;
	
public:
	friend OpenGLPipeline;
	
	OpenGLShader(ShaderType type, const std::string& source);
	~OpenGLShader();
	SD_DEFAULT_MOVE_OPS(OpenGLShader)

	ShaderType type() const override { return type_; }
};


} // ns render
} // ns stardazed

#endif
