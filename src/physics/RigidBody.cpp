// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"
#include "system/Logging.hpp"
#include "physics/RK4Integrator.hpp"

namespace stardazed {
namespace physics {


RigidBodyManager::RigidBodyManager(memory::Allocator& allocator, scene::TransformManager& transform)
: transformMgr_(transform)
, instanceData_{ allocator, 1024 }
, entityMap_{ allocator, 1024 }
{
	instanceData_.extend();  // index 0 is a null-instance
}


auto RigidBodyManager::create(scene::Entity entity, const RigidBodyDescriptor& desc) -> Instance {
	instanceData_.extend();
	uint index = instanceData_.count() - 1;
	
	// FIXME: calc average drag intersection area, right now A = 1
	
	auto trans = transformMgr_.forEntity(entity);

	*(basePtr<InstField::Properties>() + index) = { true, desc.obeysGravity };
	*(basePtr<InstField::Mass>() + index) = { desc.mass, 1.0f / desc.mass };
	*(basePtr<InstField::DragArea>() + index) = { desc.drag, 1.0f / desc.drag };
	*(basePtr<InstField::AngularDrag>() + index) = { desc.angularDrag, 1.0f / desc.angularDrag };
	*(basePtr<InstField::Transform>() + index) = trans;
	*(basePtr<InstField::PreviousPosition>() + index) = transformMgr_.position(trans);

	Instance h { index };
	entityMap_.insert(entity, h);
	return h;
}


auto RigidBodyManager::forEntity(scene::Entity ent) -> Instance {
	Instance* result = entityMap_.find(ent);
	return result ? *result : Instance{0};
}


void RigidBodyManager::addExternalForce(Instance h, const math::Vec3& force) {
	auto fp = instancePtr<InstField::ExternalForce>(h);
	*fp += force;
}


void RigidBodyManager::integrateAll(Time dt) {
	auto massBase = basePtr<InstField::Mass>();
	auto dragAreaBase = basePtr<InstField::DragArea>();
	auto propertiesBase = basePtr<InstField::Properties>();
	auto transformBase = basePtr<InstField::Transform>();
	auto externalForceBase = basePtr<InstField::ExternalForce>();
	auto momentumBase = basePtr<InstField::Momentum>();
	auto velocityBase = basePtr<InstField::Velocity>();
	auto previousPositionBase = basePtr<InstField::PreviousPosition>();
	auto previousVelocityBase = basePtr<InstField::PreviousVelocity>();

	using namespace math;

	const Vec3 gravityAccel { 0, -9.80665, 0 };
	EulerIntegrator integrator;

	for (uint rbi=1, rbCount = instanceData_.count(); rbi < rbCount; ++rbi) {
		auto properties = propertiesBase[rbi];
		auto dragArea = dragAreaBase[rbi].value;
		auto transform = transformBase[rbi];
		auto totalForce = externalForceBase[rbi];
		auto velocity = velocityBase[rbi];

		// -- apply constant forces: gravity and air drag
		if (properties.gravity)
			totalForce += gravityAccel * massBase[rbi].value;
		
		if (! (nearEqual(0.0f, lengthSquared(velocity)) || nearEqual(0.0f, dragArea)) ) {
			// 1.2f is air drag (rho) at 15C at 0m elevation
			auto signedDirection = math::sign(velocity);
			totalForce -= .5f * 1.2f * dragArea * velocity * velocity * signedDirection;
			
			// -- apply friction
			totalForce -= .5f * momentumBase[rbi];
		}
		
		// -- update position and keep old position (for collision tests)
		RK4State state {
			massBase[rbi].reciprocal,
			transformMgr_.position(transform),
			momentumBase[rbi],
			velocity
		};

		previousPositionBase[rbi] = state.position;
		previousVelocityBase[rbi] = velocity;
		integrator.integrate(state, totalForce, dt);
		transformMgr_.setPosition(transform, state.position);
		momentumBase[rbi] = state.momentum;
		velocityBase[rbi] = state.velocity;
	}
	
	// clear all external forces (FIXME: make this a MAB method)
	memset(basePtr<InstField::ExternalForce>(), 0, instanceData_.count() * sizeof(Vec3));
}



} // ns physics
} // ns stardazed
