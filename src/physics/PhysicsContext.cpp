// ------------------------------------------------------------------
// physics::Integration.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/PhysicsContext.hpp"

namespace stardazed {
namespace physics {


PhysicsContext::PhysicsContext() {
	rigidBodyPool_.reserve(128);
}


RigidBody* PhysicsContext::makeRigidBody(Transform& linkedTransform, Mass mass) {
	rigidBodyPool_.emplace_back(linkedTransform, mass);
	return &rigidBodyPool_.back();
}


} // ns physics
} // ns stardazed
