// ------------------------------------------------------------------
// memory::MemoryResource - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_MEMORYRESOURCE_H
#define SD_MEMORY_MEMORYRESOURCE_H

namespace stardazed {
namespace memory {


template <typename T>
class MemoryResource {
public:
	virtual ~MemoryResource() = default;
	
	template <typename... Args>
	T* create(Args&&... args) {
		auto obj = static_cast<T*>(allocate(1));
		new (obj) T(std::forward<Args>(args)...);
	}
	
	template <typename... Args>
	T* createN(size_t count, Args&&... args) {
		auto array = static_cast<T*>(allocate(count));

		while (count--)
			new (*array) T(std::forward<Args>(args)...);
			++array;
		}
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

	// for deallocate(size_t, void* ptr), ptr points to a block of contiguous
	// memory sized sizeof(T) * count bytes, the objects have already been destructed.
	virtual void deallocate(size_t count, void* ptr) = 0;
};


} // ns memory
} // ns stardazed

#endif
