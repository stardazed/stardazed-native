// ------------------------------------------------------------------
// memory::Allocator - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_ALLOCATOR_H
#define SD_MEMORY_ALLOCATOR_H

#include "system/Config.hpp"
#include <cstring>

namespace stardazed {
namespace memory {


struct Allocator {
	virtual ~Allocator() = default;
	
	virtual void* alloc(size_t sizeBytes) = 0;
	virtual void* realloc(void* ptr, size_t newSizeBytes) = 0;
	virtual void free(void* ptr) = 0;
};


struct SystemAllocator final : public Allocator {
	void* alloc(size_t sizeBytes) final {
		return calloc(1, sizeBytes);
	}

	void* realloc(void* ptr, size_t newSizeBytes) final {
		return realloc(ptr, newSizeBytes);
	}

	void free(void* ptr) final {
		free(ptr);
	}
	
	static SystemAllocator& sharedInstance();
};


} // ns memory
} // ns stardazed

#endif
