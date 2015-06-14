// ------------------------------------------------------------------
// physics::RigidBody - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_RIGIDBODY_H
#define SD_PHYSICS_RIGIDBODY_H

#include "system/Config.hpp"
#include "physics/Integration.hpp"

namespace stardazed {
namespace physics {


// defined in physics/Integration.hpp, which already includes this file
struct Environment;
class IntegrationStep;


class RigidBody {
	Transform previousTransform_;
	PhysicsState previous_, current_;
	
public:
	RigidBody(Transform& linkedTransform, const Mass, const AngInertia);
	
	void update(const IntegrationStep&);
	
	// -- observable state
	const PhysicsState& state() const { return current_; }
	
	// -- shared semi-constant state
	void setMass(const Mass);
	void setAngularInertia(const AngInertia);

	// -- behaviour-generated force
	Force3 userForce;
	void addForce(const Force3& force) {
		userForce += force;
	}
	
	// -- integration
	Force3 calcForce(const Environment&, const Time& globalTime) const;
};


} // ns physics
} // ns stardazed

#endif
