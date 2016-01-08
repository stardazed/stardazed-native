// ------------------------------------------------------------------
// memory::Block - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "memory/Block.hpp"

namespace stardazed {
namespace memory {


//  ___ _            _ ___ _          _
// | __(_)_ _____ __| / __(_)______  /_\  _ _ ___ _ _  __ _
// | _|| \ \ / -_) _` \__ \ |_ / -_)/ _ \| '_/ -_) ' \/ _` |
// |_| |_/_\_\___\__,_|___/_/__\___/_/ \_\_| \___|_||_\__,_|
//

FixedSizeArena::FixedSizeArena(Allocator& allocator, uint32 capacity)
: allocator_(allocator)
{
	assert(capacity > 0);

	base_ = static_cast<uint8*>(allocator_.alloc(capacity));
	cur_ = base_;
	capacity_ = capacity;
}


FixedSizeArena::~FixedSizeArena() {
	allocator_.free(base_);
}


void* FixedSizeArena::alloc(uint32 sizeBytes, uint32 alignment) {
	auto usedBytes = uint32(cur_ - base_);
	auto alignmentPadding = math::alignUp(usedBytes, alignment) - usedBytes;
	auto effectiveSizeBytes = alignmentPadding + sizeBytes;

	assert(usedBytes + effectiveSizeBytes <= capacity_);

	auto block = cur_ + alignmentPadding;
	cur_ += effectiveSizeBytes;
	return block;
}


//   ___                     _    _       _
//  / __|_ _ _____ __ ____ _| |__| |___  /_\  _ _ ___ _ _  __ _
// | (_ | '_/ _ \ V  V / _` | '_ \ / -_)/ _ \| '_/ -_) ' \/ _` |
//  \___|_| \___/\_/\_/\__,_|_.__/_\___/_/ \_\_| \___|_||_\__,_|
//

GrowableArena::GrowableArena(Allocator& allocator, uint32 initialCapacity)
: allocator_(allocator)
{
	assert(initialCapacity > 0);

	base_ = static_cast<uint8*>(allocator_.alloc(initialCapacity));
	cur_ = base_;
	capacity_ = initialCapacity;
}


GrowableArena::~GrowableArena() {
	allocator_.free(base_);
}


void* GrowableArena::alloc(uint32 sizeBytes, uint32 alignment) {
	auto usedBytes = uint32(cur_ - base_);
	auto alignmentPadding = math::alignUp(usedBytes, alignment) - usedBytes;
	auto effectiveSizeBytes = alignmentPadding + sizeBytes;

	if (__builtin_expect(usedBytes + effectiveSizeBytes > capacity_, 0)) {
		auto newCapacity = capacity_ * growthFactor_s;
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


} // ns memory
} // ns stardazed
