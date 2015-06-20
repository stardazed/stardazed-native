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

	virtual const math::Bounds& worldBounds() = 0;
	virtual const Transform& linkedTransform() const = 0;

	virtual void linkToRigidBody(RigidBody&) = 0;
	virtual RigidBody* linkedRigidBody() const = 0;
	virtual bool isStatic() const = 0;
};


class BoxCollider : public Collider {
	math::Bounds localBounds_, worldBounds_;
	const Transform& transform_;
	RigidBody* rigidBody_ = nullptr;

public:
	BoxCollider(const Transform&, const math::Vec3& localCenter, const math::Vec3& size);
	const math::Bounds& worldBounds() final;
	const Transform& linkedTransform() const final { return transform_; }

	void linkToRigidBody(RigidBody& rigidBody) final {
		rigidBody_ = &rigidBody;
	}
	RigidBody* linkedRigidBody() const final { return rigidBody_; }
	bool isStatic() const final { return rigidBody_ == nullptr; }
};


class SphereCollider : public Collider {
	math::Bounds localBounds_, worldBounds_;
	const Transform& transform_;
	RigidBody* rigidBody_ = nullptr;
	float radius_;

public:
	SphereCollider(const Transform&, const math::Vec3& localCenter, float radius);
	const math::Bounds& worldBounds() final;
	const Transform& linkedTransform() const final { return transform_; }

	void linkToRigidBody(RigidBody& rigidBody) final {
		rigidBody_ = &rigidBody;
	}
	RigidBody* linkedRigidBody() const final { return rigidBody_; }
	bool isStatic() const final { return rigidBody_ == nullptr; }
};


} // ns physics
} // ns stardazed

#endif
