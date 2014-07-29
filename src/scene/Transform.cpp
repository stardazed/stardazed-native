// ------------------------------------------------------------------
// scene::Transform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Transform.hpp"
#include <cmath>

namespace stardazed {
namespace scene {


static math::Quat lookAtImpl(const math::Vec3& eye, const math::Vec3& target, const math::Vec3& up) {
	using namespace math;
	
	static const Vec3 forward { 0, 0, -1 };
	
	auto dir = normalize(target - eye);
	auto axis = cross(dir, up);
	auto angle = dot(dir, forward);
	return Quat::fromAxisAngle(up, Radians{std::acosf(angle)});
}


void Transform::lookAt(const math::Vec3& target, const math::Vec3& up) {
	rotation = lookAtImpl(position, target, up);
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
