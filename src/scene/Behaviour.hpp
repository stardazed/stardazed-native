// ------------------------------------------------------------------
// scene::Behaviour - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_BEHAVIOUR_H
#define SD_SCENE_BEHAVIOUR_H

#include "system/Config.hpp"
#include "runtime/FrameContext.hpp"

#include <functional>

namespace stardazed {
namespace scene {


struct Entity;
class Scene;


struct Behaviour {
	virtual ~Behaviour() = default;
	virtual void update(Entity&, Scene&, runtime::FrameContext&) = 0;
};


class PluggableBehaviour : public Behaviour {
	using SimpleBehaviourHandler = std::function<void(Entity&, Scene&, runtime::FrameContext&)>;

	SimpleBehaviourHandler updateFunc_;

public:
	PluggableBehaviour();

	void update(Entity&, Scene&, runtime::FrameContext&) override;
	void setUpdateHandler(SimpleBehaviourHandler);
};
	
	
} // ns scene
} // ns stardazed

#endif
