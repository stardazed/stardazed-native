// ------------------------------------------------------------------
// container::RingBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_RINGBUFFER_H
#define SD_CONTAINER_RINGBUFFER_H

#include "system/Config.hpp"
#include "memory/Allocator.hpp"

#include <type_traits>

namespace stardazed {
namespace container {


template <typename T>
class RingBuffer {
	static constexpr bool canSkipElementDestructor = std::is_trivially_destructible<T>::value;

	memory::Allocator& allocator_;
	T* data_;
	uint head_, tail_, capacity_, count_;

	uint next(uint index) const {
		++index;
		if (index == capacity())
			index = 0;
		return index;
	}

	uint prev(uint index) const {
		if (index == 0)
			index = capacity();
		return index - 1;
	}

public:
	RingBuffer(memory::Allocator& allocator, uint capacity)
	: allocator_(allocator)
	{
		assert(capacity > 0);

		data_ = static_cast<T*>(allocator_.alloc(capacity * sizeof(T)));
		head_ = tail_ = 0;
		capacity_ = capacity;
		count_ = 0;
	}
	
	~RingBuffer() {
		clear();
		allocator_.free(data_);
	}


	// -- adding elements	
	void append(T t) {
		assert(count() < capacity());

		data_[tail_] = t;
		tail_ = next(tail_);
		++count_;
	}
	
	void prepend(T t) {
		assert(count() < capacity());

		head_ = prev(head_);
		data_[head_] = t;
		++count_;
	}

	template <typename... Args>
	void emplaceFront(Args&&... args) {
		assert(count() < capacity());

		head_ = prev(head_);
		new (&data_[head_]) T(std::forward<Args>(args)...);
		++count_;
	}
	
	template <typename... Args>
	void emplaceBack(Args&&... args) {
		assert(count() < capacity());

		new (&data_[tail_]) T(std::forward<Args>(args)...);
		tail_ = next(tail_);
		++count_;
	}


	// -- removing elements
	void popFront() {
		assert(count() > 0);

		if (! canSkipElementDestructor) {
			data_[head_].~T();
		}

		head_ = next(head_);
		--count_;
	}
	
	void popBack() {
		assert(count() > 0);

		tail_ = prev(tail_);
		if (! canSkipElementDestructor) {
			data_[tail_].~T();
		}
		--count_;
	}

	void clear() {
		if (! canSkipElementDestructor) {
			auto h = head_;
			auto base = data();

			for (uint c = count(); c; --c) {
				base[h].~T();
				h = next(h);
			}
		}

		head_ = tail_ = 0;
		count_ = 0;
	}
	

	// -- observers
	uint count() const { return count_; }
	uint capacity() const { return capacity_; }
	bool empty() const { return count_ == 0; }
	bool full() const { return count_ == capacity_; }

	const T& front() const {
		assert(count_ > 0);
		return data_[head_];
	}

	T& front() {
		assert(count_ > 0);
		return data_[head_];
	}
	
	const T& back() const {
		assert(count_ > 0);
		return data_[prev(tail_)];
	}

	T& back() {
		assert(count_ > 0);
		return data_[prev(tail_)];
	}
	
	T* data() {
		return data_;
	}
};


} // ns container
} // ns stardazed

#endif
