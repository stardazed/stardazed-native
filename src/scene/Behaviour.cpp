// ------------------------------------------------------------------
// scene::Behaviour.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Behaviour.hpp"
#include "scene/Entity.hpp"

namespace stardazed {
namespace scene {


PluggableBehaviour::PluggableBehaviour() {
	static auto noAction = [](Entity&, Scene&, runtime::FrameContext&){};
	
	updateFunc_ = noAction;
}


void PluggableBehaviour::update(Entity& entity, Scene& scene, runtime::FrameContext& frame) {
	updateFunc_(entity, scene, frame);
}


void PluggableBehaviour::setUpdateHandler(SimpleBehaviourHandler handler) {
	updateFunc_ = handler;
}

	
} // ns scene
} // ns stardazed
