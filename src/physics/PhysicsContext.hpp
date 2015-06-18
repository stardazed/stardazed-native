// ------------------------------------------------------------------
// physics::PhysicsContext - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_PHYSICSCONTEXT_H
#define SD_PHYSICS_PHYSICSCONTEXT_H

#include "system/Config.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Collider.hpp"

#include <vector>
#include <memory>

namespace stardazed {
namespace physics {


class PhysicsContext {
	std::vector<RigidBody> rigidBodyPool_;
	std::vector<std::unique_ptr<Collider>> colliderPool_;
	
public:
	PhysicsContext();

	// --
	
	RigidBody* makeRigidBody(Transform& linkedTransform, float mass, float angInertia);

	template <typename C, typename... Args> // C : public Collider
	C* makeCollider(Args... args) {
		auto c = new C(std::forward<Args>(args)...);
		colliderPool_.emplace_back(c);
		return c;
	}

	// --

	void integrateStep(Time t, Time dt);

	// --

	Environment environment {};
};


} // ns physics
} // ns stardazed

#endif
