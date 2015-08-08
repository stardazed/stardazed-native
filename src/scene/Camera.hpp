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
#include "scene/Transform.hpp"

namespace stardazed {
namespace scene {


class Camera {
	TransformComponent& transformComp_;
	TransformComponent::Instance transform_;
	uint32 pixelWidth_, pixelHeight_;

	math::Angle fov_ = math::Degrees{90};
	float aspect_ = 1, zNear_ = 0, zFar_ = 1;

	math::Mat4 projection_;

public:
	Camera(TransformComponent&, TransformComponent::Instance, uint32 pixelWidth, uint32 pixelHeight);

	auto transformInstance() const { return transform_; }

	// -- projection
	const math::Mat4& projectionMatrix() const { return projection_; }
	void perspective(math::Angle fov, float aspect, float zNear, float zFar);
	void perspective(math::Angle fov, float zNear, float zFar);

	void setFieldOfView(math::Angle fov);

	// -- view
	math::Mat4 viewMatrix() const;

	// -- observers
	uint32 viewPortWidth() const { return pixelWidth_; }
	uint32 viewPortHeight() const { return pixelHeight_; }
	
};


} // ns scene
} // ns stardazed

#endif
