// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {


Scene::Scene()
: entities_()
, transform_()
{}


Entity Scene::makeEntity() {
	auto ent = entities_.create();
	transform_.assign(ent);
	return ent;
}


Entity Scene::makeEntity(const TransformDescriptor& td) {
	auto ent = entities_.create();
	transform_.assign(ent, td);
	return ent;
}


Entity Scene::makeEntity(const math::Vec3& pos, const math::Quat& rot, const math::Vec3& scale) {
	auto ent = entities_.create();
	transform_.assign(ent, { pos, rot, scale });
	return ent;
}


/*
Scene::Scene() {
	// FIXME: make this settable by client
	entityPool_.reserve(512);
	renderablePool_.reserve(512);
	meshRendererPool_.reserve(64);
	behaviourPool_.reserve(64);
}


Entity* Scene::makeEntity() {
	entityPool_.emplace_back();
	return &entityPool_.back();
}


//Light* Scene::makeLight(const scene::LightDescriptor& descriptor) {
//	auto lightEntity = makeEntity();
//	return X;
//}


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


MeshRenderer* Scene::makeMeshRenderer(Renderable& renderable) {
	meshRendererPool_.emplace_back();
	auto& meshRenderer = meshRendererPool_.back();
	meshRenderer.renderable = &renderable;
	return &meshRenderer;
}
*/
		
} // ns scene
} // ns stardazed
