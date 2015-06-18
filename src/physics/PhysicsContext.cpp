// ------------------------------------------------------------------
// physics::Integration.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/PhysicsContext.hpp"
#include "system/Logging.hpp"

namespace stardazed {
namespace physics {


PhysicsContext::PhysicsContext() {
	rigidBodyPool_.reserve(128);
	colliderPool_.reserve(256);
}


RigidBody* PhysicsContext::makeRigidBody(Transform& linkedTransform, float mass, float angInertia) {
	rigidBodyPool_.emplace_back(linkedTransform, mass, angInertia);
	return &rigidBodyPool_.back();
}


void PhysicsContext::integrateStep(Time t, Time dt) {
	for (auto& rigidBody : rigidBodyPool_) {
		rigidBody.update(t, dt);
	}
	
	auto C1 = colliderPool_[0].get();
	auto C2 = colliderPool_[1].get();
	
	sd::log(C1->worldBounds().intersects(C2->worldBounds()));
}


} // ns physics
} // ns stardazed
