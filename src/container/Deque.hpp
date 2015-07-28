// ------------------------------------------------------------------
// container::Deque - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_DEQUE_H
#define SD_CONTAINER_DEQUE_H

#include "system/Config.hpp"
#include "container/Array.hpp"

#include <type_traits>

namespace stardazed {
namespace container {


template <typename T>
class Deque {
	static constexpr bool canSkipElementDestructor = std::is_trivially_destructible<T>::value;

	memory::Allocator& allocator_;
	container::Array<T*> blocks_;
	uint headBlock_, headIndex_;
	uint tailBlock_, tailIndex_;
	uint count_;

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

	static constexpr uint blockCapacity = sizeof32<T>() < 256 ? 32768 / sizeof32<T>() : 128;
	static constexpr uint blockSizeBytes = blockCapacity * sizeof32<T>();
	
	T* newBlock() const {
		return static_cast<T*>(allocator_.alloc(blockSizeBytes));
	}
	
	T* headPtr() { return blocks_[headBlock_] + headIndex_; }
	T* tailPtr() { return blocks_[tailBlock_] + tailIndex_; }
	

public:
	Deque(memory::Allocator& allocator)
	: allocator_(allocator)
	, blocks_(allocator_, 4)
	{
		blocks_.append(newBlock());

		headBlock_ = tailBlock_ = 0;
		headIndex_ = tailIndex_ = 0;
		count_ = 0;
	}
	
	~Deque() {
//		clear();
//		allocator_.free(data_);
	}


	// -- adding elements	
	void append(T t) {
		if (tailIndex_ == blockCapacity) {
			assert(tailBlock_ == blocks_.count());

			blocks_.append(newBlock());
			tailBlock_++;
			tailIndex_ = 0;
		}

		new (tailPtr()) T(t);
		++tailIndex_;
		++count_;
	}
	
	void prepend(T t) {
		if (headIndex_ == 0) {
			assert(headBlock_ == 0);
			
			blocks_.append(nullptr);
			tailBlock_++;
			tailIndex_ = 0;
		}

		head_ = prev(head_);
		data_[head_] = t;
		++count_;
	}

	template <typename... Args>
	void emplaceFront(Args&&... args) {
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
	static constexpr size32 elementSizeBytes() { return sizeof32<T>(); }
	
	uint count() const { return count_; }
	bool empty() const { return count_ == 0; }

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
};


} // ns container
} // ns stardazed

#endif
