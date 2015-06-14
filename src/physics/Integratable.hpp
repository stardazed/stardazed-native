// ------------------------------------------------------------------
// physics::Integratable - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_INTEGRATABLE_H
#define SD_PHYSICS_INTEGRATABLE_H

#include "system/Config.hpp"
#include "physics/PhysicsState.hpp"

namespace stardazed {
namespace physics {


struct Derivative {
	Velocity3 velocity;
	Force3 force;

	math::Quat spin;
	Torque3 torque;
};


template <typename Impl>
class Integratable {
	Derivative evaluate(const PhysicsState& initial, const Time t) {
		Derivative output;
		output.velocity = initial.velocity();
		static_cast<Impl*>(this)->calcForces(initial, t, output.force, output.torque);
		return output;
	}

	Derivative evaluate(const PhysicsState& initial, const Time t, const Time dt, const Derivative& d) {
		Transform tempTransform{ initial.transform };
		PhysicsState state{ tempTransform, initial.mass(), initial.angularInertia() };
		state.transform.position += d.velocity * dt;
		state.momentum += d.force * dt;
		state.recalcSecondaryValues();

		Derivative output;
		output.velocity = state.velocity();
		static_cast<Impl*>(this)->calcForces(initial, t + dt, output.force, output.torque);
		return output;
	}

public:
	void integrate(PhysicsState& state, const Time t, const Time dt) {
		Derivative a = evaluate(state, t);
		Derivative b = evaluate(state, t, dt*0.5f, a);
		Derivative c = evaluate(state, t, dt*0.5f, b);
		Derivative d = evaluate(state, t, dt, c);
		
		Velocity3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
		Force3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
		
		state.transform.position += dxdt * dt;
		state.momentum += dpdt * dt;
	}
};


} // ns physics
} // ns stardazed

#endif
