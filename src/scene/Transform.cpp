// ------------------------------------------------------------------
// scene::Transform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Transform.hpp"
#include <cmath>

namespace stardazed {
namespace scene {


void Transform::lookAt(const math::Vec3& target, const math::Vec3& up) {
	auto direction = math::normalize(target - position);
	auto axis = math::cross(direction, up);

	auto dot = math::dot(up, direction);
	auto angle = std::acos(dot);

	rotation = math::Quat::fromAxisAngle(axis, angle);
}
	

math::Mat4 Transform::toMatrix4() const {
	auto m = rotation.toMatrix4();

	// insert position in first 3 elements of column 3
//	auto r3 = math::colBegin(m, 3);
//	*r3++ = position.x;
//	*r3++ = position.y;
//	*r3++ = position.z;

	return m;
}


} // ns scene
} // ns stardazed
