// ------------------------------------------------------------------
// physics::Integration.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/Integration.hpp"

namespace stardazed {
namespace physics {


struct Derivative {
	Velocity3 velocity;
	Force3 force;
};


IntegrationStep::IntegrationStep(Environment env, GlobalTime t, Time dt)
: environment_(env)
, t3_{splat3(Time{t})}
, dt3_{splat3(dt)}
{}


Derivative IntegrationStep::evaluate(const RigidBody& initial) {
	Derivative output;
	output.velocity = initial.velocity();
	output.force = initial.calcForce(environment_, t3_);
	return output;
}


Derivative IntegrationStep::evaluate(const RigidBody& initial, const Time3& dt3, const Derivative& d) {
	Transform next{ initial.transform() };
	RigidBody state{initial};
	state.useTransform(next); // FIMXE: this setup kinda sucks
	state.transform().position += d.velocity * dt3;
	state.setMomentum(initial.momentum() + d.force * dt3);

	Derivative output;
	output.velocity = state.velocity();
	output.force = state.calcForce(environment_, t3_ + dt3_);
	return output;
}


void IntegrationStep::integrateRK4(RigidBody& state) {
	Derivative a = evaluate(state);
	Derivative b = evaluate(state, dt3_*0.5f, a);
	Derivative c = evaluate(state, dt3_*0.5f, b);
	Derivative d = evaluate(state, dt3_, c);
	
	Velocity3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
	Force3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
	
	state.transform().position += dxdt * dt3_;
	state.setMomentum(state.momentum() + dpdt * dt3_);
}


void IntegrationStep::integrateEuler(RigidBody& state) {
	Derivative d = evaluate(state);
	
	state.transform().position += d.velocity * dt3_;
	state.setMomentum(state.momentum() + d.force * dt3_);
}


} // ns physics
} // ns stardazed
