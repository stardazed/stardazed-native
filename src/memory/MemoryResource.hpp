// ------------------------------------------------------------------
// memory::MemoryResource - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_MEMORYRESOURCE_H
#define SD_MEMORY_MEMORYRESOURCE_H

#include "system/Config.hpp"

#include <memory.h>

namespace stardazed {
namespace memory {


class Allocator {
public:
	virtual ~Allocator() {}

	virtual void* allocate(size_t) = 0;
	virtual void deallocate(void*, size_t) = 0;
};


class SystemAllocator : public Allocator {
public:
	void* allocate(size_t byteSize) override {
		return std::malloc(byteSize);
	}
	
	void deallocate(void* ptr, size_t) override {
		std::free(ptr);
	}
};


class PoolAllocator : public Allocator {
	size_t size_, offset_ = 0;
	Allocator* alloc_;
	uint8_t* base_ = nullptr;

public:
	PoolAllocator(size_t poolSizeBytes, Allocator* baseAllocator)
	: size_(poolSizeBytes)
	, alloc_(baseAllocator)
	{
		base_ = static_cast<uint8_t*>(baseAllocator->allocate(size_));
	}
	
	~PoolAllocator() {
		baseAllocator->deallocate(base_);
	}

	void* allocate(size_t byteSize) override {
		if (offset_ + byteSize > size_)
			return nullptr;
		auto ret = base_;
		offset_ += byteSize;
		return ret;
	}

	void deallocate(void*, size_t) override {
	}
};


class


} // ns memory
} // ns stardazed

#endif
