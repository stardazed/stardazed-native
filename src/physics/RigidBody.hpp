// ------------------------------------------------------------------
// physics::RigidBody - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_RIGIDBODY_H
#define SD_PHYSICS_RIGIDBODY_H

#include "system/Config.hpp"
#include "system/Time.hpp"
#include "container/MultiArrayBuffer.hpp"
#include "container/HashMap.hpp"
#include "scene/Entity.hpp"
#include "scene/Transform.hpp"

namespace stardazed {
namespace physics {


class RigidBodyManager {
public:
	using Instance = scene::Instance<RigidBodyManager>;

private:
	scene::TransformComponent& transform_;

	container::MultiArrayBuffer<
		// constant
		float, // mass / inertia
		float, // inverseMass
		float, // angularInertia
		float, // inverseAngInertia

		// secondary
		math::Vec3, // velocity
		math::Vec3, // angularVelocity
		math::Quat, // spin

		// primary
		scene::TransformComponent::Instance, // linkedTransformInstance
		math::Vec3, // momentum
		math::Vec3, // angularMomentum
	
		// previous state
		math::Vec3, // previousPosition
		math::Quat, // previousRotation
		math::Vec3  // previousVelocity
	> instanceData_;
	
	HashMap<scene::Entity, Instance> entityMap_;
	
	enum class InstField : uint {
		Mass,
		InverseMass,
		AngularInertia,
		InverseAngInertia,
		
		Velocity,
		AngularVelocity,
		Spin,
		
		Transform,
		Momentum,
		AngularMomentum,
		
		PreviousPosition,
		PreviousRotation,
		PreviousVelocity
	};
	
	template <InstField F>
	auto basePtr() const {
		return instanceData_.template elementsBasePtr<(uint)F>();
	}

public:
	RigidBodyManager(memory::Allocator&, scene::TransformComponent&);

	Instance create(scene::Entity, float mass, float angularInertia);
	
	// -- single instance access
	float mass(Instance h) const { return *(basePtr<InstField::Mass>() + h.ref); }
	float angularInertia(Instance h) const { return *(basePtr<InstField::AngularInertia>() + h.ref); }
	
	scene::TransformComponent::Instance transform(Instance h) const { return *(basePtr<InstField::Transform>() + h.ref); }
	math::Vec3& momentum(Instance h) { return *(basePtr<InstField::Momentum>() + h.ref); }
	math::Vec3& angularMomentum(Instance h) { return *(basePtr<InstField::AngularMomentum>() + h.ref); }
	
	const math::Vec3& velocity(Instance h) const { return *(basePtr<InstField::Velocity>() + h.ref); }
	const math::Quat& spin(Instance h) const { return *(basePtr<InstField::Spin>() + h.ref); }
	const math::Vec3& angularVelocity(Instance h) const { return *(basePtr<InstField::AngularVelocity>() + h.ref); }
};


/*
class RigidBody : public Integratable<RigidBody> {
	Transform previousTransform_;
	PhysicsState previous_, current_;
	
public:
	RigidBody(Transform& linkedTransform, float mass, float angInertia);
	
	void update(Time t, Time dt);
	
	// -- observable state
	const PhysicsState& previousState() const { return previous_; }
	PhysicsState& state() { return current_; }
	const PhysicsState& state() const { return current_; }

	// -- shared semi-constant state
	void setMass(float mass) { current_.setMass(mass); }
	void setAngularInertia(float angularInertia) { current_.setAngularInertia(angularInertia); }

	// -- behaviour-generated forces
	math::Vec3 userForce;
	math::Vec3 userTorque;
	void addForce(const math::Vec3& force) { userForce += force; }
	void addTorque(const math::Vec3& torque) { userTorque += torque; }
	
	// -- integration
	void calcForces(const PhysicsState&, const Time globalTime, math::Vec3& outForce, math::Vec3& outTorque) const;
};
*/


} // ns physics
} // ns stardazed

#endif
