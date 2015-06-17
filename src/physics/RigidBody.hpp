// ------------------------------------------------------------------
// physics::RigidBody - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_RIGIDBODY_H
#define SD_PHYSICS_RIGIDBODY_H

#include "system/Config.hpp"
#include "system/Time.hpp"
#include "physics/Integratable.hpp"

namespace stardazed {
namespace physics {


struct Environment {
	math::Vec3 gravity = earthGravity();
	float airDensity = earthSeaLevelAirDensity();
};


class RigidBody : public Integratable<RigidBody> {
	Transform previousTransform_;
	PhysicsState previous_, current_;
	
public:
	RigidBody(Transform& linkedTransform, float mass, float angInertia);
	
	void update(Time t, Time dt);
	
	// -- observable state
	const PhysicsState& state() const { return current_; }
	
	// -- shared semi-constant state
	void setMass(float mass) { current_.setMass(mass); }
	void setAngularInertia(float angularInertia) { current_.setAngularInertia(angularInertia); }

	// -- behaviour-generated force
	math::Vec3 userForce;
	math::Vec3 userTorque;
	void addForce(const math::Vec3& force) { userForce += force; }
	void addTorque(const math::Vec3& torque) { userTorque += torque; }
	
	// -- integration
	void calcForces(const PhysicsState&, const Time globalTime, math::Vec3& outForce, math::Vec3& outTorque) const;
};


} // ns physics
} // ns stardazed

#endif
