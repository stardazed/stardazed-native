// ------------------------------------------------------------------
// scene::Entity - stardazed
// (c) 2015 by Arthur Langereis
//
// Based on (code in) an article by Bitsquid
// http://bitsquid.blogspot.se/2014/08/building-data-oriented-entity-system.html
// ------------------------------------------------------------------

#ifndef SD_SCENE_ENTITY_H
#define SD_SCENE_ENTITY_H

#include "system/Config.hpp"
#include "container/Array.hpp"
#include "container/Deque.hpp"

namespace stardazed {
namespace scene {


struct Handle {
	uint ref;

	constexpr bool operator ==(Handle other) const { return ref == other.ref; }
	constexpr bool operator !=(Handle other) const { return ref != other.ref; }
};


struct Entity {
	uint id = 0;
	
	static constexpr uint indexBits = 24;
	static constexpr uint generationBits = 8;

	static_assert(indexBits + generationBits <= sizeof(id) * 8, "Index and gen bits won't fit in the id");
	
	static constexpr uint indexMask = (1 << indexBits) - 1;
	static constexpr uint generationMask = (1 << generationBits) - 1;
	
	constexpr uint index() const { return id & indexMask; }
	constexpr uint generation() const { return (id >> indexBits) & generationMask; }

	constexpr bool operator ==(Entity other) const { return id == other.id; }
	constexpr bool operator !=(Entity other) const { return id != other.id; }
	
	constexpr Entity() = default;

private:
	friend class EntityManager;
	
	constexpr Entity(uint index, uint gen)
	: id{ (gen << indexBits) | index }
	{
		assert(index <= indexMask);
		assert(gen <= generationMask);
	}
};


class EntityManager {
	container::Array<uint8> generation_;
	container::Deque<uint> freedIndices_;
	
	static constexpr uint minFreedBuildup = 1024;
	
public:
	EntityManager()
	: generation_(memory::SystemAllocator::sharedInstance(), 2048)
	, freedIndices_(memory::SystemAllocator::sharedInstance(), 1024)
	{
		generation_.append(0);	// entity id 0 is reserved
	}
		
	Entity create() {
		uint index;
		
		if (freedIndices_.count() >= minFreedBuildup) {
			index = freedIndices_.front();
			freedIndices_.popFront();
		}
		else {
			index = generation_.count();
			generation_.append(0);
		}

		return { index, generation_[index] };
	}
	
	bool alive(Entity ent) const {
		return ent.generation() == generation_[ent.index()];
	}
	
	void destroy(Entity ent) {
		auto index = ent.index();
		generation_[index]++;
		freedIndices_.append(index);
	}
};


} // ns scene
} // ns stardazed

#endif
