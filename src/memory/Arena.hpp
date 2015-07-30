// ------------------------------------------------------------------
// memory::Arena - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_ARENA_H
#define SD_MEMORY_ARENA_H

#include "system/Config.hpp"
#include "memory/Allocator.hpp"
#include "container/Array.hpp"
#include "math/Algorithm.hpp"

namespace stardazed {
namespace memory {


class ArenaAllocator : Allocator {
	Allocator& blockAlloc_;
	uint32 blockSize_, offsetInCurBlock_;
	Array<uint8*> blockList_;
	uint8* curBlock_;
	
	void nextBlock() {
		curBlock_ = static_cast<uint8*>(blockAlloc_.alloc(blockSize_));
		if (! blockAlloc_.allocZeroesMemory())
			memset(curBlock_, 0, blockSize_);

		blockList_.emplaceBack(curBlock_);
		offsetInCurBlock_ = 0;
	}

public:
	ArenaAllocator(Allocator& blockAlloc, uint32 blockSize = 4_MB)
	: blockAlloc_(blockAlloc)
	, blockSize_(math::alignUp(blockSize, 32))
	, offsetInCurBlock_(0)
	, blockList_(blockAlloc_, 16)
	{
		nextBlock();
	}


	~ArenaAllocator() {
		for (uint b = 0; b < blockList_.count(); ++b) {
			blockAlloc_.free(blockList_[b]);
		}
	}


	void* alloc(uint64 sizeBytes) final {
		// ArenaAllocators are not meant for large allocations, but rather
		// many small ones. We simply bail on too large an allocation.
		if (__builtin_expect(sizeBytes > blockSize_, 0)) {
			assert(! "Tried to allocate a block > blockSize");
			return nullptr;
		}
		
		if (__builtin_expect(offsetInCurBlock_ + sizeBytes > blockSize_, 0)) {
			nextBlock();
		}

		auto ptr = curBlock_ + offsetInCurBlock_;
		offsetInCurBlock_ += math::alignUp(sizeBytes, guaranteedAlignment());
		return ptr;
	}


	void free(void* /* ptr */) final {
		// Arena doesn't free individual objects
	}

	
	bool allocZeroesMemory() const final { return true; }
	uint guaranteedAlignment() const final { return 8; }
};


} // ns memory
} // ns stardazed

#endif
