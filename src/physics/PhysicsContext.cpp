// ------------------------------------------------------------------
// physics::Integration.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/PhysicsContext.hpp"
#include "physics/Integration.hpp"

namespace stardazed {
namespace physics {


PhysicsContext::PhysicsContext() {
	rigidBodyPool_.reserve(128);
}


RigidBody* PhysicsContext::makeRigidBody(Transform& linkedTransform, Mass mass) {
	rigidBodyPool_.emplace_back(linkedTransform, mass);
	return &rigidBodyPool_.back();
}


void PhysicsContext::integrateStep(GlobalTime t, GlobalTime dt) {
	integrate(rigidBodyPool_.begin(), rigidBodyPool_.end(), t, Time(dt));
}


} // ns physics
} // ns stardazed
