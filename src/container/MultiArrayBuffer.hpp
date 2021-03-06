// ------------------------------------------------------------------
// container::MultiArrayBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_MULTIARRAYBUFFER_H
#define SD_CONTAINER_MULTIARRAYBUFFER_H

#include "system/Config.hpp"
#include "memory/Allocator.hpp"
#include "math/Algorithm.hpp"

#include <utility>
#include <functional>

namespace stardazed {
namespace container {
		

namespace detail {

	// -- Compute the sum of the sizes of all types in Ts
	
	template <size_t Index, typename T, typename... Ts>
	struct ElementSumSize {
		static constexpr size32 value() {
			return sizeof32<T>() + ElementSumSize<Index - 1, Ts...>::value();
		}
	};
	
	template <typename T, typename... Ts>
	struct ElementSumSize<0, T, Ts...> {
		static constexpr size32 value() {
			return sizeof32<T>();
		}
	};
	
	template <typename... Ts>
	constexpr size32 elementSumSize() {
		return ElementSumSize<sizeof...(Ts) - 1, Ts...>::value();
	}


	// -- Compute the byte offset of type # Index in Ts

	template <size_t Index, typename T, typename... Ts>
	struct ElementOffset {
		static constexpr size32 value() {
			return sizeof32<T>() + ElementOffset<Index - 1, Ts...>::value();
		}
	};
	
	template <typename T, typename... Ts>
	struct ElementOffset<0, T, Ts...> {
		static constexpr size32 value() {
			return 0;
		}
	};
	
	template <size_t Index, typename... Ts>
	constexpr size32 elementOffset() {
		return ElementOffset<Index, Ts...>::value();
	};


	// -- Get type # Index from Ts

	template <uint32 Index, typename T, typename... Ts>
	struct ElementType {
		using Type = typename ElementType<Index - 1, Ts...>::Type;
	};

	template <typename T, typename... Ts>
	struct ElementType<0, T, Ts...> {
		using Type = T;
	};
	
	
	// -- Call a callback with base pointer and element size info for each T
	// Used internally in MAB::reserve
	
	using ItFn = std::function<void(void*, uint32)>;
	
	template <size_t Counter, typename T, typename... Ts>
	struct EachArrayBasePtr {
		static void apply(void* basePtr, uint32 capacity, const ItFn& fn) {
			fn(basePtr, sizeof32<T>());
			
			auto bytePtr = static_cast<uint8*>(basePtr);
			EachArrayBasePtr<Counter - 1, Ts...>::apply(bytePtr + (capacity * sizeof(T)), capacity, fn);
		}
	};
	
	template <typename T, typename... Ts>
	struct EachArrayBasePtr<0, T, Ts...> {
		static void apply(void* basePtr, uint32 /*capacity*/, const ItFn& fn) {
			fn(basePtr, sizeof32<T>());
		}
	};
	
	template <typename... Ts>
	void eachArrayBasePtr(void* basePtr, uint32 capacity, const ItFn& fn) {
		EachArrayBasePtr<sizeof...(Ts) - 1, Ts...>::apply(basePtr, capacity, fn);
	}

} // ns detail


enum class InvalidatePointers : int {
	No,
	Yes
};


template <typename... Ts>
class MultiArrayBuffer {
	memory::Allocator* allocator_;
	uint32 capacity_ = 0, count_ = 0;
	void* data_ = nullptr;

public:
	static constexpr uint32 elementCount = sizeof...(Ts);
	static constexpr float growthFactor_s = 1.5f;
	
	template <uint32 Index>
	static constexpr size32 elementSizeBytes() {
		return sizeof32<typename detail::ElementType<Index, Ts...>::Type>();
	}


	MultiArrayBuffer(memory::Allocator& allocator, uint32 initialCapacity)
	: allocator_(&allocator)
	{
		reserve(initialCapacity);
	}

	
	~MultiArrayBuffer() {
		allocator_->free(data_);
	}

	
	uint32 capacity() const { return capacity_; }
	uint32 count() const { return count_; }
	uint32 backIndex() const {
		assert(count() > 0);
		return count() - 1;
	}
	
