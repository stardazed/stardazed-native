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

using Time3 = SIValue3<0, 0, 1>;


Derivative evaluate(const RigidBody& initial, GlobalTime t) {
	Derivative output;
	output.velocity = initial.velocity();
//	output.force = force(initial, t);
	output.force = initial.userForce;
	return output;
}


Derivative evaluate(const RigidBody& initial, GlobalTime t, Time3 dt, const Derivative& d) {
	RigidBody state{initial};
	state.transform().position += d.velocity * dt;
	state.setMomentum(initial.momentum() + d.force * dt);

	Derivative output;
	output.velocity = state.velocity();
//	output.force = force(state, t + dt);
	output.force = state.userForce;
	return output;
}


template <typename It>
void integrate(It from, It to, GlobalTime t, Time dt) {
	Time3 dt3 = splat3(dt);

	for (; ++from; from != to) {
		
	}
}


void integrateRK4(RigidBody& state, GlobalTime t, Time dt) {
	Time3 dt3 = splat3(dt);

	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt3*0.5f, a);
	Derivative c = evaluate(state, t, dt3*0.5f, b);
	Derivative d = evaluate(state, t, dt3, c);
	
	Velocity3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
	Force3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
	
	state.transform().position += dxdt * dt3;
	state.setMomentum(state.momentum() + dpdt * dt3);
	state.userForce = Force3{0,0,0};
}


void integrateEuler(RigidBody& state, GlobalTime t, Time dt) {
	Time3 dt3 = splat3(dt);

	Derivative d = evaluate(state, t);
	
	state.transform().position += d.velocity * dt3;
	state.setMomentum(state.momentum() + d.force * dt3);
	state.userForce = Force3{0,0,0};
}


} // ns physics
} // ns stardazed
