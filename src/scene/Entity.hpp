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
#include "physics/Collider.hpp"
#include "scene/MeshRenderer.hpp"

namespace stardazed {
namespace scene {


struct Entity {
	// -- components
	physics::Transform transform;
	physics::RigidBody* rigidBody = nullptr;
	physics::Collider* collider = nullptr;
	MeshRenderer* renderer = nullptr;
	Behaviour* behaviour = nullptr;
	
	// -- util
	uint32 tag = 0;
};


} // ns scene
} // ns stardazed

#endif
