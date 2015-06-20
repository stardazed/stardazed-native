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
		auto& collA = *collBegin;
		auto rigidBodyA = collA->linkedRigidBody();
		if (! rigidBodyA)
			continue;

		for (auto nextBegin = collBegin + 1; nextBegin != collEnd; ++nextBegin) {
			auto& collB = *nextBegin;
			auto rigidBodyB = collB->linkedRigidBody();

			if (collA->worldBounds().intersects(collB->worldBounds())) {
				auto forceA = rigidBodyA->state().momentum / dt;
				auto forceB = rigidBodyB ? (rigidBodyB->state().momentum / dt) : math::Vec3::zero();
				auto totalForce = forceA + forceB;
				
				auto dA = rigidBodyA->state().transform.position - rigidBodyA->previousState().transform.position;
				auto tEnter = (collB->worldBounds().min() - (collA->worldBounds().max() - dA)) / dA;
				auto tLeave = (collB->worldBounds().max() - (collA->worldBounds().min() - dA)) / dA;
			}
		}
	}
}


} // ns physics
} // ns stardazed
