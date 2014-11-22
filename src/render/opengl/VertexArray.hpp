// ------------------------------------------------------------------
// render::opengl::VertexArray - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLVERTEXARRAY_H
#define SD_RENDER_OPENGLVERTEXARRAY_H

#include "system/Config.hpp"
#include "render/common/VertexBuffer.hpp"
#include "render/opengl/OpenGL.hpp"

namespace stardazed {
namespace render {


class GLVertexArray {
	GLuint glVAO_ = 0;
	
public:
	GLVertexArray() {
		glGenVertexArrays(1, &glVAO_);
	}
	
	~GLVertexArray() {
		if (glVAO_)
			glDeleteVertexArrays(1, &glVAO_);
	}
	
	// -- attributes

	void bindVertexBufferAttributes(const VertexBuffer&, uint32_t startBoundIndex = 0);

	// -- observers

	GLuint name() const { return glVAO_; }

	// -- binding
	
	void bind() const { glBindVertexArray(glVAO_); }
};


// ---- VAO Binding Specializations

template <>
inline GLuint saveAndBind(const GLVertexArray& va) {
	GLuint currentlyBound;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, reinterpret_cast<GLint*>(&currentlyBound));
	if (currentlyBound != va.name())
		va.bind();
	
	return currentlyBound;
}

template <>
inline void unbindAndRestore(const GLVertexArray& vao, GLuint savedVAOName) {
	if (savedVAOName != vao.name()) {
		glBindVertexArray(savedVAOName);
	}
}


} // ns render
} // ns stardazed


#endif
