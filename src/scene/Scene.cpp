// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {


Scene::Scene() {

}
		
		
Node* Scene::makeNode(NodeType type) {
	return nodePool_.emplace(type);
}


Camera* Scene::makeCamera() {
	auto cameraNode = makeNode(NodeType::Camera);
	return cameraPool_.emplace(*cameraNode);
}

		
} // ns scene
} // ns stardazed
