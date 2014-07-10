// ------------------------------------------------------------------
// scene::Camera - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_CAMERA_H
#define SD_SCENE_CAMERA_H

#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "math/Quaternion.hpp"

namespace stardazed {
namespace scene {


class Camera {
public:
	math::Vec3 worldPosition, worldTarget, upNormal;
	
	math::Mat4 viewMatrix() {
		return math::lookAt(worldPosition, worldTarget, upNormal);
	}
};


} // ns scene
} // ns stardazed

#endif
