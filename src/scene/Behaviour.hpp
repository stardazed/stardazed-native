// ------------------------------------------------------------------
// scene::Behaviour - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_BEHAVIOUR_H
#define SD_SCENE_BEHAVIOUR_H

#include "runtime/Client.hpp"

#include <functional>

// Let's prototype some object event/behaviour mechanics

namespace stardazed {
namespace scene {


class Entity;
class Scene;


class Behaviour {
public:
	virtual ~Behaviour() = default;
	using SimpleBehaviourHandler = std::function<void(Entity&, Scene&, runtime::Client&)>;
	
	virtual void update(Entity&, Scene&, runtime::Client&) = 0;
};


class PluggableBehaviour : public Behaviour {
	SimpleBehaviourHandler updateFunc_;

public:
	PluggableBehaviour();

	void update(Entity&, Scene&, runtime::Client&) override;
	void setUpdateHandler(SimpleBehaviourHandler);
};
	
	
} // ns scene
} // ns stardazed

#endif
