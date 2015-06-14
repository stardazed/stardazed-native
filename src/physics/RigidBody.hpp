// ------------------------------------------------------------------
// physics::RigidBody - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_RIGIDBODY_H
#define SD_PHYSICS_RIGIDBODY_H

#include "system/Config.hpp"
#include "physics/Integratable.hpp"
#include "physics/GlobalTime.hpp"

namespace stardazed {
namespace physics {


struct Environment {
	Acceleration3 gravity = earthGravity();
	MassDensity airDensity = earthSeaLevelAirDensity();
};


class RigidBody : public Integratable<RigidBody> {
	Transform previousTransform_;
	PhysicsState previous_, current_;
	
public:
	RigidBody(Transform& linkedTransform, const Mass, const AngInertia);
	
	void update(GlobalTime t, GlobalTime dt);
	
	// -- observable state
	const PhysicsState& state() const { return current_; }
	
	// -- shared semi-constant state
	void setMass(const Mass);
	void setAngularInertia(const AngInertia);

	// -- behaviour-generated force
	Force3 userForce;
	Torque3 userTorque;
	void addForce(const Force3& force) { userForce += force; }
	void addTorque(const Torque3& torque) { userTorque += torque; }
	
	// -- integration
	void calcForces(const PhysicsState&, const Time globalTime, Force3& outForce, Torque3& outTorque) const;
};


} // ns physics
} // ns stardazed

#endif
