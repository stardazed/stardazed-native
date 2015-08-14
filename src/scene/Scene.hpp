// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENE_H
#define SD_SCENE_SCENE_H

#include "system/Config.hpp"
#include "scene/Entity.hpp"
#include "scene/Transform.hpp"

namespace stardazed {
namespace scene {


class Scene {
	EntityManager entities_;
	TransformManager transform_;

public:
	Scene();
	
	Entity makeEntity();
	Entity makeEntity(const TransformDescriptor&);
	Entity makeEntity(const math::Vec3& pos, const math::Quat& rot = math::Quat::identity(), const math::Vec3& scale = math::Vec3::one());
	
	TransformManager& transform() { return transform_; }
};


} // ns scene
} // ns stardazed

#endif
