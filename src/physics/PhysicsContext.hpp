// ------------------------------------------------------------------
// physics::PhysicsContext - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_PHYSICSCONTEXT_H
#define SD_PHYSICS_PHYSICSCONTEXT_H

#include "system/Config.hpp"
#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


class PhysicsContext {
	
public:
	PhysicsContext();

	// --
	
	RigidBody* makeRigidBody(Transform& linkedTransform, float mass, float angInertia);

	// --

	void integrateStep(Time t, Time dt);

	// --

	Environment environment {};
};


} // ns physics
} // ns stardazed

#endif
