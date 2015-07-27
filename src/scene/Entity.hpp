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
#include "container/RingBuffer.hpp"

namespace stardazed {
namespace scene {


struct Entity {
	static constexpr uint indexBits = 24;
	static constexpr uint generationBits = 8;

	static_assert(indexBits + generationBits <= 32, "Only 32 bits avail for index and gen");
	
	static constexpr uint indexMask = (1 << indexBits) - 1;
	static constexpr uint generationMask = (1 << generationBits) - 1;
	
	uint id;
	
	constexpr uint index() const { return id & indexMask; }
	constexpr uint generation() const { return (id >> indexBits) & generationMask; }

	constexpr bool operator ==(Entity other) { return id == other.id; }
	constexpr bool operator !=(Entity other) { return id != other.id; }

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
	container::RingBuffer<uint> freedIndices_;
	
public:
	EntityManager()
	: generation_(memory::SystemAllocator::sharedInstance(), 2048)
	, freedIndices_(memory::SystemAllocator::sharedInstance(), 1024)
	{
		generation_.append(0);	// entity id 0 is reserved
	}
		
	Entity create() {
		uint index;
		
		if (freedIndices_.full()) {
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
