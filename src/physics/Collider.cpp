// ------------------------------------------------------------------
// physics::Collider.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/Collider.hpp"
#include "system/Logging.hpp"

namespace stardazed {
namespace physics {


ColliderManager::ColliderManager(memory::Allocator& allocator, scene::TransformManager& tm, RigidBodyManager& rbm)
: transformMgr_(tm)
, rigidBodyMgr_(rbm)
, instanceData_(allocator, 1024)
{
	instanceData_.extend(); // instance 0 is a null-instance
}


auto ColliderManager::create(scene::Entity entity, ColliderType type, const math::Vec3& localCenter, const math::Vec3& size) -> Instance {
	instanceData_.extend();
	uint index = instanceData_.count() - 1;
	
	auto trans = transformMgr_.forEntity(entity);
	auto rigid = rigidBodyMgr_.forEntity(entity); // may be a null-instance
	
	using math::Bounds;
	
	*(basePtr<InstField::Type>() + index) = type;
	*(basePtr<InstField::Transform>() + index) = trans;
	*(basePtr<InstField::RigidBody>() + index) = rigid;
	*(basePtr<InstField::LocalBounds>() + index) = Bounds::fromCenterAndSize(localCenter, size);
	*(basePtr<InstField::WorldBounds>() + index) = Bounds::fromCenterAndSize(localCenter + transformMgr_.position(trans), size * transformMgr_.scale(trans));
	
	Instance h { index };
	entityMap_.insert(entity, h);
	return h;

}


void ColliderManager::linkToRigidBody(Instance h, RigidBodyManager::Instance rb) {
	*(basePtr<InstField::RigidBody>() + h.ref) = rb;
}


RigidBodyManager::Instance ColliderManager::linkedRigidBody(Instance h) const {
	return *(basePtr<InstField::RigidBody>() + h.ref);
}


void ColliderManager::resolveAll() {
	auto collCount = instanceData_.count();
	const float bounciness = 0.3;
	
	using namespace math;
	
	auto linkedBodyBase = basePtr<InstField::RigidBody>();
	auto localBoundsBase = basePtr<InstField::LocalBounds>();
	auto worldBoundsBase = basePtr<InstField::WorldBounds>();
	auto transformBase = basePtr<InstField::Transform>();
	
	// fun little n^2 algo for my couple of colliders
	for (auto loopIndex = 1u; loopIndex < collCount; ++loopIndex) {
		auto rigidBodyA = linkedBodyBase[loopIndex];
		if (! rigidBodyA)
			continue;
		
		auto transA = transformBase[loopIndex];
		auto& posA = transformMgr_.position(transA);

		auto& localABounds = localBoundsBase[loopIndex];
		Bounds collABounds = Bounds::fromCenterAndSize(localABounds.center() + posA, localABounds.size() * transformMgr_.scale(transA));
		
		for (auto subIndex = 1u; subIndex < collCount; ++subIndex) {
			if (subIndex == loopIndex)
				continue;

			auto& collBBounds = worldBoundsBase[subIndex];
			
			if (collABounds.intersects(collBBounds)) {
				auto previousPosition = rigidBodyMgr_.previousPosition(rigidBodyA);

				auto dA = posA - previousPosition;
				auto tEnter = (collBBounds.min() - (collABounds.max() - dA)) / dA;
				auto tLeave = (collBBounds.max() - (collABounds.min() - dA)) / dA;
				
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
					auto previousVelocity = rigidBodyMgr_.previousVelocity(rigidBodyA);
					auto dVA = rigidBodyMgr_.velocity(rigidBodyA) - previousVelocity;
					auto velAtHit = previousVelocity + (dVA * tEnterMin);
					auto velOut = reflect(velAtHit, bounceNormal) * bounceFriction;
					
					auto clippedPos = previousPosition + (dA * tEnterMin);
					transformMgr_.setPosition(transA, clippedPos + ((1.0 - tEnterMin) * reflect(dA, bounceNormal) * bounciness));
//					sd::log(rigidBodyA->previousState().transform.position, clippedPos, collA->linkedTransform().position);
					
					rigidBodyMgr_.setVelocity(rigidBodyA, velOut);
//					rigidBodyA->state().recalcSecondaryValues();
					
					sd::log(previousVelocity, rigidBodyMgr_.velocity(rigidBodyA));
				}
			}
		}
	}
}

} // ns physics
} // ns stardazed
