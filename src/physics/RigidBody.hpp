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
	scene::TransformManager& transformMgr_;
	
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
		ValInv, // dragArea       ---- This is the drag co-efficient (Cd) * an average intersection area (A)
		ValInv, // angularDrag
	
		// external
		math::Vec3, // externalForce
	
		// primary
		scene::TransformManager::Instance, // linkedTransform
		math::Vec3, // momentum
	
		// secondary
		math::Vec3, // velocity

		math::Vec3, // previousPosition
		math::Vec3  // previousVelocity
	> instanceData_;
	
	HashMap<scene::Entity, Instance> entityMap_;
	
	enum class InstField : uint {
		Properties,

		Mass,
		DragArea,
		AngularDrag,
		
		ExternalForce,
		
		Transform,
		Momentum,

		Velocity,
		
		PreviousPosition,
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
	RigidBodyManager(memory::Allocator&, scene::TransformManager&);

	Instance create(scene::Entity, const RigidBodyDescriptor&);
	
	Instance forEntity(scene::Entity);

	// -- single instance access
	const Properties properties(Instance h) const { return *(instancePtr<InstField::Properties>(h)); }
	const ValInv mass(Instance h) const { return *(instancePtr<InstField::Mass>(h)); }

	const scene::TransformManager::Instance linkedTransform(Instance h) const { return *(instancePtr<InstField::Transform>(h)); }

	const math::Vec3& momentum(Instance h) const { return *(instancePtr<InstField::Momentum>(h)); }
	const math::Vec3& velocity(Instance h) const { return *(instancePtr<InstField::Velocity>(h)); }
	void setMomentum(Instance h, const math::Vec3& newMomentum) {
		*(instancePtr<InstField::Momentum>(h)) = newMomentum;
		*(instancePtr<InstField::Velocity>(h)) = newMomentum * mass(h).reciprocal;
	}

	const math::Vec3& previousPosition(Instance h) const { return *(instancePtr<InstField::PreviousPosition>(h)); }
	const math::Vec3& previousVelocity(Instance h) const { return *(instancePtr<InstField::PreviousVelocity>(h)); }

	void addExternalForce(Instance, const math::Vec3&);
	
	// TODO: to make full use of RK4, certain forces will be a function, so something like:
	// void addContinuousForce(Instance, Vec3(Transform, Instance, Time)* );
	// which will be called when needed to calculate a continuous time-based force

	// -- integration
	void integrateAll(Time dt);
};


} // ns physics
} // ns stardazed

#endif
