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


struct Environment {
	Acceleration3 gravity = earthGravity();
	MassDensity airDensity = earthSeaLevelAirDensity();
};


struct Derivative;


class IntegrationStep {
	Environment environment_;
	Time t_, dt_;

	Derivative evaluate(const RigidBody&);
	Derivative evaluate(const RigidBody&, const Time&, const Derivative&);

	void integrateRK4(RigidBody& state);
	void integrateEuler(RigidBody& state);

public:
	IntegrationStep(Environment, GlobalTime, Time dt);

	template <typename It>
	void integrateRange(It from, It to) {
		for (; from != to; ++from) {
			integrateRK4(*from);
			from->userForce = Force3{0,0,0}; // FIXME: this var is the comms between Behaviour and RigidBody
		}
	}
};


} // ns physics
} // ns stardazed

#endif
