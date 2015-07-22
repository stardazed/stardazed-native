// ------------------------------------------------------------------
// memory::Block - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_BLOCK_H
#define SD_MEMORY_BLOCK_H

#include "system/Config.hpp"
#include "memory/Allocator.hpp"
#include "math/Algorithm.hpp"

namespace stardazed {
namespace memory {


class FixedSizeArena {
	Allocator& allocator_;
	uint8* base_;
	uint8* cur_;
	uint32 capacity_;

public:
	FixedSizeArena(Allocator& allocator, uint32 capacity);
	~FixedSizeArena();

	void* alloc(uint32 sizeBytes, uint32 alignment = 8);
	
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



class GrowableArena {
	Allocator& allocator_;
	uint8* base_;
	uint8* cur_;
	uint32 capacity_;

public:
	GrowableArena(Allocator& allocator, uint32 initialCapacity);
	~GrowableArena();

	void* alloc(uint32 sizeBytes, uint32 alignment = 8);
	
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
