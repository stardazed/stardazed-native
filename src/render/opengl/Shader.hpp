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


class Shader;


class Program {
	GLuint glProgram_;

public:
	Program();
	~Program();
	SD_DEFAULT_MOVE_OPS(Program)
	
	GLuint name() const { return glProgram_; }
	
	// ----
	
	void attach(const Shader&);
	void setSeparable();

	void link();
	
	void bind();


	// ---- Uniform Blocks
	GLuint uniformBlockIndexForName(const char* blockName) {
		return glGetUniformBlockIndex(glProgram_, blockName);
	}
	
	GLuint uniformBlockIndexForName(const std::string& blockName) {
		return uniformBlockIndexForName(blockName.c_str());
	}
	
	void bindUniformBlockToGlobalIndex(GLuint programBlockIndex, GLuint globalIndex) {
		glUniformBlockBinding(glProgram_, programBlockIndex, globalIndex);
	}
	
	// ---- Uniforms
	GLint uniformPositionForName(const char* valueName) {
		return glGetUniformLocation(glProgram_, valueName);
	}
	
	GLint uniformPositionForName(const std::string& valueName) {
		return uniformPositionForName(valueName.c_str());
	}
	
	void setUniform(GLint valuePos, float value) { glProgramUniform1f(glProgram_, valuePos, value); }
	void setUniform(GLint valuePos, int32 value) { glProgramUniform1i(glProgram_, valuePos, value); }
	void setUniform(GLint valuePos, uint32 value) { glProgramUniform1ui(glProgram_, valuePos, value); }
	
	void setUniform(GLint valuePos, const math::Vector<2, float>& value) {
		glProgramUniform2fv(glProgram_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, float>& value) {
		glProgramUniform3fv(glProgram_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, float>& value) {
		glProgramUniform4fv(glProgram_, valuePos, 1, value.data);
	}
	
	void setUniform(GLint valuePos, const math::Vector<2, int32>& value) {
		glProgramUniform2iv(glProgram_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, int32>& value) {
		glProgramUniform3iv(glProgram_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, int32>& value) {
		glProgramUniform4iv(glProgram_, valuePos, 1, value.data);
	}
	
	void setUniform(GLint valuePos, const math::Vector<2, uint32>& value) {
		glProgramUniform2uiv(glProgram_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<3, uint32>& value) {
		glProgramUniform3uiv(glProgram_, valuePos, 1, value.data);
	}
	void setUniform(GLint valuePos, const math::Vector<4, uint32>& value) {
		glProgramUniform4uiv(glProgram_, valuePos, 1, value.data);
	}
	
	void setUniform(GLint valuePos, const math::Matrix<2, 2, float>& value) {
		glProgramUniformMatrix2fv(glProgram_, valuePos, 1, GL_FALSE, value.data);
	}
	void setUniform(GLint valuePos, const math::Matrix<3, 3, float>& value) {
		glProgramUniformMatrix3fv(glProgram_, valuePos, 1, GL_FALSE, value.data);
	}
	void setUniform(GLint valuePos, const math::Matrix<4, 4, float>& value) {
		glProgramUniformMatrix4fv(glProgram_, valuePos, 1, GL_FALSE, value.data);
	}
};


class Shader {
	GLuint glShader_;
	ShaderType type_;
	
public:
	explicit Shader(ShaderType type);
	~Shader();
	SD_DEFAULT_MOVE_OPS(Shader)

	ShaderType type() const { return type_; }
	GLuint name() const { return glShader_; }
	
	void compileSource(const std::string& source);
};


} // ns render
} // ns stardazed

#endif
