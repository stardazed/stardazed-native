// ------------------------------------------------------------------
// physics::PhysicsContext - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_PHYSICSCONTEXT_H
#define SD_PHYSICS_PHYSICSCONTEXT_H

#include "system/Config.hpp"
#include "physics/Integration.hpp"

#include <vector>

namespace stardazed {
namespace physics {


class PhysicsContext {
	std::vector<physics::RigidBody> rigidBodyPool_;
	GlobalTime baseTime_;
	
public:
	PhysicsContext();
	
	RigidBody* makeRigidBody(Transform& linkedTransform, Mass mass);
	
	void integrateStep(GlobalTime t, GlobalTime dt);
	
	Environment environment {};
};


} // ns physics
} // ns stardazed

#endif
