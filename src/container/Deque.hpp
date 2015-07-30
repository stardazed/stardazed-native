// ------------------------------------------------------------------
// container::Deque - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_DEQUE_H
#define SD_CONTAINER_DEQUE_H

#include "system/Config.hpp"
#include "container/Array.hpp"
#include "container/Algorithm.hpp"

#include <type_traits>

namespace stardazed {
namespace container {


template <typename T>
class Deque {
	static constexpr bool canSkipElementDestructor = std::is_trivially_destructible<T>::value;

	memory::Allocator& allocator_;
	Array<T*> blocks_;
	uint headBlock_, headIndex_;
	uint tailBlock_, tailIndex_;
	uint count_;


	// -- block access
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
		clear();
		
		// clear() removes all blocks past the first, so we only
		// need to free the last remaining one here
		allocator_.free(blocks_.front());
	}


	// -- adding elements
	template <typename... Args>
	void emplaceBack(Args&&... args) {
		if (tailIndex_ == blockCapacity) {
			if (tailBlock_ == blocks_.count() - 1) {
				blocks_.append(newBlock());
			}
			
			tailBlock_++;
			tailIndex_ = 0;
		}
		
		new (tailPtr()) T{std::forward<Args>(args)...};
		++tailIndex_;
		++count_;
	}

	template <typename... Args>
	void emplaceFront(Args&&... args) {
		if (headIndex_ == 0) {
			if (headBlock_ == 0) {
				blocks_.prepend(newBlock());
				++tailBlock_;
			}
			else {
				--headBlock_;
			}
			
			headIndex_ = blockCapacity;
		}

		--headIndex_;
		new (headPtr()) T{std::forward<Args>(args)...};
		++count_;
	}

	void append(const T& t) {
		emplaceBack(t);
	}
	
	void prepend(const T& t) {
		emplaceFront(t);
	}


	// -- removing elements
	void popFront() {
		assert(count() > 0);

		if (! canSkipElementDestructor) {
			headPtr()->~T();
		}

		++headIndex_;

		if (headIndex_ == blockCapacity) {
			// Strategy: keep max. 1 block before head if it was previously created.
			// Once we get to 2 empty blocks before head, then remove the front block.

			if (headBlock_ == 0) {
				++headBlock_;
			}
			else if (headBlock_ == 1) {
				allocator_.free(blocks_.front());
				blocks_.popFront();
				tailBlock_--;
			}

			headIndex_ = 0;
		}

		--count_;
	}

	
	void popBack() {
		assert(count() > 0);

		if (tailIndex_ == 0) {
			// Strategy: keep max. 1 block after tail if it was previously created.
			// Once we get to 2 empty blocks after tail, then remove the back block.
			
			auto lastBlockIndex = blocks_.count() - 1;

			if (tailBlock_ == lastBlockIndex - 1) {
				allocator_.free(blocks_.back());
				blocks_.popBack();
			}
			
			--tailBlock_;
			tailIndex_ = blockCapacity;
		}

		--tailIndex_;
		
		if (! canSkipElementDestructor) {
			tailPtr()->~T();
		}

		--count_;
	}


	void clear() {
		// destruct elements if needed
		if (count() && ! canSkipElementDestructor) {
			while (count_--) {
				headPtr()->~T();
				++headIndex_;

				if (headIndex_ == blockCapacity) {
					++headBlock_;
					headIndex_ = 0;
				}
			}
		}

		// free all blocks save 1
		if (blocks_.count() > 1) {
			auto allBlocks = blocks_.all();
			allBlocks.next(); // skip first
			
			while (allBlocks.next()) {
				allocator_.free(allBlocks.current());
			}
			
			blocks_.resize(1);
		}

		headBlock_ = 0; tailBlock_ = 0;
		headIndex_ = 0; tailIndex_ = 0;
		count_ = 0;
	}
	

	// -- observers
	static constexpr size32 elementSizeBytes() { return sizeof32<T>(); }
	
	uint count() const { return count_; }
	bool empty() const { return count_ == 0; }

	const T& front() const {
		assert(count_ > 0);
		return *headPtr();
	}

	T& front() {
		assert(count_ > 0);
		return *headPtr();
	}
	
	const T& back() const {
		assert(count_ > 0);
		return (__builtin_expect(tailIndex_ > 0, 1)) ? *(tailPtr() - 1) : *(blocks_[tailBlock_ - 1] + blockCapacity - 1);
	}

	T& back() {
		assert(count_ > 0);
		return (__builtin_expect(tailIndex_ > 0, 1)) ? *(tailPtr() - 1) : *(blocks_[tailBlock_ - 1] + blockCapacity - 1);
	}
};


} // ns container


// -- export to sd namespace
using container::Deque;


} // ns stardazed

#endif
