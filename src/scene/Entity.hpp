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

namespace stardazed {
namespace scene {


struct Entity {
	static constexpr const uint indexBits = 24;
	static constexpr const uint generationBits = 8;

	static_assert(indexBits + generationBits <= 32, "Only 32 bits avail for index and gen");
	
	static constexpr const uint indexMask = (1 << indexBits) - 1;
	static constexpr const uint generationMask = (1 << generationBits) - 1;
	
	uint id;
	
	constexpr uint index() const { return id & indexMask; }
	constexpr uint generation() const { return (id >> indexBits) & generationMask; }

private:
	friend class EntityManager;
	
	constexpr Entity(uint index, uint gen)
	: id{ (gen << indexBits) | index }
	{
		assert(index <= indexMask);
		assert(gen <= generationMask);
	}
};


template <typename T>
class CircularArray {
	std::vector<T> data_;
	uint head_, tail_;

	CircularArray(uint capacity)
	: data_(capacity)
	{
		tail_ = head_ = capacity / 2;
	}
	
	uint count() {
		if (head_ < tail_)
			return tail_ - head_;
		return (tail_ + data_.size()) - head_;
	}
	
	void pushBack(T t) {
		data_[tail_] = t;
		++tail_;
	}
};


class EntityManager {
	std::vector<uint8> generation_;
	
public:
	EntityManager() {
		generation_.reserve(2048);
		generation_.push_back(0);	// entity id 0 is reserved
	}
		
	Entity create() {
		uint index = (uint)generation_.size();
		generation_.push_back(0);
		return { index, 0 };
	}
	
	bool alive(Entity ent) {
		return ent.generation() == generation_[ent.index()];
	}
	
	void destroy(Entity ent) {
		
	}
};


} // ns scene
} // ns stardazed

#endif
