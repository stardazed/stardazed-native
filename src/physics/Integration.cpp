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


Derivative evaluate(const RigidBody& initial, Time3 t) {
	Derivative output;
	output.velocity = initial.velocity();
//	output.force = force(initial, t);
	return output;
}


Derivative evaluate(const RigidBody& initial, Time3 t, Time3 dt, const Derivative& d) {
	RigidBody state{initial};
	state.transform().position += d.velocity * dt;
	state.setMomentum(initial.momentum() + d.force * dt);

	Derivative output;
	output.velocity = state.velocity();
//	output.force = force(state, t + dt);
	return output;
}


void integrateRK4(RigidBody& state, Time t, Time dt) {
	Time3 t3 = splat<3>(t);
	Time3 dt3 = splat<3>(dt);

	Derivative a = evaluate(state, t3);
	Derivative b = evaluate(state, t3, dt3*0.5f, a);
	Derivative c = evaluate(state, t3, dt3*0.5f, b);
	Derivative d = evaluate(state, t3, dt3, c);
	
	Velocity3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
	Force3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
	
	state.transform().position += dxdt * dt3;
	state.setMomentum(state.momentum() + dpdt * dt3);
}


void integrateEuler(RigidBody& state, Time t, Time dt) {
	Time3 t3 = splat<3>(t);
	Time3 dt3 = splat<3>(dt);

	Derivative d = evaluate(state, t3);
	
	state.transform().position += d.velocity * dt3;
	state.setMomentum(state.momentum() + d.force * dt3);
}


} // ns physics
} // ns stardazed
