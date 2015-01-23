// ------------------------------------------------------------------
// render::opengl::Shader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLSHADER_H
#define SD_RENDER_OPENGLSHADER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "render/common/Shader.hpp"
#include "render/opengl/OpenGL.hpp"
#include "render/opengl/Buffer.hpp"

#include <string>
#include <vector>

namespace stardazed {
namespace render {


class Pipeline;


class Shader {
	GLuint glShader_;
	ShaderType type_;
	
public:
	friend Pipeline;
	
	Shader(ShaderType type, const std::string& source);
	~Shader();
	SD_DEFAULT_MOVE_OPS(Shader)

	ShaderType type() const { return type_; }
	GLuint name() const { return glShader_; }
	
	// ---- Uniform Blocks
	
	GLuint uniformBlockIndexForName(const char* blockName) {
		return glGetUniformBlockIndex(glShader_, blockName);
	}

	GLuint uniformBlockIndexForName(const std::string& blockName) {
		return uniformBlockIndexForName(blockName.c_str());
	}

	void bindUniformBlockToGlobalIndex(GLuint programBlockIndex, GLuint globalIndex) {
		glUniformBlockBinding(glShader_, programBlockIndex, globalIndex);
	}
	
	// ---- Uniforms
	
	GLint uniformPositionForName(const char* valueName) {
		return glGetUniformLocation(glShader_, valueName);
	}
	
	GLint uniformPositionForName(const std::string& valueName) {
		return uniformPositionForName(valueName.c_str());
	}
	
	void setUniform(GLint valuePos, float value) { glProgramUniform1f(glShader_, valuePos, value); }
	void setUniform(GLint valuePos, int32_t value) { glProgramUniform1i(glShader_, valuePos, value); }
	void setUniform(GLint valuePos, uint32_t value) { glProgramUniform1ui(glShader_, valuePos, value); }

	void setUniform(GLint valuePos, const math::Vector<2, float>& value) {
		glProgramUniform2fv(glShader_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, float>& value) {
		glProgramUniform3fv(glShader_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, float>& value) {
		glProgramUniform4fv(glShader_, valuePos, 1, value.data);
	}

	void setUniform(GLint valuePos, const math::Vector<2, int32_t>& value) {
		glProgramUniform2iv(glShader_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, int32_t>& value) {
		glProgramUniform3iv(glShader_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, int32_t>& value) {
		glProgramUniform4iv(glShader_, valuePos, 1, value.data);
	}
	
	void setUniform(GLint valuePos, const math::Vector<2, uint32_t>& value) {
		glProgramUniform2uiv(glShader_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, uint32_t>& value) {
		glProgramUniform3uiv(glShader_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, uint32_t>& value) {
		glProgramUniform4uiv(glShader_, valuePos, 1, value.data);
	}
	
	void setUniform(GLint valuePos, const math::Matrix<2, 2, float>& value) {
		glProgramUniformMatrix2fv(glShader_, valuePos, 1, GL_FALSE, value.data);
	}
	void setUniform(GLint valuePos, const math::Matrix<3, 3, float>& value) {
		glProgramUniformMatrix3fv(glShader_, valuePos, 1, GL_FALSE, value.data);
	}
	void setUniform(GLint valuePos, const math::Matrix<4, 4, float>& value) {
		glProgramUniformMatrix4fv(glShader_, valuePos, 1, GL_FALSE, value.data);
	}
};


} // ns render
} // ns stardazed

#endif
