// ------------------------------------------------------------------
// physics::Integration - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_INTEGRATION_H
#define SD_PHYSICS_INTEGRATION_H

#include "system/Config.hpp"
#include "physics/GlobalTime.hpp"
#include "physics/RigidBody.hpp"

namespace stardazed {
namespace physics {


void integrateRK4(RigidBody&, GlobalTime t, Time dt);
void integrateEuler(RigidBody&, GlobalTime t, Time dt);


} // ns physics
} // ns stardazed

#endif
