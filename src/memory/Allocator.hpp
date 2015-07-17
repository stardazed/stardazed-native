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
	
	// allocation traits
	virtual bool allocZeroesMemory() const = 0;
};


//  ___         _               _   _ _              _
// / __|_  _ __| |_ ___ _ __   /_\ | | |___  __ __ _| |_ ___ _ _
// \__ \ || (_-<  _/ -_) '  \ / _ \| | / _ \/ _/ _` |  _/ _ \ '_|
// |___/\_, /__/\__\___|_|_|_/_/ \_\_|_\___/\__\__,_|\__\___/_|
//      |__/

// Allocates memory on the heap using the default system allocator.

struct SystemAllocator final : Allocator {
	void* alloc(size_t sizeBytes) final {
		return calloc(1, sizeBytes);
	}

	void* realloc(void* ptr, size_t newSizeBytes) final {
		return realloc(ptr, newSizeBytes);
	}

	void free(void* ptr) final {
		free(ptr);
	}
	
	bool allocZeroesMemory() const final { return true; }
	
	static SystemAllocator& sharedInstance();

private:
	SystemAllocator() = default; // singleton
};


//  ___ _           _      _   _ _              _
// / __| |_ __ _ __| |__  /_\ | | |___  __ __ _| |_ ___ _ _
// \__ \  _/ _` / _| / / / _ \| | / _ \/ _/ _` |  _/ _ \ '_|
// |___/\__\__,_\__|_\_\/_/ \_\_|_\___/\__\__,_|\__\___/_|
//

// Allocates directly from the stack and thus free() is a nop.
// Use only for small(ish), local allocations.

struct StackAllocator final : Allocator {
	void* alloc(size_t sizeBytes) final {
		return alloca(sizeBytes);
	}
	
	void* realloc(void* /* ptr */, size_t newSizeBytes) final {
		return alloca(newSizeBytes);
	}
	
	void free(void* /* ptr */) final {
		/* nothing, exiting the current function will free */
	}
	
	bool allocZeroesMemory() const final { return false; }
	
	static StackAllocator& sharedInstance();
	
private:
	StackAllocator() = default; // singleton
};


} // ns memory
} // ns stardazed

#endif
