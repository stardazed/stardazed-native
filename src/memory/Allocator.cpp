// ------------------------------------------------------------------
// memory::Allocator.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "memory/Allocator.hpp"

namespace stardazed {
namespace memory {


SystemAllocator& SystemAllocator::sharedInstance() {
	static SystemAllocator sa_s;
	return sa_s;
}

	
} // ns memory
} // ns stardazed
