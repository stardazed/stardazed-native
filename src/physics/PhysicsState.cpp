// ------------------------------------------------------------------
// physics::PhysicsState.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/PhysicsState.hpp"

namespace stardazed {
namespace physics {


void PhysicsState::recalcSecondaryValues() {
	velocity_ = momentum * oneOverMass_;
	angularVelocity_ = angularMomentum * oneOverAngularInertia_;
	math::normalizeInPlace(transform.rotation);
	spin_ = 0.5 * math::Quat{angularVelocity_.value, 0} * transform.rotation;
}


void PhysicsState::copyPrimaryAndSecondaryValuesFrom(const PhysicsState& other) {
	transform = other.transform;
	momentum = other.momentum;
	angularMomentum = other.angularMomentum;
	
	velocity_ = other.velocity_;
	angularVelocity_ = other.angularVelocity_;
	spin_ = other.spin_;
}


} // ns physics
} // ns stardazed
