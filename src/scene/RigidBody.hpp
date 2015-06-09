// ------------------------------------------------------------------
// scene::RigidBody - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_RIGIDBODY_H
#define SD_SCENE_RIGIDBODY_H

#include "system/Config.hpp"
#include "scene/Transform.hpp"
#include "physics/Units.hpp"

namespace stardazed {
namespace scene {


class RigidBody {
	physics::Mass mass_{1}, oneOverMass_{1};
	physics::Momentum3 momentum_ {};
	
public:
	RigidBody() = default;
	
	physics::Mass mass() const { return mass_; }
	void setMass(physics::Mass mass) {
		mass_ = mass;
		oneOverMass_ = 1 / mass_;
	}
};
	

} // ns scene
} // ns stardazed

#endif
