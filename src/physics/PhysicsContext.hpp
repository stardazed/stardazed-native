// ------------------------------------------------------------------
// physics::PhysicsContext - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_PHYSICSCONTEXT_H
#define SD_PHYSICS_PHYSICSCONTEXT_H

#include "system/Config.hpp"
#include "physics/RigidBody.hpp"

#include <vector>

namespace stardazed {
namespace physics {


class PhysicsContext {
	std::vector<physics::RigidBody> rigidBodyPool_;
	
public:
	PhysicsContext();
	
	RigidBody* makeRigidBody(Transform& linkedTransform, Mass, AngInertia);
	
	void integrateStep(GlobalTime t, GlobalTime dt);
	
	Environment environment {};
};


} // ns physics
} // ns stardazed

#endif
