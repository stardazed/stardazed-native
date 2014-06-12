// ------------------------------------------------------------------
// memory::STLAllocAdapter - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_STLALLOCADAPTER_H
#define SD_MEMORY_STLALLOCADAPTER_H

#include <cstddef>
#include <memory>

namespace stardazed {
namespace memory {


template <typename T, typename Alloc>
class STLAllocAdapter {
	const Alloc* allocator_;

public:
	template <typename U, typename A>
	friend class AllocAdapter;

	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using value_type = T;
	using size_type = size_t;
	using pointer = T*;
	using const_pointer = const T*;

	using propagate_on_container_move_assignment = std::true_type;

	template <typename U>
	struct rebind { typedef STLAllocAdapter<U, Alloc> other; };

    STLAllocAdapter(); // not implemented but defined for GCC 4.9 compat
	STLAllocAdapter(const Alloc* alloc) noexcept : allocator_{ alloc } {}

	template <class U, class UAlloc>
	STLAllocAdapter(const STLAllocAdapter<U, UAlloc>& rhs) noexcept
	: allocator_{rhs.allocator_}
	{}

	pointer allocate(size_type n) {
		return static_cast<pointer>(allocator_->allocate(sizeof(T) * n));
	}

	void deallocate(pointer p, size_type n) {
		allocator_->deallocate(p, sizeof(T) * n);
	}
};

template <typename T, typename Alloc, typename U>
inline bool operator ==(const STLAllocAdapter<T, Alloc>&, const STLAllocAdapter<U, Alloc>) noexcept { return true; }

template <typename T, typename Alloc, typename U>
inline bool operator !=(const STLAllocAdapter<T, Alloc>&, const STLAllocAdapter<U, Alloc>) noexcept { return false; }


} // ns memory
} // ns stardazed

#endif
