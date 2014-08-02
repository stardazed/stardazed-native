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

namespace stardazed {
namespace scene {


class Scene {
	container::ObjectPool<Camera, 32> cameraPool_;
	container::ObjectPoolChain<Node, 500> nodePool_;
	
	container::RefTree<Node> nodeTree_;

public:
	Scene();

	Node* makeNode(NodeType type = NodeType::Generic);
	Camera* makeCamera();
};


} // ns scene
} // ns stardazed

#endif
