// ------------------------------------------------------------------
// memory::Arena - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_ARENA_H
#define SD_MEMORY_ARENA_H

#include "system/Config.hpp"
#include "memory/Allocator.hpp"
#include "math/Algorithm.hpp"

namespace stardazed {
namespace memory {


class DynamicArena {
	Allocator& allocator_;
	uint8* base_;
	uint8* cur_;
	uint32 capacity_;

public:
	DynamicArena(Allocator& allocator, uint32 initialCapacity)
	: allocator_(allocator)
	{
		assert(initialCapacity > 0);

		base_ = static_cast<uint8*>(allocator_.alloc(initialCapacity));
		cur_ = base_;
		capacity_ = initialCapacity;
	}
	
	
	~DynamicArena() {
		allocator_.free(base_);
	}


	void* alloc(uint32 sizeBytes, uint32 alignment = 8) {
		auto usedBytes = uint32(cur_ - base_);
		auto alignmentPadding = math::alignUp(usedBytes, alignment) - usedBytes;
		auto effectiveSizeBytes = alignmentPadding + sizeBytes;

		if (__builtin_expect(usedBytes + sizeBytes > capacity_, 0)) {
			auto newCapacity = capacity_ * 2;
			if (sizeBytes >= capacity_)
				newCapacity += sizeBytes;

			auto newBase = static_cast<uint8*>(allocator_.alloc(newCapacity));
			assert(newBase);

			// Copy over the data to the new buffer and free the old one
			memcpy(newBase, base_, capacity_);
			allocator_.free(base_);

			base_ = newBase;
			cur_ = newBase + usedBytes;
			capacity_ = newCapacity;
		}

		auto block = cur_ + alignmentPadding;
		cur_ += effectiveSizeBytes;
		return block;
	}

	
	void clear() {
		cur_ = base_;
	}

	
	uint32 internalOffsetOf(void* block) {
		assert((uint8*)block > base_ && (uint8*)block < base_ + capacity_);
		
		return uint32((uint8*)block - base_);
	}

	
	void* internalPointerAtOffset(uint32 offsetBytes) {
		assert(offsetBytes < capacity_ - 1);
		return base_ + offsetBytes;
	}


	template <typename T>
	T* instanceAtOffset(uint32 offsetBytes) {
		assert(offsetBytes < capacity_ - 1);
		return reinterpret_cast<T*>(base_ + offsetBytes);
	}


	void* basePointer() const { return base_; }
};


} // ns memory
} // ns stardazed

#endif
