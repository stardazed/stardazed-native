// ------------------------------------------------------------------
// scene::Behaviour.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Behaviour.hpp"
#include "scene/Entity.hpp"

namespace stardazed {
namespace scene {


PluggableBehaviour::PluggableBehaviour() {
	static auto noAction = [](Entity, Scene&, Time dt){};
	updateFunc_ = noAction;
}


PluggableBehaviour::PluggableBehaviour(const SimpleBehaviourHandler& fn) {
	updateFunc_ = fn;
}
	
	
void PluggableBehaviour::update(Entity entity, Scene& scene, Time dt) {
	updateFunc_(entity, scene, dt);
}


void PluggableBehaviour::setUpdateHandler(SimpleBehaviourHandler handler) {
	updateFunc_ = handler;
}


} // ns scene
} // ns stardazed
