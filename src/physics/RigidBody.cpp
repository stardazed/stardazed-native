// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"
#include "physics/Integration.hpp"

namespace stardazed {
namespace physics {


RigidBody::RigidBody(Transform& linkedTransform, const Mass mass, const AngInertia angularInertia)
: previousTransform_(linkedTransform)
, previous_{ previousTransform_, mass, angularInertia }
, current_ { linkedTransform, mass, angularInertia }
{}


void RigidBody::update(const IntegrationStep& integrator) {
	previous_.copyPrimaryAndSecondaryValuesFrom(current_);
	integrator.
}


void RigidBody::setMass(const Mass mass) {
	current_.setMass(mass);
}


void RigidBody::setAngularInertia(const AngInertia angularInertia) {
	current_.setAngularInertia(angularInertia);
}


Force3 RigidBody::calcForce(const Environment& /* env */, const Time& /* globalTime */) const {
	return userForce - Force3{ state().momentum.value * .6125 };
}


} // ns physics
} // ns stardazed
