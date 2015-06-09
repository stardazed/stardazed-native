// ------------------------------------------------------------------
// scene::Entity - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_ENTITY_H
#define SD_SCENE_ENTITY_H

#include "system/Config.hpp"
#include "scene/Behaviour.hpp"
#include "physics/Transform.hpp"
#include "physics/RigidBody.hpp"

namespace stardazed {
namespace scene {


enum class EntityType {
	Generic,
	Light,
	Camera
};


class Renderable;


class Entity {
	EntityType type_;

public:
	Entity(EntityType type) : type_(type) {}
	Entity() : Entity(EntityType::Generic) {}

	EntityType type() const { return type_; }

	// -- components
	physics::Transform transform;
	physics::RigidBody* rigidBody = nullptr;
	Renderable* renderable = nullptr;
	Behaviour* behaviour = nullptr;
};


} // ns scene
} // ns stardazed

#endif
