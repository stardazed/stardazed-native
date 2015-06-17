// ------------------------------------------------------------------
// physics::Collider.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/Collider.hpp"

namespace stardazed {
namespace physics {


BoxCollider::BoxCollider(const math::Vec3& size) {
	bounds_.setCenterAndSize(math::Vec3::zero(), size);
}


} // ns physics
} // ns stardazed
