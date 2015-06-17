// ------------------------------------------------------------------
// physics::PhysicsState - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_PHYSICSSTATE_H
#define SD_PHYSICS_PHYSICSSTATE_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "physics/Transform.hpp"

namespace stardazed {
namespace physics {


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
