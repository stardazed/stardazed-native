// ------------------------------------------------------------------
// scene::Behaviour - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_BEHAVIOUR_H
#define SD_SCENE_BEHAVIOUR_H

#include "system/Config.hpp"
#include "system/Time.hpp"
#include "container/Array.hpp"
#include "container/HashMap.hpp"
#include "memory/Arena.hpp"
#include "scene/Entity.hpp"

#include <functional>

namespace stardazed {
namespace scene {


struct Entity;
class Scene;


struct BehaviourConcept {
	virtual ~BehaviourConcept() = default;
	virtual void update(Entity, Scene&, Time) = 0;
};


class PluggableBehaviour : public BehaviourConcept {
	using SimpleBehaviourHandler = std::function<void(Entity, Scene&, Time)>;

	SimpleBehaviourHandler updateFunc_;

public:
	PluggableBehaviour();
	PluggableBehaviour(const SimpleBehaviourHandler&);

	void update(Entity, Scene&, Time) final;
	void setUpdateHandler(SimpleBehaviourHandler);
};



class Behaviour {
public:
	using Instance = scene::Instance<Behaviour>;

private:
	memory::ArenaAllocator arena_;
	Array<BehaviourConcept*> items_;
	HashMap<Entity, BehaviourConcept*> entityMap_;
	uint32 count_;

public:
	Behaviour(memory::Allocator& allocator)
	: arena_(allocator)
	, items_(allocator, 128)
	, entityMap_(allocator)
	, count_(0)
	{}
	
	~Behaviour() {
		for (auto b = 0u; b < count_; ++b)
			items_[b]->~BehaviourConcept();
	}
	
	template <typename B, typename... Args>
	Instance append(const B& beh) {
		auto space = static_cast<B*>(arena_.alloc(sizeof(B)));
		new (space) B(beh);
		items_.emplaceBack(space);
		return { count_++ };
	}

	template <typename B, typename... Args>
	Instance emplace(Args&&... args) {
		auto space = static_cast<B*>(arena_.alloc(sizeof(B)));
		new (space) B(std::forward<Args>(args)...);
		items_.emplaceBack(space);
		return { count_++ };
	}


	void linkEntityToBehaviour(Entity ent, Instance h) {
		assert(h.ref < count_);
		auto behaviour = items_[h.ref];
		entityMap_.insert(ent, behaviour);
	}
	
	
	void updateAll(Scene& scene, Time dt) {
		auto allLinkedBehaviours = entityMap_.all();
		while (allLinkedBehaviours.next()) {
			auto entBeh = allLinkedBehaviours.current();
			entBeh.val->update(entBeh.key, scene, dt);
		}
	}
};


	
} // ns scene
} // ns stardazed

#endif
