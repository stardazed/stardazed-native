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


using Time3 = SIValue3<0, 0, 1>;


void integrateRK4(RigidBody& state, GlobalTime t, Time3 dt3);
void integrateEuler(RigidBody& state, GlobalTime t, Time3 dt3);


template <typename It>
void integrate(It from, It to, GlobalTime t, Time dt) {
	auto dt3 = splat3(dt);

	for (; from != to; ++from) {
		integrateRK4(*from, t, dt3);
	}
}


} // ns physics
} // ns stardazed

#endif
