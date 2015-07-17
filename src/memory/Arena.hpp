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


class FixedSizeArena {
	size_t sizeBytes_;
	uint8* base_;
	uint8* cur_;
	
public:
	FixedSizeArena(void* base, size_t sizeBytes)
	: sizeBytes_(sizeBytes)
	, base_(static_cast<uint8*>(base))
	, cur_(base_)
	{
		assert(base);
		assert(sizeBytes > 0);
	}
	
	void* alloc(size_t sizeBytes) final {
		assert(freeBytes() > sizeBytes);

		auto next = cur_;
		cur_ += sizeBytes;
		return next;
	}
	
	void clear() {
		cur_ = base_;
	}

	// -- specific to fixed size arena
	size_t capacity() const { return sizeBytes_; }
	size_t usedBytes() const { return cur_ - base_; }
	size_t freeBytes() const { return capacity() - usedBytes(); }
	void* basePointer() const { return arena_; }
};


} // ns memory
} // ns stardazed

#endif
