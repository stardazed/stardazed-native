// ------------------------------------------------------------------
// render::BufferStorage - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_BUFFERSTORAGE_H
#define SD_RENDER_BUFFERSTORAGE_H

#include "system/Config.hpp"

#include <alloca.h>
#include <memory>

namespace stardazed {
namespace render {


class BufferStorage {
	virtual uint8_t* get() const = 0;
	
public:
	virtual ~BufferStorage() = default;
	
	template <typename T>
	T* getAs() const {
		return reinterpret_cast<T*>(get());
	}
	
	virtual size_t byteSize() const = 0;
};


class OwnedBufferStorage : public BufferStorage {
	std::unique_ptr<uint8_t[]> ptr_;
	size_t byteSize_;

	uint8_t* get() const override {
		return ptr_.get();
	}
	
public:
	OwnedBufferStorage(size_t byteSize)
	: ptr_(std::make_unique<uint8_t[]>(byteSize))
	, byteSize_{byteSize}
	{}
	
	size_t byteSize() const override {
		return byteSize_;
	}
};


} // ns render
} // ns stardazed

#endif
