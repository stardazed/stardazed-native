// ------------------------------------------------------------------
// physics::RigidBody - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_RIGIDBODY_H
#define SD_PHYSICS_RIGIDBODY_H

#include "system/Config.hpp"
#include "physics/Units.hpp"
#include "physics/Transform.hpp"

namespace stardazed {
namespace physics {


struct Environment;


class RigidBody {
	Transform* transform_;
//	Transform nextTransform_; this will be used when collision detection etc comes into play

	Mass mass_{};
	InverseMass oneOverMass_{};
	Momentum3 momentum_ {};
	Velocity3 velocity_ {};

public:
	explicit RigidBody(Transform& transform, Mass mass = 1_kg)
	: transform_(&transform)
	{
		setMass(mass);
	}

	// -- observers for primary and secondary values

	Transform& transform() { return *transform_; }
	const Transform& transform() const { return *transform_; }
	void useTransform(Transform& newTransformRef) { transform_ = &newTransformRef; }

	Mass mass() const { return mass_; }
	void setMass(Mass mass) {
		mass_ = mass;
		oneOverMass_ = 1 / mass_;
	}

	const Momentum3& momentum() const { return momentum_; }
	void setMomentum(Momentum3 newMomentum) {
		momentum_ = newMomentum;
		velocity_ = momentum_ * oneOverMass_;
	}

	const Velocity3& velocity() const { return velocity_; }

	// -- behaviour-generated force

	Force3 userForce;
	void addForce(const Force3& force) {
		userForce += force;
	}
	
	// -- integration
	Force3 calcForce(const Environment&, const Time& globalTime) const;
};


} // ns physics
} // ns stardazed

#endif
