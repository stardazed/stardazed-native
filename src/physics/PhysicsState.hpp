// ------------------------------------------------------------------
// physics::PhysicsManager - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_PHYSICSMANAGER_H
#define SD_PHYSICS_PHYSICSMANAGER_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "math/Quaternion.hpp"
#include "container/MultiArrayBuffer.hpp"
#include "scene/Transform.hpp"

namespace stardazed {
namespace physics {


class PhysicsManager {
	scene::TransformComponent& transform_;

	container::MultiArrayBuffer<
		// constant
		float, // mass
		float, // inverseMass
		float, // angularInertia
		float  // inverseAngInertia

		// secondary
		math::Vec3, // velocity
		math::Vec3, // angularVelocity
		math::Quat, // spin

		// primary
		TransformComponent::Instance, // linkedTransform (position, rotation)
		math::Vec3, // momentum
		math::Vec3  // angularMomentum
	> instanceData_;

public:
	PhysicsManager(memory::Allocator&, scene::TransformComponent&);
};


class PhysicsState {
	// (semi-)constant values
	float mass_ {};
	float angularInertia_ {};

	float oneOverMass_ {};
	float oneOverAngularInertia_ {};

	// secondary values
	math::Vec3 velocity_ {};
	math::Vec3 angularVelocity_ {};
	math::Quat spin_{};

public:
	// primary values
	Transform& transform;
	math::Vec3 momentum {};
	math::Vec3 angularMomentum {};
	
	PhysicsState(Transform& transform, float mass, float angularInertia)
	: transform(transform)
	{
		setMass(mass);
		setAngularInertia(angularInertia);
	}

	SD_DEFAULT_MOVE_OPS(PhysicsState)

	// -- observers for semi-constant state

	float mass() const { return mass_; }
	void setMass(float mass) {
		mass_ = mass;
		oneOverMass_ = 1 / mass;
	}

	float angularInertia() const { return angularInertia_; }
	void setAngularInertia(float angInertia) {
		angularInertia_ = angInertia;
		oneOverAngularInertia_ = 1 / angInertia;
	}

	// -- observers for secondary state
	
	const math::Vec3& velocity() const { return velocity_; }
	const math::Quat& spin() const { return spin_; }
	const math::Vec3& angularVelocity() { return angularVelocity_; }
	
	void recalcSecondaryValues();
	void copyPrimaryAndSecondaryValuesFrom(const PhysicsState&);
};


} // ns physics
} // ns stardazed

#endif
