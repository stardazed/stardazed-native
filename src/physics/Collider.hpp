// ------------------------------------------------------------------
// physics::Collider - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_COLLIDER_H
#define SD_PHYSICS_COLLIDER_H

#include "system/Config.hpp"
#include "math/Bounds.hpp"
#include "container/MultiArrayBuffer.hpp"
#include "container/HashMap.hpp"
#include "physics/RigidBody.hpp"
#include "scene/Transform.hpp"
#include "scene/Entity.hpp"

namespace stardazed {
namespace physics {


enum class ColliderType : uint8 {
	Box,
	Sphere
};


class ColliderManager {
public:
	using Instance = scene::Instance<ColliderManager>;

private:
	scene::TransformComponent& transformMgr_;
	RigidBodyManager& rigidBodyMgr_;

	container::MultiArrayBuffer<
		ColliderType,
		scene::TransformComponent::Instance,
		RigidBodyManager::Instance,
		math::Bounds, // localBounds
		math::Bounds  // worldBounds
	> instanceData_;
	
	enum class InstField {
		Type,
		Transform,
		RigidBody,
		LocalBounds,
		WorldBounds
	};
	
	HashMap<scene::Entity, Instance> entityMap_;
	
	template <InstField F>
	auto basePtr() const {
		return instanceData_.template elementsBasePtr<(uint)F>();
	}

public:
	ColliderManager(memory::Allocator&, scene::TransformComponent&, RigidBodyManager&);
	
	Instance create(scene::Entity, ColliderType, const math::Vec3& localCenter, const math::Vec3& size);

	void linkToRigidBody(Instance, RigidBodyManager::Instance);
	RigidBodyManager::Instance linkedRigidBody(Instance) const;
	
	void resolveAll();
};


} // ns physics
} // ns stardazed

#endif
