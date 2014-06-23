// ------------------------------------------------------------------
// render::OpenGLBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLBUFFER_H
#define SD_RENDER_OPENGLBUFFER_H

#include "render/opengl/OpenGL.hpp"
#include "util/ConceptTraits.hpp"


namespace stardazed {
namespace render {


template <typename T, GLenum Type = GL_ARRAY_BUFFER>
class OpenGLBuffer {
	mutable GLuint name {0};
	
public:
	~OpenGLBuffer() {
		if (name)
			glDeleteBuffers(1, &name);
		name = 0;
	}
	
	void bind() const {
		glBindBuffer(Type, name);
	}
	
	void upload() const {
		glGenBuffers(1, &name);
		glBindBuffer(Type, name);
		glBufferData(Type, elements.size() * sizeof(T), elements.data(), GL_STATIC_DRAW);
		glBindBuffer(Type, 0);
	}
	
	void makeVAAttribute(GLuint attribIndex) const {
		glBindBuffer(Type, name);
		glEnableVertexAttribArray(attribIndex);
		setDefaultAttribPointer<T>(attribIndex);
		glBindBuffer(Type, 0);
	}
};


} // ns render
} // ns stardazed

#endif
