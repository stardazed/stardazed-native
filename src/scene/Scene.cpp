// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {


Scene::Scene() {
	// FIXME: make this settable by client
	lightPool_.reserve(128);
	entityPool_.reserve(512);
}


Entity* Scene::makeEntity(EntityType type) {
	entityPool_.emplace_back(type);
	auto entity = &entityPool_.back();

	return entity;
}


Light* Scene::makeLight(const render::LightDescriptor& descriptor) {
	auto lightEntity = makeEntity(EntityType::Light);
	lightPool_.emplace_back(*lightEntity, descriptor);
	return &lightPool_.back();
}


Camera* Scene::makeCamera(uint32 viewPortWidth, uint32 viewPortHeight) {
	auto cameraEntity = makeEntity(EntityType::Camera);
	camera_ = std::make_unique<Camera>(*cameraEntity, viewPortWidth, viewPortHeight);
	return camera_.get();
}


physics::RigidBody* Scene::makeRigidBody(Entity& entity, physics::Mass mass) {
	auto rigidBody = physicsCtx_.makeRigidBody(entity.transform, mass);
	entity.rigidBody = rigidBody;
	return rigidBody;
}

		
} // ns scene
} // ns stardazed
