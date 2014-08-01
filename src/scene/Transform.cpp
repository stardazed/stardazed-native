// ------------------------------------------------------------------
// scene::Transform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Transform.hpp"
#include <cmath>

namespace stardazed {
namespace scene {


static math::Quat lookAtImpl(math::Vec3 localForward, const math::Vec3& worldUp) {
	using namespace math;

	auto localUp = worldUp;
	orthoNormalize(localForward, localUp);
	auto localRight = cross(localUp, localForward);
	
	auto w = std::sqrtf(1.0f + localRight.x + localUp.y	+ localForward.z) * 0.5f;
	if (nearEqual(w, 0.f))
		return Quat::fromAxisAngle({ 0,1,0 }, Radians{Pi<float>});

	auto oneOver4w = 1.0f / (4.0f * w);
	
	return {
		(localUp.z      - localForward.y) * oneOver4w,
		(localForward.x - localRight.z  ) * oneOver4w,
		(localRight.y   - localUp.x     ) * oneOver4w,
		w
	};
}


void Transform::lookAt(const math::Vec3& target, const math::Vec3& up) {
	rotation = lookAtImpl(target - position, up);
}


void Transform::rotate(math::Angle overX, math::Angle overY, math::Angle overZ) {
	// fromEuler takes yaw, pitch and roll arguments -> Y, X, Z
	rotation *= math::Quat::fromEuler(overY, overX, overZ);
}


void Transform::rotate(const math::Vec3& axis, math::Angle angle) {
	rotation *= math::Quat::fromAxisAngle(axis, angle);
}


void Transform::rotate(const math::Quat& q) {
	rotation *= q;
}


void Transform::translate(const math::Vec3& translation) {
	position += rotation * translation;
}


void Transform::translate(float x, float y, float z) {
	position += rotation * math::Vec3{ x, y, z };
}


void Transform::translateGlobal(const math::Vec3& globalTranslation) {
	position += globalTranslation;
}


void Transform::translateGlobal(float gx, float gy, float gz) {
	position += math::Vec3{ gx, gy, gz };
}


math::Mat4 Transform::toMatrix4() const {
	using namespace math;

	// rotation
	auto m = rotation.toMatrix4();

	// translation
    m[3].xyz = {
		-dot(Vec3{m[0][0], m[1][0], m[2][0]}, position),
		-dot(Vec3{m[0][1], m[1][1], m[2][1]}, position),
		-dot(Vec3{m[0][2], m[1][2], m[2][2]}, position)
	};

	return m;
}


} // ns scene
} // ns stardazed
