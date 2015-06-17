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


RigidBody* PhysicsContext::makeRigidBody(Transform& linkedTransform, float mass, float angInertia) {
	rigidBodyPool_.emplace_back(linkedTransform, mass, angInertia);
	return &rigidBodyPool_.back();
}


void PhysicsContext::integrateStep(Time t, Time dt) {
	for (auto& rigidBody : rigidBodyPool_) {
		rigidBody.update(t, dt);
	}
}


} // ns physics
} // ns stardazed
