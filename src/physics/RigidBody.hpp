// ------------------------------------------------------------------
// physics::RigidBody - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_RIGIDBODY_H
#define SD_PHYSICS_RIGIDBODY_H

#include "system/Config.hpp"
#include "physics/Structures.hpp"
#include "physics/Transform.hpp"

namespace stardazed {
namespace physics {


class RigidBody {
	using InverseMass3 = physics::SIValue3<-1, 0, 0>;

	Transform& transform_;
	physics::Mass mass_{1};
	InverseMass3 oneOverMass_{1};
	physics::Momentum3 momentum_ {};
	physics::Velocity3 velocity_ {};
	
public:
	explicit RigidBody(Transform& transform, physics::Mass mass = 1)
	: transform_(transform)
	, mass_(mass)
	{}

	Transform& transform() { return transform_; }

	physics::Mass mass() const { return mass_; }
	void setMass(physics::Mass mass) {
		mass_ = mass;
		oneOverMass_ = 1 / physics::splat<3>(mass_);
	}

	const physics::Momentum3& momentum() const { return momentum_; }
	void setMomentum(physics::Momentum3 newMomentum) {
		momentum_ = newMomentum;
		velocity_ = momentum_ * oneOverMass_;
	}

	const physics::Velocity3& velocity() const { return velocity_; }
};
	
	
} // ns physics
} // ns stardazed

#endif
