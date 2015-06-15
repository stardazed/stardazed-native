// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


RigidBody::RigidBody(Transform& linkedTransform, const Mass mass, const AngInertia angularInertia)
: previousTransform_(linkedTransform)
, previous_{ previousTransform_, mass, angularInertia }
, current_ { linkedTransform, mass, angularInertia }
{}


void RigidBody::update(GlobalTime t, GlobalTime dt) {
	previous_.copyPrimaryAndSecondaryValuesFrom(current_);
	integrate(current_, Time{t}, Time{dt});
	userForce = Force3{0,0,0};
	userTorque = Torque3{0,0,0};
}


void RigidBody::setMass(const Mass mass) {
	current_.setMass(mass);
}


void RigidBody::setAngularInertia(const AngInertia angularInertia) {
	current_.setAngularInertia(angularInertia);
}


void RigidBody::calcForces(const PhysicsState& state, const Time /*globalTime*/, Force3& outForce, Torque3& outTorque) const {
	outForce  = userForce - Force3{ state.momentum.value * .5 };
	outTorque = userTorque - Torque3{ state.angularMomentum.value * .5 };
}


} // ns physics
} // ns stardazed
