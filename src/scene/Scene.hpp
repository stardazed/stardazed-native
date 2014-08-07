// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENE_H
#define SD_SCENE_SCENE_H

#include "container/ObjectPool.hpp"
#include "container/RefTree.hpp"
#include "scene/Node.hpp"
#include "scene/Camera.hpp"
#include "scene/Behaviour.hpp"

#include <vector>
#include <memory>

namespace stardazed {
namespace scene {


class Scene {
	container::ObjectPool<Camera, 32> cameraPool_;
	container::ObjectPoolChain<Node, 512> nodePool_;

	std::vector<std::unique_ptr<Behaviour>> behaviours_; // polymorphic
	
	container::RefTree<Node> nodeTree_;

public:
	Scene();

	// -- factory methods

	Node* makeNode(NodeType type = NodeType::Generic);
	Camera* makeCamera();
	
	template <typename B, typename... Args> // B : public Behaviour
	B* makeBehaviour(Args... args) {
		auto b = new B(std::forward<Args>(args)...);
		behaviours_.emplace_back(b);
		return b;
	}


	// -- scene::Node access
	
	auto allNodesBegin() { return nodePool_.begin(); }
	auto allNodesEnd() { return nodePool_.end(); }
	
	auto rootNodesBegin() { return nodeTree_.root().begin(); };
	auto rootNodesEnd() { return nodeTree_.root().end(); }
	
	auto camerasBegin() { return cameraPool_.begin(); }
	auto camerasEnd() { return cameraPool_.end(); }
};


} // ns scene
} // ns stardazed

#endif
