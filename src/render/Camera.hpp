// ------------------------------------------------------------------
// render::Camera - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_CAMERA_H
#define SD_RENDER_CAMERA_H

#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "math/Quaternion.hpp"

namespace stardazed {
namespace render {


enum class BufferClearing {
	None,
	Colour,
	Depth,
	ColourAndDepth
};





class Camera {
public:
	math::Vec3 worldPosition, worldTarget, upNormal;
	
	math::Mat4 viewMatrix() {
		return math::lookAt(worldPosition, worldTarget, upNormal);
	}
};


	
} // ns render
} // ns stardazed

#endif
