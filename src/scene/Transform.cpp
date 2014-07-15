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
	auto axis = math::normalize(math::cross(direction, up));

	auto dot = math::dot(up, direction);
	auto angle = std::acos(dot);

	rotation = math::Quat::fromAxisAngle(axis, math::Radians{angle});
}
	

math::Mat4 Transform::toMatrix4() const {
	auto qr = math::Quat::fromAxisAngle({1,0,0}, math::Radians{math::Pi<float> / 2.f});
	math::Mat4 m = qr.toMatrix4();

	// insert position
	m[3].xyz = -position;

	return m;
}


} // ns scene
} // ns stardazed
