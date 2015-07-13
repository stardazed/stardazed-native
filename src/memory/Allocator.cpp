// ------------------------------------------------------------------
// memory::Allocator.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "memory/Allocator.hpp"

namespace stardazed {
namespace memory {


SystemAllocator& SystemAllocator::sharedInstance() {
	static SystemAllocator sha_s;
	return sha_s;
}


StackAllocator& StackAllocator::sharedInstance() {
	static StackAllocator ssa_s;
	return ssa_s;
}

	
} // ns memory
} // ns stardazed
