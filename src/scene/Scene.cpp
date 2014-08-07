// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {


Scene::Scene() {

}


// ---- factory methods
		
Node* Scene::makeNode(NodeType type) {
	auto sceneNode = nodePool_.emplace(type);

	auto treeNode = nodeTree_.makeNode(*sceneNode);
	nodeTree_.root().append(treeNode);

	return sceneNode;
}


Camera* Scene::makeCamera() {
	auto cameraNode = makeNode(NodeType::Camera);
	return cameraPool_.emplace(*cameraNode);
}

		
} // ns scene
} // ns stardazed
