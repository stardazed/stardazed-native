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
	scene::TransformComponent& transformMgr_;

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
	
		// per-frame applied forces
		math::Vec3, // currentForce
		math::Vec3, // currentTorque
	
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
		
		CurrentForce,
		CurrentTorque,
		
		PreviousPosition,
		PreviousRotation,
		PreviousVelocity
	};
	
	template <InstField F>
	auto basePtr() const {
		return instanceData_.template elementsBasePtr<(uint)F>();
	}
	
	template <InstField F>
	auto instancePtr(Instance h) const {
		return basePtr<F>() + h.ref;
	}

public:
	RigidBodyManager(memory::Allocator&, scene::TransformComponent&);

	Instance create(scene::Entity, float mass, float angularInertia);
	
	// -- single instance access
	float mass(Instance h) const { return *(instancePtr<InstField::Mass>(h)); }
	float inverseMass(Instance h) const { return *(instancePtr<InstField::InverseMass>(h)); }
	float angularInertia(Instance h) const { return *(instancePtr<InstField::AngularInertia>(h)); }
	float inverseAngInertia(Instance h) const { return *(instancePtr<InstField::InverseAngInertia>(h)); }
	
	scene::TransformComponent::Instance linkedTransform(Instance h) const { return *(instancePtr<InstField::Transform>(h)); }
	math::Vec3& momentum(Instance h) { return *(instancePtr<InstField::Momentum>(h)); }
	math::Vec3& angularMomentum(Instance h) { return *(instancePtr<InstField::AngularMomentum>(h)); }
	
	math::Vec3& velocity(Instance h) { return *(instancePtr<InstField::Velocity>(h)); }
	math::Quat& spin(Instance h) { return *(instancePtr<InstField::Spin>(h)); }
	math::Vec3& angularVelocity(Instance h) { return *(instancePtr<InstField::AngularVelocity>(h)); }
	
	void addForce(Instance, const math::Vec3&);
	void addTorque(Instance, const math::Vec3&);
	void recalcSecondaries(Instance);
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
