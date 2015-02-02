// ------------------------------------------------------------------
// scene::Camera - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_CAMERA_H
#define SD_SCENE_CAMERA_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "scene/Entity.hpp"

namespace stardazed {
namespace scene {


class Camera {
	Entity& entity_;
	math::Mat4 projection_;

public:
	Camera(Entity& linkedEntity);
	
	// -- observers
	Entity& entity() { return entity_; }
	const Entity& entity() const { return entity_; }

	Transform& transform() { return entity_.transform; }
	const Transform& transform() const { return entity_.transform; }

	// -- projection
	const math::Mat4& projectionMatrix() const { return projection_; }
	void perspective(math::Angle fovy, float aspect, float zNear, float zFar);
	void perspective(math::Angle fovy, size_t pixelWidth, size_t pixelHeight, float zNear, float zFar);
	
	// -- view
	math::Mat4 viewMatrix() const;
};


} // ns scene
} // ns stardazed

#endif
