// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {
		
		
Node* Scene::makeNode() {
	return nodePool_.emplace();
}


Camera* Scene::makeCamera() {
	auto cameraNode = makeNode();
	return cameraPool_.emplace(*cameraNode);
}

		
} // ns scene
} // ns stardazed
