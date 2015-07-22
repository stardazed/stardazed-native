// ------------------------------------------------------------------
// container::ArenaSet - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_ARENASET_H
#define SD_CONTAINER_ARENASET_H

#include "system/Config.hpp"
#include "memory/Array.hpp"

namespace stardazed {
namespace container {


class ArenaSet {
	struct Block {
		void* data;
		Block* next;
	};

	memory::Allocator& allocator_;
	Array<Block> freeBlocks_;

public:
	ArenaSet(memory::Allocator&);

	void* alloc(uint32 sizeBytes, int alignment = 8);
	
	template <typename T>
	T* alloc() {
		return alloc(sizeof(T), alignof(T));
	}
	
	
};


} // ns container
} // ns stardazed

#endif
