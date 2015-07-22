// ------------------------------------------------------------------
// container::Array - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_ARRAY_H
#define SD_CONTAINER_ARRAY_H

#include "system/Config.hpp"
#include "memory/Allocator.hpp"

#include <type_traits>

namespace stardazed {
namespace container {


template <typename T>
// requires DefaultConstructible<T>
// prefers TriviallyDefaultConstructible<T> && TriviallyDestructible<T>
class Array {
	static_assert(std::is_default_constructible<T>::value, "T must be default constructible");
	
	static constexpr bool canSkipElementConstructor = std::is_trivially_default_constructible<T>::value;
	static constexpr bool canSkipElementDestructor = std::is_trivially_destructible<T>::value;

	uint32 capacity_ = 0, count_ = 0;
	T* data_ = nullptr;
	memory::Allocator& allocator_;
	

	void destructRange(T* first, uint32 count) {
		// Call destructor for each deleted value for Ts that need it.
		while (count--) {
			first->~T();
			++first;
		}
	}


public:
	Array(memory::Allocator& allocator, uint32 initialCapacity)
	: allocator_(allocator)
	{
		reserve(initialCapacity);
	}

	~Array() {
		if (! canSkipElementDestructor) {
			destructRange(data_, count());
		}

		allocator_.free(data_);
	}


	// -- observers

	static constexpr size32 elementSizeBytes() { return sizeof32<T>(); }
	
	uint32 capacity() const { return capacity_; }
	uint32 count() const { return count_; }
	
	const T* elementsBasePtr() const { return const_cast<const T*>(data_); }
	T* elementsBasePtr() { return data_; }
	

	// -- storage sizing and object lifetime

	void reserve(uint32 newCapacity) {
		assert(newCapacity > 0);

		if (newCapacity <= capacity()) {
			return;
		}
		
		auto newSizeBytes = newCapacity * elementSizeBytes();
		
		auto newData = static_cast<T*>(allocator_.alloc(newSizeBytes));
		assert(newData);
		
		// Not all Allocators return zero-filled memory but this container guarantees
		// zeroed elements for default constructed values so we pre-clear the memory
		if (canSkipElementConstructor && ! allocator_.allocZeroesMemory()) {
			memset(newData, 0, newSizeBytes);
		}

		if (data_) {
			// Copy over the data to the new buffer and free the old one
			memcpy(newData, data_, count() * elementSizeBytes());
			allocator_.free(data_);
		}
		
		data_ = newData;
		capacity_ = newCapacity;
	}

	
	void clear() {
		if (! canSkipElementDestructor) {
			destructRange(data_, count());
		}

		// Clear memory for potential newly default-constructed values after the clear()
		if (canSkipElementConstructor) {
			memset(data_, 0, count() * elementSizeBytes());
		}

		count_ = 0;
	}

	
	void resize(uint32 newCount) {
		auto oldCount = count();

		if (newCount > oldCount) {
			if (newCount > capacity()) {
				reserve(newCount);
			}

			// Trivial types are just zeroed, but if there is a user-specified default constructor
			// or if explicit field initialization was used then we need to default-construct
			// new values (in-place).
			if (! canSkipElementConstructor) {
				auto elementsToConstruct = newCount - oldCount;
				auto newElementPtr = data_ + oldCount;

				// If we were to know that a constructor has no side effects, then we could
				// store a single constructed instance and splat it out over the region.
				while (elementsToConstruct--) {
					new (newElementPtr) T{};
					++newElementPtr;
				}
			}
		}
		else if (newCount < oldCount) {
			// Call destructor for each deleted value for Ts that need it.
			if (! canSkipElementDestructor) {
				destructRange(data_ + newCount, oldCount - newCount);
			}

			// For trivial Ts we zero the released memory now so append()s don't have to.
			if (canSkipElementConstructor) {
				auto elementsToClear = oldCount - newCount;
				auto firstDeletedElementPtr = data_ + newCount;
				memset(firstDeletedElementPtr, 0, elementSizeBytes() * elementsToClear);
			}
		}

		count_ = newCount;
	}
	

	// -- adding elements

	void append(const T& t) {
		if (__builtin_expect(count() == capacity(), 0)) {
			reserve(capacity() * 2);
		}

		// copy construct in place
		new (data_ + count_) T{t};
		
		++count_;
	}


	void emplaceBack() {
		if (__builtin_expect(count() == capacity(), 0)) {
			reserve(capacity() * 2);
		}

		// default-construct in place
		if (! canSkipElementConstructor) {
			new (data_ + count_) T{};
		}
		
		++count_;
	}

	
	template <typename... Args>
	void emplaceBack(Args&&... args) {
		if (__builtin_expect(count() == capacity(), 0)) {
			reserve(capacity() * 2);
		}

		// construct in place
		new (data_ + count_) T{std::forward<Args>(args)...};
		
		++count_;
	}


	// -- removing elements
	
	void remove(uint32 startIndex, uint32 elementsToRemove = 1) {
		assert(startIndex + elementsToRemove - 1 < count());

		// Call destructor for each deleted value for Ts that need it.
		if (! canSkipElementDestructor) {
			destructRange(data_ + startIndex, elementsToRemove);
		}

		// Shift back items beyond the deleted range
		int elementsToCopy = count() - startIndex - elementsToRemove;
		if (elementsToCopy > 0) {
			// FIXME: check for trivially_copyable
			memmove(
					data_ + startIndex,
					data_ + startIndex + elementsToRemove,
					elementSizeBytes() * elementsToCopy
			);
		}

		count_ -= elementsToRemove;
		
		// Pre-clear now unused object space
		if (canSkipElementConstructor) {
			auto elementsToClear = elementsToRemove;
			auto firstDeletedElementPtr = data_ + count();
			memset(firstDeletedElementPtr, 0, elementSizeBytes() * elementsToClear);
		}
	}


	// -- subscripting
	
	T& operator[](size_t index) {
		assert(index < count());
		return *(data_ + index);
	}

	const T& operator[](size_t index) const {
		assert(index < count());
		return *(data_ + index);
	}
};


} // ns container
} // ns stardazed

#endif
