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
	
	// -- observers
	Node& node() { return *node_; }
	const Node& node() const { return *node_; }

	Transform& transform() { return node_->transform; }
	const Transform& transform() const { return node_->transform; }

	// -- projection
	const math::Mat4& projectionMatrix() const { return projection_; }
	void perspective(math::Angle fovy, float aspect, float zNear, float zFar);
	void perspective(math::Angle fovy, size_t pixelWidth, size_t pixelHeight, float zNear, float zFar);
	
	// -- view
	const math::Mat4 viewMatrix() const;
};


} // ns scene
} // ns stardazed

#endif
