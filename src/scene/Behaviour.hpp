// ------------------------------------------------------------------
// scene::Behaviour - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_BEHAVIOUR_H
#define SD_SCENE_BEHAVIOUR_H

#include "system/Config.hpp"
#include "runtime/FrameContext.hpp"

#include <functional>

// Let's prototype some object event/behaviour mechanics

namespace stardazed {
namespace scene {


struct Entity;
class Scene;


struct Behaviour {
	virtual ~Behaviour() = default;

	using SimpleBehaviourHandler = std::function<void(Entity&, Scene&, runtime::FrameContext&)>;
	
	virtual void update(Entity&, Scene&, runtime::FrameContext&) = 0;
};


class PluggableBehaviour : public Behaviour {
	SimpleBehaviourHandler updateFunc_;

public:
	PluggableBehaviour();

	void update(Entity&, Scene&, runtime::FrameContext&) override;
	void setUpdateHandler(SimpleBehaviourHandler);
};
	
	
} // ns scene
} // ns stardazed

#endif
