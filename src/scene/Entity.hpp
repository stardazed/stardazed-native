// ------------------------------------------------------------------
// scene::Entity - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_ENTITY_H
#define SD_SCENE_ENTITY_H

#include "system/Config.hpp"
#include "scene/Transform.hpp"
#include "scene/Behaviour.hpp"

namespace stardazed {
namespace scene {


enum class EntityType {
	Generic,
	Light,
	Camera
};


class Entity;


class Renderable {
public:
	virtual ~Renderable() = default;
	
	virtual void render(const math::Mat4& projMat, const math::Mat4& viewMat, const Entity& entity) const = 0;
};


class Entity {
	EntityType type_;

public:
	Entity(EntityType type) : type_(type) {}
	Entity() : Entity(EntityType::Generic) {}

	EntityType type() const { return type_; }

	// -- components
	Transform transform;
	scene::Renderable* renderable = nullptr;
	scene::Behaviour* behaviour = nullptr;
};


} // ns scene
} // ns stardazed

#endif
