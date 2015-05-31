// ------------------------------------------------------------------
// render::opengl::Shader - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_SHADER_H
#define SD_RENDER_OPENGL_SHADER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "render/common/Shader.hpp"
#include "render/opengl/OpenGL.hpp"

#include <string>

namespace stardazed {
namespace render {


class Shader {
	ShaderType type_;
	GLuint glProgram_ = 0;
	bool32 usesXFB_ = false;

public:
	Shader(ShaderType, const std::string& source, TransformFeedbackDescriptor* = nullptr);
	~Shader();
	SD_DEFAULT_MOVE_OPS(Shader)

	// -- observers
	ShaderType type() const { return type_; }
	GLuint name() const { return glProgram_; }
	bool32 usesTransformFeedback() const { return usesXFB_; }

	// -- Uniform Blocks
	GLuint uniformBlockIndexForName(const char* blockName) const {
		return glGetUniformBlockIndex(glProgram_, blockName);
	}
	
	GLuint uniformBlockIndexForName(const std::string& blockName) const {
		return uniformBlockIndexForName(blockName.c_str());
	}
	
	void bindUniformBlockToGlobalIndex(GLuint programBlockIndex, GLuint globalIndex) {
		glUniformBlockBinding(glProgram_, programBlockIndex, globalIndex);
	}
	
	// -- Uniforms
	GLint uniformPositionForName(const char* valueName) const {
		return glGetUniformLocation(glProgram_, valueName);
	}
	
	GLint uniformPositionForName(const std::string& valueName) const {
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


} // ns render
} // ns stardazed

#endif
