// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {


Scene::Scene() {
	// FIXME: make this settable by client
	cameraPool_.reserve(32);
	lightPool_.reserve(128);
	entityPool_.reserve(512);
}


Entity* Scene::makeEntity(EntityType type) {
	entityPool_.emplace_back(type);
	auto entity = &entityPool_.back();

	auto treeNode = entityTree_.makeNode(*entity);
	entityTree_.root().append(treeNode);

	return entity;
}


Light* Scene::makeLight(const render::LightDescriptor& descriptor) {
	makeEntity(EntityType::Light);
	auto& lightEntity = entityPool_.back();
	lightPool_.emplace_back(lightEntity, descriptor);
	return &lightPool_.back();
}


Camera* Scene::makeCamera() {
	makeEntity(EntityType::Camera);
	auto& cameraEntity = entityPool_.back();
	cameraPool_.emplace_back(cameraEntity);
	return &cameraPool_.back();
}

		
} // ns scene
} // ns stardazed
