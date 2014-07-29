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

	// from glm

	auto cosTheta = dot(eye, target);
	Vec3 rotationAxis;
	
	if (cosTheta < -1.f + 0.000001f) {
		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		// This implementation favors a rotation around the Up axis (Y),
		// since it's often what you want to do.
		rotationAxis = cross(Vec3{0, 0, 1}, eye);
		if (lengthSquared(rotationAxis) < 0.000001f) // bad luck, they were parallel, try again!
			rotationAxis = cross(Vec3{1, 0, 0}, eye);

		return Quat::fromAxisAngle(normalize(rotationAxis), Radians{Pi<float>});
	}
	
	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(eye, target);
	
	auto s = std::sqrt((1.f + cosTheta) * 2.f);
	
	return {
		rotationAxis / s,
		s * 0.5f
	};
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
