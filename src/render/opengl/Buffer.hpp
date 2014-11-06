// ------------------------------------------------------------------
// render::opengl::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLBUFFER_H
#define SD_RENDER_OPENGLBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

#include "math/Vector.hpp"
#include "render/common/Mesh.hpp"
#include "render/opengl/OpenGL.hpp"

#include <cassert>


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


enum class BufferClientAccess {
	None,
	ReadOnly,
	WriteOnly,
	ReadWrite
};


enum class BufferUpdateFrequency {
	Never,
	Occassionally,
	Frequently
};

/*
GL_ARRAY_BUFFER
GL_ELEMENT_ARRAY_BUFFER
GL_DRAW_INDIRECT_BUFFER

GL_COPY_READ_BUFFER
GL_COPY_WRITE_BUFFER

GL_PIXEL_PACK_BUFFER
GL_PIXEL_UNPACK_BUFFER

GL_UNIFORM_BUFFER
GL_TEXTURE_BUFFER
*/


enum class GLArrayType {
	Attribute,
	Index
};


namespace detail {
	GLbitfield glAccessFlagsForBCA(BufferClientAccess access);
	GLenum glUsageHint(BufferUpdateFrequency frequency, BufferClientAccess typicalAccess);
	GLenum glTargetForArrayType(GLArrayType);
}



class Buffer {
	GLuint name_ {0};
	GLenum target_ {0}, usage_ {0};
	size32_t byteSize_ {0};

public:
	Buffer(GLenum target, BufferUpdateFrequency frequency, BufferClientAccess access)
	: target_{ target }
	, usage_{ detail::glUsageHint(frequency, access) }
	{
		glGenBuffers(1, &name_);
	}

	Buffer(GLArrayType type, BufferUpdateFrequency frequency, BufferClientAccess access)
	: Buffer(detail::glTargetForArrayType(type), frequency, access)
	{}
	
	virtual ~Buffer() {
		if (name_)
			glDeleteBuffers(1, &name_);
	}

	void allocate(size32_t bytes, void* data) {
		byteSize_ = bytes;
		bind();
		glBufferData(target_, bytes, data, usage_);
		unbind();
	}
	
	void allocate(size32_t bytes) {
		allocate(bytes, nullptr);
	}
	
	void write(size32_t bytes, void* data, size32_t offset) {
		bind();
		glBufferSubData(target_, offset, bytes, data);
		unbind();
	}
	
	void update(void* data) {
		write(byteSize_, data, 0);
	}
	
	GLuint name() const { return name_; }
	GLenum target() const { return target_; }
	size32_t byteSize() const { return byteSize_; }
	
	void bind() const { glBindBuffer(target_, name_); }
	void unbind() const { glBindBuffer(target_, 0); }
};


template <typename T, GLenum Type = GL_ARRAY_BUFFER>
class GLBuffer {
	GLuint name_ {0};
	
public:
	GLBuffer() {
		glGenBuffers(1, &name_);
	}

	~GLBuffer() {
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
