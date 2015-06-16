// ------------------------------------------------------------------
// physics::Transform - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/Transform.hpp"
#include <cmath>

namespace stardazed {
namespace physics {


// FIXME: this method is flawed
static math::Quat lookAtImpl(math::Vec3 localForward, const math::Vec3& worldUp) {
	using namespace math;

	auto localUp = worldUp;
	orthoNormalize(localForward, localUp);
	auto localRight = cross(localUp, localForward);
	
	auto w = std::sqrtf(1.0f + localRight.x + localUp.y	+ localForward.z) * 0.5f;
	if (nearEqual(w, 0.f)) // FIXME: this doesn't work
		return Quat::fromAxisAngle({ 0,1,0 }, Pi);

	auto oneOver4w = 1.0f / (4.0f * w);
	
	return {
		(localUp.z      - localForward.y) * oneOver4w,
		(localForward.x - localRight.z  ) * oneOver4w,
		(localRight.y   - localUp.x     ) * oneOver4w,
		w
	};
}


void Transform::lookAt(const Position3& target, const math::Vec3& up) {
	rotation = lookAtImpl((target - position).value, up);
}


Transform Transform::apply(const Transform& subTr) const {
	return {
		position + (rotation * subTr.position),
		scale * subTr.scale,
		rotation * subTr.rotation
	};
}


math::Mat4 Transform::toMatrix4() const {
	using namespace math;

	// rotation and scale
	// TODO: scaleMatrix is unlikely to change for many transforms, optimize/cache
	// TODO: same goes for position and rotation of basically all static geometry…
	auto m = math::translationMatrix(position.value) * rotation.toMatrix4() * math::scaleMatrix(scale);

	return m;
}


} // ns physics
} // ns stardazed
