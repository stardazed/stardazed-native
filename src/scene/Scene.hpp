// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENE_H
#define SD_SCENE_SCENE_H

#include "container/ObjectPool.hpp"
#include "scene/Node.hpp"
#include "scene/Camera.hpp"

namespace stardazed {
namespace scene {


class Scene {
	container::ObjectPool<Camera, 32> cameraPool_;
	container::ObjectPoolChain<Node, 500> nodePool_;

public:
	Node* makeNode();
	Camera* makeCamera();
};


} // ns scene
} // ns stardazed

#endif
