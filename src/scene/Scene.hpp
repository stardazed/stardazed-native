// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENE_H
#define SD_SCENE_SCENE_H

#include "system/Config.hpp"
#include "container/RefTree.hpp"
#include "scene/Entity.hpp"
#include "scene/Light.hpp"
#include "scene/Camera.hpp"
#include "scene/Behaviour.hpp"

#include <vector>
#include <memory>

namespace stardazed {
namespace scene {


class Scene {
	std::vector<Camera> cameraPool_;
	std::vector<Light> lightPool_;
	std::vector<Entity> entityPool_;

	std::vector<std::unique_ptr<Behaviour>> behaviours_; // polymorphic
	std::vector<RigidBody> rigidBodyPool_;
	
	container::RefTree<Entity> entityTree_;

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
	
	RigidBody* makeRigidBody(physics::Mass);

	// -- scene::Entity access
	auto allEntitiesBegin() { return entityPool_.begin(); }
	auto allEntitiesEnd() { return entityPool_.end(); }
	
	auto rootEntitiesBegin() { return entityTree_.root().begin(); };
	auto rootEntitiesEnd() { return entityTree_.root().end(); }
	
	auto camerasBegin() { return cameraPool_.begin(); }
	auto camerasEnd() { return cameraPool_.end(); }
};


} // ns scene
} // ns stardazed

#endif
