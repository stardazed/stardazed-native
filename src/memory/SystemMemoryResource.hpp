// ------------------------------------------------------------------
// memory::MemoryResource - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_SYSTEMMEMORYRESOURCE_H
#define SD_MEMORY_SYSTEMMEMORYRESOURCE_H

#include "memory/MemoryResource.hpp"
#include <cstdlib>

namespace stardazed {
namespace memory {


template <typename T>
class SystemMemoryResource : public MemoryResource<T> {

void* allocate(size_t count) override {
	return std::malloc(sizeof(T) * count);
}

void deallocate(size_t count, void* ptr) override {
	std::free(ptr);
}

};


} // ns memory
} // ns stardazed

#endif
