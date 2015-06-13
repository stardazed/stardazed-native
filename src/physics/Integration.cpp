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
, t_{Time{t}}
, dt_{dt}
{}


Derivative IntegrationStep::evaluate(const RigidBody& initial) {
	Derivative output;
	output.velocity = initial.velocity();
	output.force = initial.calcForce(environment_, t_);
	return output;
}


Derivative IntegrationStep::evaluate(const RigidBody& initial, const Time& dt, const Derivative& d) {
	Transform next{ initial.transform() };
	RigidBody state{initial};
	state.useTransform(next); // FIMXE: this setup kinda sucks
	state.transform().position += d.velocity * dt;
	state.setMomentum(initial.momentum() + d.force * dt);

	Derivative output;
	output.velocity = state.velocity();
	output.force = state.calcForce(environment_, t_ + dt);
	return output;
}


void IntegrationStep::integrateRK4(RigidBody& state) {
	Derivative a = evaluate(state);
	Derivative b = evaluate(state, dt_*0.5f, a);
	Derivative c = evaluate(state, dt_*0.5f, b);
	Derivative d = evaluate(state, dt_, c);
	
	Velocity3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
	Force3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
	
	state.transform().position += dxdt * dt_;
	state.setMomentum(state.momentum() + dpdt * dt_);
}


void IntegrationStep::integrateEuler(RigidBody& state) {
	Derivative d = evaluate(state);
	
	state.transform().position += d.velocity * dt_;
	state.setMomentum(state.momentum() + d.force * dt_);
}


} // ns physics
} // ns stardazed
