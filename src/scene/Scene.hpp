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
	std::vector<std::unique_ptr<Behaviour>> behaviours_; // polymorphic

	// -- subsystems
	physics::PhysicsContext physicsCtx_ {};

public:
	Scene();

	// -- factory methods
	Entity* makeEntity(EntityType type = EntityType::Generic);
	Light* makeLight(const render::LightDescriptor&);
	Camera* makeCamera(uint32 viewPortWidth, uint32 viewPortHeight);

	template <typename B, typename... Args> // B : public Behaviour
	B* makeBehaviour(Args... args) {
		auto b = new B(std::forward<Args>(args)...);
		behaviours_.emplace_back(b);
		return b;
	}

	physics::RigidBody* makeRigidBody(Entity&, float mass, float angInertia);

	// -- observers
	auto entitiesBegin() { return entityPool_.begin(); }
	auto entitiesEnd() { return entityPool_.end(); }

	auto& camera() { return *camera_; }
	
	physics::PhysicsContext& physics() {
		return physicsCtx_;
	}
};


} // ns scene
} // ns stardazed

#endif
