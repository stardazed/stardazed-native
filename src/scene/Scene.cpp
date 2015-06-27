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
	renderablePool_.reserve(512);
	materialPool_.reserve(64);
	behaviourPool_.reserve(64);
}


Entity* Scene::makeEntity() {
	entityPool_.emplace_back();
	return &entityPool_.back();
}


// FIXME: Light and Camera should be converted into Components

Light* Scene::makeLight(const render::LightDescriptor& descriptor) {
	auto lightEntity = makeEntity();
	lightPool_.emplace_back(*lightEntity, descriptor);
	return &lightPool_.back();
}


Camera* Scene::makeCamera(uint32 viewPortWidth, uint32 viewPortHeight) {
	auto cameraEntity = makeEntity();
	camera_ = std::make_unique<Camera>(*cameraEntity, viewPortWidth, viewPortHeight);
	return camera_.get();
}


physics::RigidBody* Scene::makeRigidBody(Entity& entity, float mass, float angInertia) {
	auto rigidBody = physicsCtx_.makeRigidBody(entity.transform, mass, angInertia);
	entity.rigidBody = rigidBody;
	if (entity.collider)
		entity.collider->linkToRigidBody(*rigidBody);
	return rigidBody;
}


Renderable* Scene::makeRenderable() {
	renderablePool_.emplace_back();
	return &renderablePool_.back();
}

		
} // ns scene
} // ns stardazed
