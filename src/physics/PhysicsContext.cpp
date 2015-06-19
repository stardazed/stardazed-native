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

	auto collEnd = colliderPool_.end();

	// fun little n log n algo for my couple of colliders
	for (auto collBegin = colliderPool_.begin(); collBegin != collEnd; ++collBegin) {
		for (auto nextBegin = collBegin + 1; nextBegin != collEnd; ++nextBegin) {
			auto& collA = *collBegin;
			auto& collB = *nextBegin;

			if (collA->worldBounds().intersects(collB->worldBounds())) {
				// GJK, essentially
			}
		}
	}
}


} // ns physics
} // ns stardazed
