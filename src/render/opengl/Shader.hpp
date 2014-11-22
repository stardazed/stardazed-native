// ------------------------------------------------------------------
// render::opengl::Shader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLSHADER_H
#define SD_RENDER_OPENGLSHADER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "render/common/Shader.hpp"
#include "render/opengl/OpenGL.hpp"
#include "render/opengl/Buffer.hpp"

#include <string>
#include <vector>

namespace stardazed {
namespace render {


class Pipeline;


constexpr GLenum glForSDShaderType(ShaderType type);


class Shader {
	GLuint glShader_;
	ShaderType type_;
	
public:
	friend Pipeline;
	
	Shader(ShaderType type, const std::string& source);
	~Shader();
	SD_DEFAULT_MOVE_OPS(Shader)

	ShaderType type() const { return type_; }
	
	// ---- Uniforms
	
	void bindUniformIndexToNamedBlock(uint32_t index, const char* blockName);
	void bindUniformIndexToNamedBlock(uint32_t index, const std::string& blockName) {
		bindUniformIndexToNamedBlock(index, blockName.c_str());
	}
};


} // ns render
} // ns stardazed

#endif
