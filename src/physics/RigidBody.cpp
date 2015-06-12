// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


Force3 RigidBody::calcForce(const Environment& env, const Time3& globalTime) const {
	return userForce - Force3{ momentum().value * .6125 };
}


} // ns physics
} // ns stardazed
