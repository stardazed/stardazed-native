// ------------------------------------------------------------------
// physics::RK4Integrator - stardazed
// (c) 2015 by Arthur Langereis
// Based on code by Glenn Fiedler (gafferongames.com)
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_RK4INTEGRATOR_H
#define SD_PHYSICS_RK4INTEGRATOR_H

#include "system/Config.hpp"
#include "system/Logging.hpp"
#include "math/Vector.hpp"

namespace stardazed {
namespace physics {


// Runge-Kutta integrator, but right now only in name as all forces
// applied are constant over dt so it is basically an Euler integrator


struct RK4Derivative {
	math::Vec3 velocity;
	math::Vec3 force;
};


struct RK4State {
	// primary
	float inverseMass;
	math::Vec3 position;
	math::Vec3 momentum;

	// secondary
	math::Vec3 velocity;
	
	void recalcSecondaryValues() {
		velocity = momentum * inverseMass;
	}
};


class RK4Integrator {
	RK4Derivative evaluate(const RK4State& initial, const math::Vec3& constantForce) {
		return {
			initial.velocity,
			constantForce
		};
	}
	
	RK4Derivative evaluate(const RK4State& initial, const math::Vec3& constantForce, const Time dt, const RK4Derivative& derivative) {
		RK4State state{};
		state.inverseMass = initial.inverseMass;
		state.position = initial.position + derivative.velocity * dt;
		state.momentum = initial.momentum + derivative.force * dt;
		state.recalcSecondaryValues();
		
		return {
			state.velocity,
			constantForce
		};
	}
	
public:
	void integrate(RK4State& state, const math::Vec3& constantForce, const Time dt) {
		auto a = evaluate(state, constantForce);
		auto b = evaluate(state, constantForce, dt*0.5f, a);
		auto c = evaluate(state, constantForce, dt*0.5f, b);
		auto d = evaluate(state, constantForce, dt, c);
		
		math::Vec3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
		math::Vec3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
		
		state.position += dxdt * dt;
		state.momentum += dpdt * dt;
		state.recalcSecondaryValues();
	}
};



// Euler integrator

class EulerIntegrator {
public:
	void integrate(RK4State& state, const math::Vec3& constantForce, const Time dt) {
		state.position += state.velocity * dt;
		state.momentum += constantForce * dt;
		state.recalcSecondaryValues();
	}
};




/*
---- version with rotation from old sources

struct Derivative {
	math::Vec3 velocity;
	math::Vec3 force;

	math::Quat spin;
	math::Vec3 torque;
};


template <typename Impl>
class Integratable {
	Derivative evaluate(const PhysicsState& initial, const Time t) {
		Derivative output;
		output.velocity = initial.velocity();
		output.spin = initial.spin();
		static_cast<Impl*>(this)->calcForces(initial, t, output.force, output.torque);
		return output;
	}

	Derivative evaluate(const PhysicsState& initial, const Time t, const Time dt, const Derivative& derivative) {
		Transform tempTransform{ initial.transform };
		PhysicsState state{ tempTransform, initial.mass(), initial.angularInertia() };
		state.transform.position += derivative.velocity * dt;
		state.momentum += derivative.force * dt;
		state.transform.rotation += derivative.spin * dt;
		state.angularMomentum += derivative.torque * dt;
		state.recalcSecondaryValues();

		Derivative output;
		output.velocity = state.velocity();
		output.spin = state.spin();
		static_cast<Impl*>(this)->calcForces(initial, t + dt, output.force, output.torque);
		return output;
	}

public:
	void integrate(PhysicsState& state, const Time t, const Time dt) {
		Derivative a = evaluate(state, t);
		Derivative b = evaluate(state, t, dt*0.5f, a);
		Derivative c = evaluate(state, t, dt*0.5f, b);
		Derivative d = evaluate(state, t, dt, c);
		
		math::Vec3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
		math::Vec3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
		math::Quat drdt = 1.0f/6.0f * (a.spin + 2.0f*(b.spin + c.spin) + d.spin);
		math::Vec3 dTdt = 1.0f/6.0f * (a.torque + 2.0f*(b.torque + c.torque) + d.torque);

		state.transform.position += dxdt * dt;
		state.momentum += dpdt * dt;
		state.transform.rotation += drdt * dt;
		state.angularMomentum += dTdt * dt;

		state.recalcSecondaryValues();
	}
};
*/

} // ns physics
} // ns stardazed

#endif
