// ------------------------------------------------------------------
// render::opengl::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLBUFFER_H
#define SD_RENDER_OPENGLBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

#include "math/Vector.hpp"
#include "render/common/BufferStorage.hpp"
#include "render/common/Mesh.hpp"
#include "render/opengl/OpenGL.hpp"

#include <cassert>


namespace stardazed {
namespace render {


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


class GLBuffer {
	GLuint name_ {0};
	GLenum target_ {0}, usage_ {0};
	size32_t byteSize_ {0};

public:
	GLBuffer(GLenum target, BufferUpdateFrequency frequency, BufferClientAccess access)
	: target_{ target }
	, usage_{ detail::glUsageHint(frequency, access) }
	{
		glGenBuffers(1, &name_);
	}

	GLBuffer(GLArrayType type, BufferUpdateFrequency frequency, BufferClientAccess access)
	: GLBuffer(detail::glTargetForArrayType(type), frequency, access)
	{}
	
	~GLBuffer() {
		if (name_)
			glDeleteBuffers(1, &name_);
	}

	// -- initialization

	void allocate(size32_t bytes, void* data) {
		byteSize_ = bytes;
		bind();
		glBufferData(target_, bytes, data, usage_);
		unbind();
	}
	
	void allocate(size32_t bytes) {
		allocate(bytes, nullptr);
	}
	
	void allocate(const BufferStorage& storage) {
		allocate(static_cast<size32_t>(storage.byteSize()), storage.getAs<void*>());
	}

	// -- direct updates
	
	void write(size32_t bytes, void* data, size32_t offset) {
		glBufferSubData(target_, offset, bytes, data);
	}
	
	void bindAndWrite(size32_t bytes, void* data, size32_t offset) {
		bind();
		write(bytes, data, offset);
		unbind();
	}

	// -- memory mapped access
private:
	template <typename T>
	T* mapForUpdates(size32_t offset, size32_t bytes, GLbitfield flags) {
		assert(offset + bytes < byteSize_);
		return static_cast<T*>(glMapBufferRange(target_, offset, bytes, flags));
	}

public:
	template <typename T>
	const T* mapRangeForReading(size32_t offset, size32_t bytes) {
		assert(offset + bytes < byteSize_);
		return static_cast<const T*>(glMapBufferRange(target_, offset, bytes, GL_MAP_READ_BIT));
	}
	
	template <typename T>
	const T* mapBufferForReading() {
		return mapRangeForReading<T>(0, byteSize_);
	}
	
	template <typename T>
	T* mapRangeForWriting(size32_t offset, size32_t bytes) {
		return mapForUpdates<T>(offset, bytes, GL_MAP_WRITE_BIT);
	}
	
	template <typename T>
	T* mapBufferForWriting() {
		return mapRangeForWriting<T>(0, byteSize_);
	}
	
	template <typename T>
	T* invalidateAndMapRangeForWriting(size32_t offset, size32_t bytes) {
		return mapForUpdates<T>(offset, bytes, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	}
	
	template <typename T>
	T* invalidateAndMapBufferForWriting() {
		return invalidateAndMapRangeForWriting<T>(0, byteSize_);
	}
	
	template <typename T>
	T* mapRangeForFullAccess(size32_t offset, size32_t bytes) {
		return mapForUpdates<T>(offset, bytes, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	}
	
	template <typename T>
	T* mapBufferForFullAccess() {
		return mapRangeForFullAccess<T>(0, byteSize_);
	}
	
	// -- specialized

	void bindRangeToSubIndex(size32_t offset, size32_t bytes, uint32_t index) {
		assert(target_ == GL_UNIFORM_BUFFER || target_ == GL_TRANSFORM_FEEDBACK_BUFFER);
		assert(offset + bytes < byteSize_);
		glBindBufferRange(target_, index, name_, static_cast<GLintptr>(offset), bytes);
	}
	
	// -- observers

	GLuint name() const { return name_; }
	GLenum target() const { return target_; }
	size32_t byteSize() const { return byteSize_; }
	
	void bind() const { glBindBuffer(target_, name_); }
	void unbind() const { glBindBuffer(target_, 0); }
};


} // ns render
} // ns stardazed

#endif
