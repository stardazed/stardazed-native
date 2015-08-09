// ------------------------------------------------------------------
// scene::Behaviour - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_BEHAVIOUR_H
#define SD_SCENE_BEHAVIOUR_H

#include "system/Config.hpp"
#include "container/Array.hpp"
#include "container/HashMap.hpp"
#include "memory/Arena.hpp"
#include "runtime/FrameContext.hpp"
#include "scene/Entity.hpp"

#include <functional>

namespace stardazed {
namespace scene {


struct Entity;
class Scene;


class PluggableBehaviour {
	using SimpleBehaviourHandler = std::function<void(Entity, Scene&, runtime::FrameContext&)>;

	SimpleBehaviourHandler updateFunc_;

public:
	PluggableBehaviour();
	PluggableBehaviour(const SimpleBehaviourHandler&);

	void update(Entity, Scene&, runtime::FrameContext&);
	void setUpdateHandler(SimpleBehaviourHandler);
};



class Behaviour {
public:
	using Instance = scene::Instance<Behaviour>;

private:
	struct ConstructInPlace {};

	struct BehaviourConcept {
		virtual ~BehaviourConcept() = default;
		virtual void update(Entity, Scene&, runtime::FrameContext&) = 0;
	};
	
	template <typename T>
	struct BehaviourInstance : BehaviourConcept {
		T t_;

		BehaviourInstance(const T& t)
		: t_(t) {}

		BehaviourInstance(T&& t)
		: t_(std::move(t)) {}

		template <typename... Args>
		BehaviourInstance(ConstructInPlace, Args&&... args)
		: t_(std::forward<Args>(args)...) {}

		void update(Entity ent, Scene& scene, runtime::FrameContext& fc) final {
			t_.render(ent, scene, fc);
		}
	};

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
		auto space = static_cast<BehaviourInstance<B>*>(arena_.alloc(sizeof(BehaviourInstance<B>)));
		new (space) BehaviourInstance<B>(beh);
		items_.emplaceBack(space);
		return { count_++ };
	}

	template <typename B, typename... Args>
	Instance emplace(Args&&... args) {
		auto space = static_cast<BehaviourInstance<B>*>(arena_.alloc(sizeof(BehaviourInstance<B>)));
		new (space) BehaviourInstance<B>(ConstructInPlace{}, std::forward<Args>(args)...);
		items_.emplaceBack(space);
		return { count_++ };
	}

	void linkEntityToInstance(Entity ent, Instance h) {
		assert(h.ref < count_);
		auto behaviour = items_[h.ref];
		entityMap_.insert(ent, behaviour);
	}
	
	
	void updateAll(Time t) {
		for (auto b = 0u; b < count_; ++b)
			;
	}
};


	
} // ns scene
} // ns stardazed

#endif
