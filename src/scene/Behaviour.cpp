// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Behaviour.hpp"
#include "scene/Entity.hpp"

namespace stardazed {
namespace scene {


PluggableBehaviour::PluggableBehaviour() {
	static auto noAction = [](Entity&){};
	
	updateFunc_ = noAction;
}


void PluggableBehaviour::update(Entity& entity) {
	updateFunc_(entity);
}


void PluggableBehaviour::setUpdateHandler(SimpleBehaviourHandler handler) {
	updateFunc_ = handler;
}

	
} // ns scene
} // ns stardazed
