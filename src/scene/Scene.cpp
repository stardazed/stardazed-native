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
		
Entity* Scene::makeEntity(EntityType type) {
	auto entity = entityPool_.emplace(type);

	auto treeNode = entityTree_.makeNode(*entity);
	entityTree_.root().append(treeNode);

	return entity;
}


Camera* Scene::makeCamera() {
	auto cameraEntity = makeEntity(EntityType::Camera);
	return cameraPool_.emplace(*cameraEntity);
}

		
} // ns scene
} // ns stardazed
