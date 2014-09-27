// ------------------------------------------------------------------
// render::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_BUFFER_H
#define SD_RENDER_BUFFER_H

#include "system/Config.hpp"

#include <memory>

namespace stardazed {
namespace render {


enum class AccessLevel {
	ReadOnly,
	WriteOnly,
	ReadWrite
};


template <typename T, AccessLevel>
class BufferAccess;


template <typename T>
class Buffer {
public:
	virtual ~Buffer() = default;
	
	template <AccessLevel Level>
	BufferAccess<T, Level> map() {
		return { mapBufferData(Level) };
	}

private:
	virtual T* mapBufferData(AccessLevel) = 0;
};


template <typename T>
class OpenGLBuffer : public Buffer<T> {
	GLuint ubo_;
	
	T* bufferDataWithAccess(BufferAccess) {
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
		T* blk = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_ONLY);
	}
	
public:
	OpenGLConstantBuffer() {
		glGenBuffers(1, &ubo_);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_DYNAMIC_DRAW);
	}
};


} // ns render
} // ns stardazed

#endif
