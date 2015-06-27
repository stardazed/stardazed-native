// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENE_H
#define SD_SCENE_SCENE_H

#include "system/Config.hpp"
#include "scene/Entity.hpp"
#include "scene/Light.hpp"
#include "scene/Camera.hpp"
#include "scene/Behaviour.hpp"
#include "scene/Material.hpp"
#include "physics/PhysicsContext.hpp"

#include <vector>
#include <memory>

namespace stardazed {
namespace scene {


class Scene {
	// -- storage
	std::unique_ptr<Camera> camera_;
	std::vector<Light> lightPool_;
	std::vector<Entity> entityPool_;
	std::vector<Renderable> renderablePool_;
	std::vector<std::unique_ptr<Material>> materialPool_; // polymorphic
	std::vector<std::unique_ptr<Behaviour>> behaviourPool_; // polymorphic

	// -- subsystems
	physics::PhysicsContext physicsCtx_ {};

public:
	Scene();

	// -- factory methods
	Entity* makeEntity();
	Light* makeLight(const render::LightDescriptor&);
	Camera* makeCamera(uint32 viewPortWidth, uint32 viewPortHeight);
	
	// -- components that can be used with multiple entities
	Renderable* makeRenderable();
	
	template <typename B, typename... Args> // B : public Behaviour
	B* makeBehaviour(Args... args) {
		auto b = new B(std::forward<Args>(args)...);
		behaviourPool_.emplace_back(b);
		return b;
	}

	template <typename M, typename... Args> // M : public Material
	M* makeMaterial(Args... args) {
		auto m = new M(std::forward<Args>(args)...);
		materialPool_.emplace_back(m);
		return m;
	}

	// -- single entity bound components
	physics::RigidBody* makeRigidBody(Entity&, float mass, float angInertia);

	template <typename C, typename... Args> // C : public Collider
	C* makeCollider(Entity& entity, Args... args) {
		auto collider = physicsCtx_.makeCollider<C>(std::ref(entity.transform), std::forward<Args>(args)...);
		entity.collider = collider;
		if (entity.rigidBody)
			collider->linkToRigidBody(*entity.rigidBody);
		return collider;
	}

	// -- observers
	auto entitiesBegin() { return entityPool_.begin(); }
	auto entitiesEnd() { return entityPool_.end(); }

	auto& camera() { return *camera_; }
	
	physics::PhysicsContext& physics() { return physicsCtx_; }
};


} // ns scene
} // ns stardazed

#endif
