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


enum class GLArrayType {
	Attribute,
	Index
};


namespace detail {
	constexpr GLbitfield glAccessFlagsForBCA(BufferClientAccess access);
	constexpr GLenum glUsageHint(BufferUpdateFrequency frequency, BufferClientAccess typicalAccess);
	constexpr GLenum glTargetForArrayType(GLArrayType);
	constexpr GLenum glBindingNameForTarget(GLenum target);
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
		glBufferData(target_, bytes, data, usage_);
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
		return mapForUpdates<T>(0, byteSize_, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
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
	
	// -- gl binding
	
	void bind() const { glBindBuffer(target_, name_); }
};



// ---- Buffer Binding Helpers

inline GLuint saveAndBind(const GLBuffer& buffer) {
	GLuint currentlyBound;
	glGetIntegerv(detail::glBindingNameForTarget(buffer.target()), reinterpret_cast<GLint*>(&currentlyBound));
	if (currentlyBound != buffer.name())
		buffer.bind();
	
	return currentlyBound;
}


inline void unbindAndRestore(const GLBuffer& buffer, GLuint savedBufferName) {
	if (savedBufferName != buffer.name()) {
		glBindBuffer(buffer.target(), savedBufferName);
	}
}


template <typename F>
void bindAndRestoreBuffer(const GLBuffer& buffer, F&& func) {
	auto currentBuf = saveAndBind(buffer);
	func();
	unbindAndRestore(buffer, currentBuf);
}


} // ns render
} // ns stardazed

#endif
