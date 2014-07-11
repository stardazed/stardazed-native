// ------------------------------------------------------------------
// scene::Camera - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_CAMERA_H
#define SD_SCENE_CAMERA_H

#include "math/Angle.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "scene/Node.hpp"

namespace stardazed {
namespace scene {


class Camera {
	Node* node_;
	math::Mat4 projection_;

public:
	Camera(Node& linkedNode);

	// -- projection
	const math::Mat4& projection() const { return projection_; }
	void perspective(math::Angle fovy, float aspect, float zNear, float zFar);
	void perspective(math::Angle fovy, size_t pixelWidth, size_t pixelHeight, float zNear, float zFar);
};


} // ns scene
} // ns stardazed

#endif
