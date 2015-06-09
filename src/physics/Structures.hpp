// ------------------------------------------------------------------
// physics::Structures - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_STRUCTURES_H
#define SD_PHYSICS_STRUCTURES_H

#include "system/Config.hpp"
#include "physics/Units.hpp"

namespace stardazed {
namespace physics {


constexpr Acceleration3 earthGravity() { return { 0, -9.80665, 0 }; }
constexpr Acceleration3 moonGravity() { return { 0, -1.62519, 0 }; }


struct WorldPhysics {
	Acceleration3 gravity = earthGravity();
	float linearDragCoefficient = 1.5;
};


struct Derivative {
	Velocity3 velocity;
	Force3 force;
};


} // ns physics
} // ns stardazed

#endif
