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


struct RigidBodyDescriptor {
	float mass, drag, angularDrag;
	bool obeysGravity;
};


class RigidBodyManager {
public:
	using Instance = scene::Instance<RigidBodyManager>;

private:
	scene::TransformComponent& transformMgr_;
	
	struct Properties {
		bool8 awake : 1;
		bool8 gravity : 1;
	};
	
	struct ValInv {
		float value;
		float reciprocal;
	};

	container::MultiArrayBuffer<
		// packed flags and properties
		Properties,

		// constant
		ValInv, // mass/inertia
		ValInv, // drag           ---- This is the drag co-efficient (Cd) * an average intersection area (A)
		ValInv, // angularDrag

		// secondary
		math::Vec3, // velocity
		math::Vec3, // angularVelocity
		math::Quat, // spin

		// primary
		scene::TransformComponent::Instance, // linkedTransform
	
		// per-frame applied forces
		math::Vec3, // externalForce
		math::Vec3, // externalTorque
	
		// previous state
		math::Vec3, // previousPosition
		math::Quat, // previousRotation
		math::Vec3, // previousAcceleration
		math::Vec3  // previousVelocity
	> instanceData_;
	
	HashMap<scene::Entity, Instance> entityMap_;
	
	enum class InstField : uint {
		Properties,

		Mass,
		Drag,
		AngularDrag,
		
		Velocity,
		AngularVelocity,
		Spin,
		
		Transform,
		
		ExternalForce,
		ExternalTorque,
		
		PreviousPosition,
		PreviousRotation,
		PreviousAcceleration,
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

	Instance create(scene::Entity, const RigidBodyDescriptor&);
	
	Instance forEntity(scene::Entity);

	// -- single instance access
	const Properties properties(Instance h) const { return *(instancePtr<InstField::Properties>(h)); }
	const ValInv mass(Instance h) const { return *(instancePtr<InstField::Mass>(h)); }
	const ValInv drag(Instance h) const { return *(instancePtr<InstField::Drag>(h)); }
	const ValInv angularDrag(Instance h) const { return *(instancePtr<InstField::AngularDrag>(h)); }

	const scene::TransformComponent::Instance linkedTransform(Instance h) const { return *(instancePtr<InstField::Transform>(h)); }

	const math::Vec3& velocity(Instance h) const { return *(instancePtr<InstField::Velocity>(h)); }
	void setVelocity(Instance, const math::Vec3&);
	const math::Quat& spin(Instance h) const { return *(instancePtr<InstField::Spin>(h)); }
	const math::Vec3& angularVelocity(Instance h) const { return *(instancePtr<InstField::AngularVelocity>(h)); }
	
	const math::Vec3& previousPosition(Instance h) const { return *(instancePtr<InstField::PreviousPosition>(h)); }
	const math::Vec3& previousAcceleration(Instance h) const { return *(instancePtr<InstField::PreviousAcceleration>(h)); }
	const math::Vec3& previousVelocity(Instance h) const { return *(instancePtr<InstField::PreviousVelocity>(h)); }

	void addForce(Instance, const math::Vec3&);
	void addTorque(Instance, const math::Vec3&);

//	void recalcSecondaries(Instance);
	void integrateAll(Time dt);
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