	memory::Allocator& allocator() const { return *allocator_; }

	
	InvalidatePointers reserve(uint32 newCapacity) {
		assert(newCapacity > 0);

		// By forcing an allocated multiple of 32 elements, we never have
		// to worry about padding between consecutive arrays. 32 is chosen
		// as it is the AVX layout requirement, so e.g. a char field followed
		// by an m256 field will be aligned regardless of array length.
		// We could align to 16 or even 8 and likely be fine, but this container
		// isn't meant for tiny arrays so 32 it is.

		newCapacity = math::alignUp(newCapacity, 32);
		if (newCapacity <= capacity()) {
			// TODO: add way to cut capacity?
			return InvalidatePointers::No;
		}
		
		auto invalidation = InvalidatePointers::No;
		auto newSizeBytes = newCapacity * detail::elementSumSize<Ts...>();
		
		auto newData = allocator_->alloc(newSizeBytes);
		assert(newData);
		
		// Not all Allocators return zero-filled memory but this container guarantees
		// zeroed elements so we clear the memory explicitly if necessary.
		if (! allocator_->allocZeroesMemory()) {
			memset(newData, 0, newSizeBytes);
		}

		if (data_) {
			// Since a capacity change will change the length of each array individually
			// we need to re-layout the data in the new buffer.
			// We iterate over the basePointers and copy count_ elements from the old
			// data to each new array. With large arrays >100k elements this can take
			// millisecond-order time, so avoid resizes when possible.

			detail::eachArrayBasePtr<Ts...>(data_, capacity_,
				[newDataPtr = (uint8*)newData, newCapacity, usedCount = count_]
				(void* basePtr, uint32 elementSizeBytes) mutable {
					memcpy(newDataPtr, basePtr, usedCount * elementSizeBytes);
					newDataPtr += elementSizeBytes * newCapacity;
				});
			
			allocator_->free(data_);
			invalidation = InvalidatePointers::Yes;
		}
		
		data_ = newData;
		capacity_ = newCapacity;
		
		return invalidation;
	}

	
	void clear() {
		count_ = 0;
		memset(data_, 0, capacity() * detail::elementSumSize<Ts...>());
	}

	
	InvalidatePointers resize(uint32 newCount) {
		auto invalidation = InvalidatePointers::No;

		if (newCount > capacity()) {
			invalidation = reserve(newCount);
		}
		else if (newCount < count()) {
			// Reducing the count will clear the now freed up elements so that when
			// a new allocation is made the element data is guaranteed to be zeroed.

			auto elementsToClear = count() - newCount;
			detail::eachArrayBasePtr<Ts...>(data_, capacity_,
				[elementsToClear, fromIndex = newCount]
				(void* basePtr, uint32 elementSizeBytes) {
					auto firstDeletedElementPtr = static_cast<uint8*>(basePtr) + (elementSizeBytes * fromIndex);
					memset(firstDeletedElementPtr, 0, elementSizeBytes * elementsToClear);
				});

		}

		count_ = newCount;
		return invalidation;
	}
	
	
	InvalidatePointers extend() {
		auto invalidation = InvalidatePointers::No;

		if (count_ == capacity_) {
			invalidation = reserve(capacity_ * growthFactor_s);
		}

		++count_;
		return invalidation;
	}

	
	template <uint32 Index>
	auto elementsBasePtr() const {
		auto basePtr = static_cast<uint8_t*>(data_) + (detail::elementOffset<Index, Ts...>() * capacity_);
		return reinterpret_cast<typename detail::ElementType<Index, Ts...>::Type*>(basePtr);
	}
	
	
	void swap(MultiArrayBuffer& other) {
		std::swap(allocator_, other.allocator_);
		std::swap(capacity_, other.capacity_);
		std::swap(count_, other.count_);
		std::swap(data_, other.data_);
	}
};

	
} // ns container
} // ns stardazed

#endif
