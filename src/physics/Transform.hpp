// ------------------------------------------------------------------
// physics::Transform - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_TRANSFORM_H
#define SD_PHYSICS_TRANSFORM_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "math/Quaternion.hpp"
#include "physics/Units.hpp"

namespace stardazed {
namespace physics {


struct Transform {
	math::Vec3 position;
	math::Vec3 scale { 1, 1, 1 };
	math::Quat rotation;

	void lookAt(const math::Vec3& target, const math::Vec3& up = {0,1,0});

	void rotate(math::Angle overX, math::Angle overY, math::Angle overZ) {
		rotation *= math::Quat::fromEuler(overZ, overY, overX);
	}

	void rotate(const math::Vec3& axis, math::Angle angle) {
		rotation *= math::Quat::fromAxisAngle(axis, angle);
	}

	void rotate(const math::Quat& q) {
		rotation *= q;
	}
	
	void translate(const math::Vec3& translation) {
		float len = math::length(translation);
		if (math::nearEqual(len, 0.0f))
			return;
		position += (rotation * translation) * len;
	}

	void translate(float x, float y, float z) {
		translate(math::Vec3{ x, y, z });
	}

	void translateGlobal(const math::Vec3& globalTranslation) {
		position += globalTranslation;
	}

	void translateGlobal(float gx, float gy, float gz) {
		position += math::Vec3{ gx, gy, gz };
	}
	
	Transform apply(const Transform&) const;

	math::Mat4 toMatrix4() const;
};


} // ns physics
} // ns stardazed

#endif
