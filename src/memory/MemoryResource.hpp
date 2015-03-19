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


size_t kiloBytes(size_t kb) { return 1024ul * kb; }
size_t megaBytes(size_t mb) { return 1024ul * kiloBytes(mb); }
size_t gigaBytes(size_t gb) { return 1024ul * megaBytes(gb); }


class SystemAllocator {
	SystemAllocator() = delete;

public:
	static void* allocate(size_t byteSize) {
		return std::calloc(1, byteSize);
	}
	
	static void deallocate(void* ptr, size_t) {
		std::free(ptr);
	}
};


class LinearMemoryPool {
	size_t size_, offset_ = 0;
	uint8* pool_ = nullptr;

public:
	LinearMemoryPool(size_t poolSizeBytes)
	: size_(poolSizeBytes)
	{
		pool_ = static_cast<uint8*>(SystemAllocator::allocate(size_));
	}
	
	~LinearMemoryPool() {
		SystemAllocator::deallocate(pool_, size_);
	}

	void* allocate(size_t byteSize) override {
		assert(offset_ + byteSize <= size_);

		auto ret = pool_ + offset_;
		offset_ += byteSize;
		return ret;
	}
};


} // ns memory
} // ns stardazed

#endif
