// ------------------------------------------------------------------
// physics::Collider.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/Collider.hpp"

namespace stardazed {
namespace physics {


BoxCollider::BoxCollider(Transform& transform, const math::Vec3& localCenter, const math::Vec3& size)
: transform_(transform)
{
	localBounds_.setCenterAndSize(localCenter, size);
	worldBounds_.setCenterAndSize(transform.position + localCenter, size * transform.scale);
}


const math::Bounds& BoxCollider::worldBounds() {
	if (rigidBody_ && math::lengthSquared(rigidBody_->state().velocity()) > 0) {
		worldBounds_.setCenterAndSize(transform_.position + localBounds_.center(), localBounds_.size() * transform_.scale);
	}
	
	return worldBounds_;
}


SphereCollider::SphereCollider(Transform& transform, const math::Vec3& localCenter, float radius)
: transform_(transform)
{
	radius_ = radius;
	auto dimension = radius * 2;
	math::Vec3 size { dimension, dimension, dimension };
	
	localBounds_.setCenterAndSize(localCenter, size);
	worldBounds_.setCenterAndSize(transform.position + localCenter, size * transform.scale);
}


const math::Bounds& SphereCollider::worldBounds() {
	if (rigidBody_ && math::lengthSquared(rigidBody_->state().velocity()) > 0) {
		worldBounds_.setCenterAndSize(transform_.position + localBounds_.center(), localBounds_.size() * transform_.scale);
	}
	
	return worldBounds_;
}


} // ns physics
} // ns stardazed
