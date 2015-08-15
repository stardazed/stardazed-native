// ------------------------------------------------------------------
// physics::RigidBody.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/RigidBody.hpp"
#include "system/Logging.hpp"

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
	*(basePtr<InstField::Drag>() + index) = { desc.drag, 1.0f / desc.drag };
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


void RigidBodyManager::addAcceleration(Instance h, const math::Vec3& accel) {
	auto ap = instancePtr<InstField::Acceleration>(h);
	*ap += accel;
}


void RigidBodyManager::accelerateAll(Time dt) {
	auto propertiesBase = basePtr<InstField::Properties>() + 1;
	auto transformBase = basePtr<InstField::Transform>() + 1;
	auto accelerationBase = basePtr<InstField::Acceleration>() + 1;

	using namespace math;

	const Vec3 gravityAccel { 0, -9.80665, 0 };

	for (uint rbi=1, rbCount = instanceData_.count(); rbi < rbCount; ++rbi) {
		auto properties = *propertiesBase;
		auto transform = *transformBase;
		auto acceleration = *accelerationBase;
		
		sd::log(acceleration);
		
		if (properties.gravity)
			acceleration += gravityAccel;
		
		// -- update position and keep old position (for collision tests)
		auto dX = acceleration * dt * dt;
		auto curPos = transformMgr_.position(transform);
		transformMgr_.setPosition(transform, curPos + dX);

		++propertiesBase;
		++transformBase;
		++accelerationBase;
	}
	
	// clear all accelerations
	memset(basePtr<InstField::Acceleration>(), 0, instanceData_.count() * sizeof(Vec3));
}


void RigidBodyManager::inertiaAll() {
	auto transformBase = basePtr<InstField::Transform>() + 1;
	auto previousPositionBase = basePtr<InstField::PreviousPosition>() + 1;
	
	using namespace math;
	
	for (uint rbi=1, rbCount = instanceData_.count(); rbi < rbCount; ++rbi) {
		auto transform = *transformBase;
		auto prevPos = *previousPositionBase;
		
		auto curPos = transformMgr_.position(transform);
		transformMgr_.setPosition(transform, (curPos * 2) - prevPos);
		*previousPositionBase = curPos;
		
		++transformBase;
		++previousPositionBase;
	}
}



} // ns physics
} // ns stardazed
