// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


RigidBodyManager::RigidBodyManager(memory::Allocator& allocator, scene::TransformComponent& transform)
: transformMgr_(transform)
, instanceData_{ allocator, 1024 }
, entityMap_{ allocator, 1024 }
{}


auto RigidBodyManager::create(scene::Entity entity, const RigidBodyDescriptor& desc) -> Instance {
	instanceData_.extend();
	uint index = instanceData_.count() - 1;
	
	// FIXME: calc average drag intersection area, right now A = 1

	*(basePtr<InstField::Mass>() + index) = { desc.mass, 1.0f / desc.mass };
	*(basePtr<InstField::Drag>() + index) = { desc.drag, 1.0f / desc.drag };
	*(basePtr<InstField::AngularDrag>() + index) = { desc.angularDrag, 1.0f / desc.angularDrag };
	*(basePtr<InstField::Transform>() + index) = transformMgr_.forEntity(entity);

	Instance h { index };
	entityMap_.insert(entity, h);
	return h;
}


void RigidBodyManager::addForce(Instance h, const math::Vec3& force) {
	auto fp = instancePtr<InstField::ExternalForce>(h);
	*fp += force;
}


void RigidBodyManager::addTorque(Instance h, const math::Vec3& torque) {
	auto tp = instancePtr<InstField::ExternalTorque>(h);
	*tp += torque;
}
//
//
//void RigidBodyManager::recalcSecondaries(Instance h) {
//	velocity(h) = momentum(h) * inverseMass(h);
//	angularVelocity(h) = angularMomentum(h) * inverseAngInertia(h);
//	
//	auto transInst = linkedTransform(h);
//	auto rotation = transformMgr_.rotation(transInst);
//	transformMgr_.setRotation(transInst, math::normalize(rotation));
//	spin(h) = 0.5f * math::Quat{angularVelocity(h), 0} * rotation;
//}


void RigidBodyManager::integrateAll(Time dt) {
	auto propertiesBase = basePtr<InstField::Properties>();
	auto massBase = basePtr<InstField::Mass>();
	auto dragBase = basePtr<InstField::Drag>();
	auto transformBase = basePtr<InstField::Transform>();
	auto velocityBase = basePtr<InstField::Velocity>();
	auto externalForceBase = basePtr<InstField::ExternalForce>();
	auto accelerationBase = basePtr<InstField::PreviousAcceleration>();
	auto previousPositionBase = basePtr<InstField::PreviousPosition>();

	using namespace math;

	const Vec3 gravityAccel { 0, 9.80665, 0 };
	const float halfTimeStepSquared = .5 * dt * dt;

	for (uint rbi=0; rbi < instanceData_.count(); ++rbi) {
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
			totalForce -= .5 * velocity;
		}

		totalForce += *externalForceBase;

		auto newAcceleration = totalForce * massBase->reciprocal;
		
		/*
		last_acceleration = acceleration
		position += velocity * time_step + ( 0.5 * last_acceleration * time_step^2 )
		new_acceleration = force / mass
		avg_acceleration = ( last_acceleration + new_acceleration ) / 2
		velocity += avg_acceleration * time_step
		*/

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
	}
	
	// clear all external forces
	memset(basePtr<InstField::ExternalForce>(), 0, instanceData_.count() * sizeof(Vec3));
}


/*
	
	
	void PhysicsState::copyPrimaryAndSecondaryValuesFrom(const PhysicsState& other) {
		transform = other.transform;
		momentum = other.momentum;
		angularMomentum = other.angularMomentum;
		
		velocity_ = other.velocity_;
		angularVelocity_ = other.angularVelocity_;
		spin_ = other.spin_;
	}


	RigidBody::RigidBody(Transform& linkedTransform, float mass, float angularInertia)
	: previousTransform_(linkedTransform)
	, previous_{ previousTransform_, mass, angularInertia }
	, current_ { linkedTransform, mass, angularInertia }
	{}


	void RigidBody::update(Time t, Time dt) {
		previous_.copyPrimaryAndSecondaryValuesFrom(current_);
		integrate(current_, t, dt);
		userForce = {0, 0, 0};
		userTorque = {0, 0, 0};
	}
*/


} // ns physics
} // ns stardazed
