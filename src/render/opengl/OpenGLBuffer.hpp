// ------------------------------------------------------------------
// render::OpenGLBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLBUFFER_H
#define SD_RENDER_OPENGLBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

#include "math/Vector.hpp"
#include "render/common/Mesh.hpp"
#include "render/opengl/OpenGL.hpp"


namespace stardazed {
namespace render {


namespace detail {
	// set the proper default pointer for a Buffer for a contained type T

	template <typename T>
	void setDefaultAttribPointer(GLuint attribIndex);
	
	template <>
	void setDefaultAttribPointer<math::Vec2>(GLuint attribIndex);

	template <>
	void setDefaultAttribPointer<math::Vec3>(GLuint attribIndex);
	
	template <>
	void setDefaultAttribPointer<math::Vec4>(GLuint attribIndex);
	
	template <>
	void setDefaultAttribPointer<render::Tri16>(GLuint attribIndex);
	
	template <>
	void setDefaultAttribPointer<render::Tri32>(GLuint attribIndex);
}


template <typename T, GLenum Type = GL_ARRAY_BUFFER>
class OpenGLBuffer {
	GLuint name_ {0};
	
public:
	OpenGLBuffer() {
		glGenBuffers(1, &name_);
	}

	~OpenGLBuffer() {
		if (name_)
			glDeleteBuffers(1, &name_);
	}
	
	void bind() const {
		glBindBuffer(Type, name_);
	}
	
	template <typename Seq> // Seq = Sequence<T>
	void initialize(const Seq& elements, GLenum usage) const {
		glBindBuffer(Type, name_);
		glBufferData(Type, elements.size() * sizeof(T), elements.data(), usage);
		glBindBuffer(Type, 0);
	}
	
	template <typename Seq> // Seq = Sequence<T>
	void update(const Seq& elements, GLintptr offset = 0) const {
		glBindBuffer(Type, name_);
		glBufferSubData(Type, offset, elements.size() * sizeof(T), elements.data());
		glBindBuffer(Type, 0);
	}
	
	void assignToVAOAttribute(GLuint attribIndex) const {
		glBindBuffer(Type, name_);
		glEnableVertexAttribArray(attribIndex);
		detail::setDefaultAttribPointer<T>(attribIndex);
		glBindBuffer(Type, 0);
	}
};


} // ns render
} // ns stardazed

#endif
