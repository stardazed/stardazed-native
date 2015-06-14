// ------------------------------------------------------------------
// physics::Integration.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/Integration.hpp"

namespace stardazed {
namespace physics {


void PhysicsState::recalcSecondaryValues() {
	velocity_ = momentum * oneOverMass_;
	angularVelocity_ = angularMomentum * oneOverAngularInertia_;
	math::normalizeInPlace(transform.rotation);
	spin_ = 0.5 * math::Quat{angularVelocity_.value, 0} * transform.rotation;
}


void PhysicsState::copyPrimaryAndSecondaryValuesFrom(const PhysicsState& other) {
	transform = other.transform;
	momentum = other.momentum;
	angularMomentum = other.angularMomentum;
	
	velocity_ = other.velocity_;
	angularVelocity_ = other.angularVelocity_;
	spin_ = other.spin_;
}



Derivative IntegrationStep::evaluate(const PhysicsState& initial, const Time t) {
	Derivative output;
	output.velocity = initial.velocity();
	output.force = initial.calcForce(t);
	return output;
}


Derivative IntegrationStep::evaluate(const PhysicsState& initial, const Time t, const Time dt, const Derivative& d) {
	Transform tempTransform{ initial.transform };
	PhysicsState state{ tempTransform, initial.mass(), initial.angularInertia() };
	state.transform.position += d.velocity * dt;
	state.momentum += d.force * dt;
	state.recalcSecondaryValues();

	Derivative output;
	output.velocity = state.velocity();
	output.force = state.calcForce(t + dt);
	return output;
}


void IntegrationStep::integrate(PhysicsState& state, const Time t, const Time dt) {
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);
	
	Velocity3 dxdt = 1.0f/6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
	Force3 dpdt = 1.0f/6.0f * (a.force + 2.0f*(b.force + c.force) + d.force);
	
	state.transform.position += dxdt * dt;
	state.momentum += dpdt * dt;
}


} // ns physics
} // ns stardazed
