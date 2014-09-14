// ------------------------------------------------------------------
// memory::MemoryResource - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_MEMORYRESOURCE_H
#define SD_MEMORY_MEMORYRESOURCE_H

#include "system/Config.hpp"

namespace stardazed {
namespace memory {


template <typename T>
class MemoryResource {
public:
	virtual ~MemoryResource() = default;
	
	template <typename... Args>
	T* create(Args&&... args) {
		auto obj = static_cast<T*>(allocate(1));
		
		// allocate can return nullptr but placement new with nullptr
		// argument does nothing
		return new (obj) T(std::forward<Args>(args)...);
	}
	
	template <typename... Args>
	T* createN(size_t count, Args&&... args) {
		auto array = static_cast<T*>(allocate(count));

		if (array) {
			auto ptr = array;
			while (count--)
				new (*ptr) T(std::forward<Args>(args)...);
				++ptr;
			}
		}
		
		return array;
	}
	
	T* createRaw() {
		return static_cast<T*>(allocate(1));
	}
	
	T* createRawN(size_t count) {
		return static_cast<T*>(allocate(count));
	}
	
	void destroy(T* obj) {
		obj.~T();
		deallocate(1, static_cast<void*>(obj));
	}

	void destroyN(size_t count, T* array) {
		auto left = count + 1;
		while (--left) {
			(*array).~T();
			++array;
		}
		deallocate(count, static_cast<void*>(obj));
	}


private:
	// allocate(size_t) returns:
	// - a pointer to a contiguous piece of memory the size of sizeof(T) * count
	// - nullptr when no memory is available from this resource
	virtual void* allocate(size_t count) = 0;

	// For deallocate(void* ptr, size_t count), ptr points to a block of contiguous
	// memory sized sizeof(T) * count bytes, the objects have already been destructed.
	virtual void deallocate(size_t count, void* ptr) = 0;
};


} // ns memory
} // ns stardazed

#endif
