// ------------------------------------------------------------------
// physics::Collider - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_COLLIDER_H
#define SD_PHYSICS_COLLIDER_H

#include "system/Config.hpp"
#include "math/Bounds.hpp"
#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


struct Collider {
	virtual ~Collider() = default;

	virtual const math::Bounds& bounds() = 0;
};


class BoxCollider : public Collider {
	math::Bounds bounds_;

public:
	BoxCollider(const math::Vec3& size);
	const math::Bounds& bounds() final { return bounds_; }
};


} // ns physics
} // ns stardazed

#endif
