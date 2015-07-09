// ------------------------------------------------------------------
// container::MultiElementArrayBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_MULTIELEMENTARRAYBUFFER_H
#define SD_CONTAINER_MULTIELEMENTARRAYBUFFER_H

#include "system/Config.hpp"
#include <utility>

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

} // ns detail


template <typename... Ts>
// requires TriviallyCopyable<Ts...>
class MultiElementArrayBuffer {
	void* data_ = nullptr;
	uint32 capacity_ = 0, count_ = 0;

public:
	static constexpr const uint32 elementCount = sizeof...(Ts);
	
	template <uint32 Index>
	static constexpr size32 elementSizeBytes() {
		return sizeof32<typename detail::ElementType<Index, Ts...>::Type>();
	}

	MultiElementArrayBuffer(uint32 initialCount) {
		capacity_ = initialCount;
		count_ = initialCount;
		data_ = calloc(capacity_, detail::elementSumSize<Ts...>());
	}
	
	~MultiElementArrayBuffer() {
		free(data_);
	}
	
	uint32 capacity() const { return capacity_; }
	uint32 count() const { return count_; }
	
//	void allocate(uint32 toAdd) {
//		if (count_ + toAdd > capacity_) {
//			// stuff
//		}
//
//		count_ += toAdd;
//	}
	
	template <uint32 Index>
	auto elementsBasePtr() {
		auto basePtr = static_cast<uint8_t*>(data_) + (detail::elementOffset<Index, Ts...>() * capacity_);
		return reinterpret_cast<typename detail::ElementType<Index, Ts...>::Type*>(basePtr);
	}
};

	
		
} // ns container
} // ns stardazed

#endif
