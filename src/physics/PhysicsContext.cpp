// ------------------------------------------------------------------
// physics::Integration.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/PhysicsContext.hpp"
#include "system/Logging.hpp"

namespace stardazed {
namespace physics {


PhysicsContext::PhysicsContext() {
}


RigidBody* PhysicsContext::makeRigidBody(Transform& linkedTransform, float mass, float angInertia) {
	return nullptr;
}


void PhysicsContext::integrateStep(Time t, Time dt) {
	for (auto& rigidBody : rigidBodyPool_) {
		rigidBody.update(t, dt);
	}

	auto collEnd = colliderPool_.end();
	float bounciness = 0.3;
	
	using namespace math;

	// fun little n2 algo for my couple of colliders
	for (auto collBegin = colliderPool_.begin(); collBegin != collEnd; ++collBegin) {
		auto& collA = *collBegin;
		auto rigidBodyA = collA->linkedRigidBody();
		if (! rigidBodyA)
			continue;

		for (auto nextBegin = colliderPool_.begin(); nextBegin != collEnd; ++nextBegin) {
			auto& collB = *nextBegin;
			
			if (&collB == &collA)
				continue;
			
			if (collA->worldBounds().intersects(collB->worldBounds())) {
				auto dA = rigidBodyA->state().transform.position - rigidBodyA->previousState().transform.position;
				auto tEnter = (collB->worldBounds().min() - (collA->worldBounds().max() - dA)) / dA;
				auto tLeave = (collB->worldBounds().max() - (collA->worldBounds().min() - dA)) / dA;
				
				// find bounce time and normal of closest plane
				float tEnterMin = 100.0;
				math::Vec3 bounceNormal, bounceFriction;
				
				if (! nearEqual(0.0f, dA.x)) {
					auto tEnterX = min(tEnter.x, tLeave.x);
					if (tEnterX > 0.0f) {
						tEnterMin = min(tEnter.x, tLeave.x);
						bounceNormal = normalize(Vec3{ -dA.x, 0, 0 });
						bounceFriction = { bounciness, 1, 1 };
					}
				}
				if (! nearEqual(0.0f, dA.y)) {
					auto tEnterY = min(tEnter.y, tLeave.y);
					if (tEnterY > 0 && tEnterY < tEnterMin) {
						tEnterMin = tEnterY;
						bounceNormal = normalize(Vec3{ 0, -dA.y, 0 });
						bounceFriction = { 1, bounciness, 1 };
					}
				}
				if (! nearEqual(0.0f, dA.z)) {
					auto tEnterZ = min(tEnter.z, tLeave.z);
					if (tEnterZ > 0 && tEnterZ < tEnterMin) {
						tEnterMin = tEnterZ;
						bounceNormal = normalize(Vec3{ 0, 0, -dA.z });
						bounceFriction = { 1, 1, bounciness };
					}
				}
				
				// --
				if (tEnterMin > 0.0f && tEnterMin < 100.0f) {
					auto dVA = rigidBodyA->state().velocity() - rigidBodyA->previousState().velocity();
					auto velAtHit = rigidBodyA->previousState().velocity() + (dVA * tEnterMin);
					auto velOut = reflect(velAtHit, bounceNormal) * bounceFriction;

					auto clippedPos = rigidBodyA->previousState().transform.position + (dA * tEnterMin);
					collA->linkedTransform().position = clippedPos + ((1.0 - tEnterMin) * reflect(dA, bounceNormal) * bounciness);
//					sd::log(rigidBodyA->previousState().transform.position, clippedPos, collA->linkedTransform().position);

					rigidBodyA->state().momentum = velOut * rigidBodyA->state().mass();
//					rigidBodyA->state().momentum = Vec3::zero();
					rigidBodyA->state().recalcSecondaryValues();
//
					sd::log(rigidBodyA->previousState().velocity(), rigidBodyA->state().velocity());
				}
			}
		}
	}
}


} // ns physics
} // ns stardazed
