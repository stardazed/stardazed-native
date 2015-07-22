// ------------------------------------------------------------------
// scene::Behaviour - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_BEHAVIOUR_H
#define SD_SCENE_BEHAVIOUR_H

#include "system/Config.hpp"
#include "container/Array.hpp"
#include "runtime/FrameContext.hpp"

#include <functional>

namespace stardazed {
namespace scene {


struct Entity;
class Scene;


struct BehaviourX {
	virtual ~BehaviourX() = default;
	virtual void update(Entity&, Scene&, runtime::FrameContext&) = 0;
};


class PluggableBehaviour : public BehaviourX {
	using SimpleBehaviourHandler = std::function<void(Entity&, Scene&, runtime::FrameContext&)>;

	SimpleBehaviourHandler updateFunc_;

public:
	PluggableBehaviour();

	void update(Entity&, Scene&, runtime::FrameContext&) override;
	void setUpdateHandler(SimpleBehaviourHandler);
};



class Behaviour {
	struct ConstructInPlace {};

	struct BehaviourConcept {
		virtual ~BehaviourConcept() = default;

		virtual void run_(Time t) = 0;
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

		void run_(Time t) final { t_.run(t); }
	};

	memory::GrowableArena objects_;
	container::Array<uint32> offsets_;
	uint32 count_;

public:
	Behaviour(memory::Allocator& allocator, uint32 initialCapacity, uint32 averageElementSize = 16)
	: objects_(allocator, initialCapacity * averageElementSize)
	, offsets_(allocator, initialCapacity)
	, count_(0)
	{}
	
	~Behaviour() {
		for (auto b = 0u; b < count_; ++b)
			reinterpret_cast<BehaviourConcept*>(base_ + offsets_[b])->~BehaviourConcept();
	}

	struct Handle { uint32 ref; };

	template <typename T>
	Handle append(const T& t) {
		auto space = reinterpret_cast<BehaviourInstance<T>*>(cur_);
		cur_ += sizeof(BehaviourInstance<T>);
		new (space) BehaviourInstance<T>{t};
		offsets_[count_] = (uint8*)space - base_;
		
		return { count_++ };
	}
	
	template <typename T, typename... Args>
	Handle emplace(Args&&... args) {
		auto space = reinterpret_cast<BehaviourInstance<T>*>(cur_);
		cur_ += sizeof(BehaviourInstance<T>);
		new (space) BehaviourInstance<T>(ConstructInPlace{}, std::forward<Args>(args)...);
		offsets_[count_] = (uint8*)space - base_;
		
		return { count_++ };
	}
	
	void run(Handle h, Time t) {
		reinterpret_cast<BehaviourConcept*>(base_ + offsets_[h.ref])->run_(t);
	}
	
	void runAll(Time t) {
		for (auto b = 0u; b < count_; ++b)
			reinterpret_cast<BehaviourConcept*>(base_ + offsets_[b])->run_(t);
	}
};


	
} // ns scene
} // ns stardazed

#endif
