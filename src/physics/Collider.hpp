// ------------------------------------------------------------------
// physics::Collider - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_COLLIDER_H
#define SD_PHYSICS_COLLIDER_H

#include "system/Config.hpp"
#include "physics/Units.hpp"

namespace stardazed {
namespace physics {


enum class ColliderType {
	Sphere,
	Box,
	Mesh
};


class Collider {
	ColliderType type_;

public:
	Collider(ColliderType);
};
	
	
} // ns physics
} // ns stardazed

#endif
