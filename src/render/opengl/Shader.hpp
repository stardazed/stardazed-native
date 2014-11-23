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
	
	void setUniform(GLint valuePos, float value) { glUniform1f(valuePos, value); }
	void setUniform(GLint valuePos, int32_t value) { glUniform1i(valuePos, value); }
	void setUniform(GLint valuePos, uint32_t value) { glUniform1ui(valuePos, value); }

	void setUniform(GLint valuePos, const math::Vector<2, float>& value) {
		glUniform2fv(valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, float>& value) {
		glUniform3fv(valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, float>& value) {
		glUniform4fv(valuePos, 1, value.data);
	}

	void setUniform(GLint valuePos, const math::Vector<2, int32_t>& value) {
		glUniform2iv(valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, int32_t>& value) {
		glUniform3iv(valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, int32_t>& value) {
		glUniform4iv(valuePos, 1, value.data);
	}
	
	void setUniform(GLint valuePos, const math::Vector<2, uint32_t>& value) {
		glUniform2uiv(valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, uint32_t>& value) {
		glUniform3uiv(valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, uint32_t>& value) {
		glUniform4uiv(valuePos, 1, value.data);
	}
	
	void setUniform(GLint valuePos, const math::Matrix<2, 2, float>& value) {
		glUniformMatrix2fv(valuePos, 1, GL_FALSE, value.data);
	}
	void setUniform(GLint valuePos, const math::Matrix<3, 3, float>& value) {
		glUniformMatrix3fv(valuePos, 1, GL_FALSE, value.data);
	}
	void setUniform(GLint valuePos, const math::Matrix<4, 4, float>& value) {
		glUniformMatrix4fv(valuePos, 1, GL_FALSE, value.data);
	}
};


} // ns render
} // ns stardazed

#endif
