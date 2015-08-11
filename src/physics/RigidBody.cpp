// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


//	void PhysicsState::recalcSecondaryValues() {
//		velocity_ = momentum * oneOverMass_;
//		angularVelocity_ = angularMomentum * oneOverAngularInertia_;
//		math::normalizeInPlace(transform.rotation);
//		spin_ = 0.5 * math::Quat{angularVelocity_, 0} * transform.rotation;
//	}
//	
//	
//	void PhysicsState::copyPrimaryAndSecondaryValuesFrom(const PhysicsState& other) {
//		transform = other.transform;
//		momentum = other.momentum;
//		angularMomentum = other.angularMomentum;
//		
//		velocity_ = other.velocity_;
//		angularVelocity_ = other.angularVelocity_;
//		spin_ = other.spin_;
//	}


//RigidBody::RigidBody(Transform& linkedTransform, float mass, float angularInertia)
//: previousTransform_(linkedTransform)
//, previous_{ previousTransform_, mass, angularInertia }
//, current_ { linkedTransform, mass, angularInertia }
//{}
//
//
//void RigidBody::update(Time t, Time dt) {
//	previous_.copyPrimaryAndSecondaryValuesFrom(current_);
//	integrate(current_, t, dt);
//	userForce = {0, 0, 0};
//	userTorque = {0, 0, 0};
//}
//
//
//void RigidBody::calcForces(const PhysicsState& state, const Time /*globalTime*/, math::Vec3& outForce, math::Vec3& outTorque) const {
//	outForce  = userForce - state.momentum * .5;
//	outTorque = userTorque - state.angularMomentum * .5;
//}


} // ns physics
} // ns stardazed
