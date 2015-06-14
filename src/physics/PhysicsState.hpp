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
	Mass mass_ {};
	AngInertia angularInertia_ {};
	InverseMass oneOverMass_ {};
	InverseAngInertia oneOverAngularInertia_ {};

	// secondary values
	Velocity3 velocity_ {};
	AngVelocity3 angularVelocity_ {};
	math::Quat spin_{};

public:
	// primary values
	Transform& transform;
	Momentum3 momentum {};
	AngMomentum3 angularMomentum {};
	
	PhysicsState(Transform& transform, const Mass mass, const AngInertia angularInertia)
	: transform(transform)
	{
		setMass(mass);
		setAngularInertia(angularInertia);
	}

	SD_DEFAULT_MOVE_OPS(PhysicsState)

	// -- observers for semi-constant state

	Mass mass() const { return mass_; }
	void setMass(const Mass mass) {
		mass_ = mass;
		oneOverMass_ = 1 / mass;
	}

	AngInertia angularInertia() const { return angularInertia_; }
	void setAngularInertia(const AngInertia angInertia) {
		angularInertia_ = angInertia;
		oneOverAngularInertia_ = 1 / angInertia;
	}

	// -- observers for secondary state
	
	const Velocity3& velocity() const { return velocity_; }
	const math::Quat& spin() const { return spin_; }
	const AngVelocity3& angularVelocity() { return angularVelocity_; }
	
	void recalcSecondaryValues();
	void copyPrimaryAndSecondaryValuesFrom(const PhysicsState&);
};


} // ns physics
} // ns stardazed

#endif
