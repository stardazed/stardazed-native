// ------------------------------------------------------------------
// scene::Transform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_TRANSFORM_H
#define SD_SCENE_TRANSFORM_H

#include "math/Angle.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "math/Quaternion.hpp"

namespace stardazed {
namespace scene {


class Transform {
public:
	math::Vec3 position;
	math::Quat rotation;

	void lookAt(const math::Vec3& target, const math::Vec3& up = {0,1,0});

	void rotate(math::Angle overX, math::Angle overY, math::Angle overZ);
	void rotate(const math::Vec3& axis, math::Angle angle);
	void rotate(const math::Quat& q);
	
	void translate(const math::Vec3& translation);
	void translate(float x, float y, float z);

	void translateGlobal(const math::Vec3& globalTranslation);
	void translateGlobal(float gx, float gy, float gz);

	math::Mat4 toMatrix4() const;
};


} // ns scene
} // ns stardazed

#endif
