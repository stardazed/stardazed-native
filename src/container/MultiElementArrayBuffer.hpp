// ------------------------------------------------------------------
// container::MultiElementArrayBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_MULTIELEMENTARRAYBUFFER_H
#define SD_CONTAINER_MULTIELEMENTARRAYBUFFER_H

#include "system/Config.hpp"
#include "memory/Allocator.hpp"
#include "math/Algorithm.hpp"

#include <utility>
#include <functional>

namespace stardazed {
namespace container {
		

namespace detail {

	// -- Compute the sum of the sizes of all types in Ts

	template <typename T>
	constexpr size32 elementSumSize(T* = nullptr) {
		return sizeof32<T>();
	};

	template <typename T, typename... Ts>
	constexpr size32 elementSumSize(T* = nullptr, Ts*... ts = nullptr) {
		return sizeof32<T>() + elementSumSize<Ts...>(std::forward<Ts*>(ts)...);
	};


	// -- Compute the byte offset of type # Index in Ts

	template <uint32 Index, typename T>
	constexpr size32 elementOffset(T* = nullptr) {
		return (Index == 0) ? 0u : sizeof32<T>();
	};

	template <uint32 Index, typename T, typename... Ts>
	constexpr size32 elementOffset(T* = nullptr, Ts*... ts = nullptr) {
		return (Index == 0) ? 0u : sizeof32<T>() + elementOffset<Index - 1, Ts...>(std::forward<Ts*>(ts)...);
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
	// Used internally to in MEAB::reserve
	
	using ItFn = std::function<void(uint32, void*, uint32)>;
	
	template <uint32 Index, typename T>
	void eachArrayBasePtr(void* basePtr, uint32 /*capacity*/, const ItFn& fn, T* = nullptr) {
		fn(Index, basePtr, sizeof32<T>());
	}
	
	template <uint32 Index, typename T, typename... Ts>
	void eachArrayBasePtr(void* basePtr, uint32 capacity, const ItFn& fn, T* = nullptr, Ts*... ts = nullptr) {
		fn(Index, basePtr, sizeof32<T>());

		auto bytePtr = static_cast<uint8*>(basePtr);
		eachArrayBasePtr<Index + 1, Ts...>(bytePtr + (capacity * sizeof(T)), capacity, fn, std::forward<Ts*>(ts)...);
	}

} // ns detail


enum class InvalidatePointers : int {
	No,
	Yes
};


template <typename... Ts>
// requires TrivialType<Ts...>
class MultiElementArrayBuffer {
	uint32 capacity_ = 0, count_ = 0;
	void* data_ = nullptr;
	memory::Allocator& allocator_;

public:
	static constexpr const uint32 elementCount = sizeof...(Ts);

	
	template <uint32 Index>
	static constexpr size32 elementSizeBytes() {
		return sizeof32<typename detail::ElementType<Index, Ts...>::Type>();
	}


	MultiElementArrayBuffer(memory::Allocator& allocator, uint32 initialCount)
	: allocator_(allocator)
	{
		reserve(initialCount);
	}

	
	~MultiElementArrayBuffer() {
		allocator_.free(data_);
	}

	
	uint32 capacity() const { return capacity_; }
	uint32 count() const { return count_; }

	
	InvalidatePointers reserve(uint32 newCapacity) {
		assert(newCapacity > 0);

		// By forcing an allocated multiple of 32 elements, we never have
		// to worry about padding between consecutive arrays. 32 is chosen
		// as it is the AVX layout requirement, so e.g. a char field followed
		// by an m256 field will be aligned regardless of array length.
		// We could align to 16 or even 8 and likely be fine, but this container
		// isn't meant for tiny arrays so 32 it is.
		newCapacity = math::alignUp(newCapacity, 32);
		if (newCapacity <= capacity())
			return InvalidatePointers::No;
		
		auto invalidation = InvalidatePointers::No;
		
		auto newData = allocator_.alloc(newCapacity * detail::elementSumSize<Ts...>());
		assert(newData);

		if (data_) {
			// Since a capacity change will change the length of each array individually
			// we need to re-layout the data in the new buffer.
			// We iterate over the basePointers and copy count_ elements from the old
			// data to each new array. With large arrays >100k elements this can take
			// millisecond-order time, so avoid resizes when possible.

			detail::eachArrayBasePtr<0, Ts...>(data_, capacity_,
				[newDataPtr = (uint8*)newData, newCapacity, usedCount = count_]
				(uint32 /*index*/, void* basePtr, uint32 elementSizeBytes) mutable {
					memcpy(newDataPtr, basePtr, usedCount * elementSizeBytes);
					newDataPtr += elementSizeBytes * newCapacity;
				});
			
			allocator_.free(data_);
			invalidation = InvalidatePointers::Yes;
		}
		
		data_ = newData;
		capacity_ = newCapacity;
		
		return invalidation;
	}

	
	// TODO: add way to cut capacity?
	// TODO: add clear() method -> count_ = 0 & memset(0, instanceData_)
	
	
	InvalidatePointers resize(uint32 newCount) {
		auto invalidation = InvalidatePointers::No;

		if (newCount > capacity()) {
			invalidation = reserve(newCount);
		}
		// TODO: if newCount < count() then clear invalidated entries in all arrays

		count_ = newCount;
		return invalidation;
	}
	
	
	InvalidatePointers append() {
		auto invalidation = InvalidatePointers::No;

		if (count_ == capacity_) {
			invalidation = reserve(capacity() * 2);
		}

		++count_;
		return invalidation;
	}

	
	template <uint32 Index>
	auto elementsBasePtr() const {
		auto basePtr = static_cast<uint8_t*>(data_) + (detail::elementOffset<Index, Ts...>() * capacity_);
		return reinterpret_cast<typename detail::ElementType<Index, Ts...>::Type*>(basePtr);
	}
};

	
} // ns container
} // ns stardazed

#endif
