// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


RigidBodyManager::RigidBodyManager(memory::Allocator& allocator, scene::TransformManager& transform)
: transformMgr_(transform)
, instanceData_{ allocator, 1024 }
, entityMap_{ allocator, 1024 }
{
	instanceData_.extend();  // index 0 is a null-instance
}


auto RigidBodyManager::create(scene::Entity entity, const RigidBodyDescriptor& desc) -> Instance {
	instanceData_.extend();
	uint index = instanceData_.count() - 1;
	
	// FIXME: calc average drag intersection area, right now A = 1

	*(basePtr<InstField::Properties>() + index) = { true, desc.obeysGravity };
	*(basePtr<InstField::Mass>() + index) = { desc.mass, 1.0f / desc.mass };
	*(basePtr<InstField::Drag>() + index) = { desc.drag, 1.0f / desc.drag };
	*(basePtr<InstField::AngularDrag>() + index) = { desc.angularDrag, 1.0f / desc.angularDrag };
	*(basePtr<InstField::Transform>() + index) = transformMgr_.forEntity(entity);

	Instance h { index };
	entityMap_.insert(entity, h);
	return h;
}


auto RigidBodyManager::forEntity(scene::Entity ent) -> Instance {
	Instance* result = entityMap_.find(ent);
	return result ? *result : Instance{0};
}


void RigidBodyManager::addForce(Instance h, const math::Vec3& force) {
	auto fp = instancePtr<InstField::ExternalForce>(h);
	*fp += force;
}


void RigidBodyManager::addTorque(Instance h, const math::Vec3& torque) {
	auto tp = instancePtr<InstField::ExternalTorque>(h);
	*tp += torque;
}


void RigidBodyManager::integrateAll(Time dt) {
	auto propertiesBase = basePtr<InstField::Properties>() + 1;
	auto massBase = basePtr<InstField::Mass>() + 1;
	auto dragBase = basePtr<InstField::Drag>() + 1;
	auto transformBase = basePtr<InstField::Transform>() + 1;
	auto velocityBase = basePtr<InstField::Velocity>() + 1;
	auto externalForceBase = basePtr<InstField::ExternalForce>() + 1;
	auto accelerationBase = basePtr<InstField::PreviousAcceleration>() + 1;
	auto previousPositionBase = basePtr<InstField::PreviousPosition>() + 1;
	auto previousVelocityBase = basePtr<InstField::PreviousVelocity>() + 1;

	using namespace math;

	const Vec3 gravityAccel { 0, -9.80665, 0 };
	const float halfTimeStepSquared = .5 * dt * dt;

	for (uint rbi=1, rbCount = instanceData_.count(); rbi < rbCount; ++rbi) {
		auto properties = *propertiesBase;
		auto transform = *transformBase;
		auto velocity = *velocityBase;
		auto acceleration = *accelerationBase;
		auto dragArea = dragBase->value;
		
		Vec3 totalForce {};
		if (properties.gravity)
			totalForce += massBase->value * gravityAccel;
		
		if (! (nearEqual(0.0f, lengthSquared(velocity)) && nearEqual(0.0f, dragArea)) ) {
			auto signedDirection = math::sign(velocity);

			// 1.2f is air drag (rho) at 15C at 0m elevation
			totalForce -= .5f * 1.2f * dragArea * velocity * velocity * signedDirection;

			// friction (fixed for now)
			totalForce -= .5f * velocity;
		}

		totalForce += *externalForceBase;
		auto newAcceleration = totalForce * massBase->reciprocal;
		
		// Velocity Verlet Integration
		// -- update position and keep old position (for collision tests)
		auto dX = velocity * dt + (halfTimeStepSquared * acceleration);
		auto curPos = transformMgr_.position(transform);
		*previousPositionBase = curPos;
		transformMgr_.setPosition(transform, curPos + dX);

		// -- update acceleration and velocity
		Vec3 smoothedAccel = (acceleration + newAcceleration) * .5f;
		auto newVelocity = velocity + (smoothedAccel * dt);
		if (lengthSquared(newVelocity) < 0.01) // prevent endless drifting
			newVelocity = Vec3::zero();

		*previousVelocityBase = velocity;
		*velocityBase = newVelocity;
		*accelerationBase = smoothedAccel;

		++propertiesBase;
		++massBase;
		++dragBase;
		++transformBase;
		++velocityBase;
		++externalForceBase;
		++accelerationBase;
		++previousPositionBase;
		++previousVelocityBase;
	}
	
	// clear all external forces
	memset(basePtr<InstField::ExternalForce>(), 0, instanceData_.count() * sizeof(Vec3));
}


void RigidBodyManager::setVelocity(Instance h, const math::Vec3& newVel) {
	assert(valid(h));
	*(basePtr<InstField::Velocity>() + h.ref) = newVel;
}



} // ns physics
} // ns stardazed
