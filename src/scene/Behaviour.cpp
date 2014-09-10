// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Behaviour.hpp"
#include "scene/Entity.hpp"

namespace stardazed {
namespace scene {


PluggableBehaviour::PluggableBehaviour() {
	static auto noAction = [](Entity&, Scene&, runtime::Client&){};
	
	updateFunc_ = noAction;
}


void PluggableBehaviour::update(Entity& entity, Scene& scene, runtime::Client& client) {
	updateFunc_(entity, scene, client);
}


void PluggableBehaviour::setUpdateHandler(SimpleBehaviourHandler handler) {
	updateFunc_ = handler;
}

	
} // ns scene
} // ns stardazed
