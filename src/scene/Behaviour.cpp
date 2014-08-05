// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Behaviour.hpp"
#include "scene/Node.hpp"

namespace stardazed {
namespace scene {


PluggableBehaviour::PluggableBehaviour() {
	static auto noAction = [](Node&){};
	
	updateFunc_ = noAction;
}


void PluggableBehaviour::update(Node& node) {
	updateFunc_(node);
}


void PluggableBehaviour::setUpdateHandler(SimpleBehaviourHandler handler) {
	updateFunc_ = handler;
}

	
} // ns scene
} // ns stardazed
