// ------------------------------------------------------------------
// container::RingBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_RINGBUFFER_H
#define SD_CONTAINER_RINGBUFFER_H

#include <array>
#include <cassert>
#include <algorithm>

namespace stardazed {
namespace container {


template <typename T, size_t N>
class RingBuffer {
	std::array<T, N> data;
	size_t head = 0, tail = 0;
	
	inline void checkOverflow() const {
		assert((tail + 1) % N != head);
	}
	
	inline void advance(size_t& index) {
		index = (index + 1) % N;
	}
	
public:
	void pushBack(const T& t) {
		checkOverflow();
		data[tail] = t;
		advance(tail);
	}
	
	void pushBack(T&& t) {
		checkOverflow();
		data[tail] = std::move(t);
		advance(tail);
	}
	
	template <typename... Args>
	void emplaceBack(Args&&... args) {
		checkOverflow();
		new (&data[tail]) T(std::forward<Args>(args)...);
		advance(tail);
	}
	
	bool popFront(T& into) {
		if (head == tail)
			return false;
		auto pos = head;
		advance(head);
		into = std::move(data[pos]);
		return true;
	}
};


} // ns container
} // ns stardazed

#endif
