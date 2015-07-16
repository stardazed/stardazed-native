// ------------------------------------------------------------------
// scene::Transform - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Transform.hpp"
#include <cmath>

namespace stardazed {
namespace scene {


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


Transform::Transform()
: instanceData_{ memory::SystemAllocator::sharedInstance(), 512 }
{
	rebase();
}


void Transform::rebase() {
	parentBase_ = instanceData_.elementsBasePtr<0>();
	positionBase_ = instanceData_.elementsBasePtr<1>();
	rotationBase_ = instanceData_.elementsBasePtr<2>();
	scaleBase_ = instanceData_.elementsBasePtr<3>();
	modelMatrixBase_ = instanceData_.elementsBasePtr<4>();
}


Transform::Handle Transform::append(Handle parent) {
	if (__builtin_expect(instanceData_.append() == container::InvalidatePointers::Yes, 0)) {
		rebase();
	}
	
	Handle h { nextRef_++ };

	parentBase_[h.ref] = parent;
	scaleBase_[h.ref] = math::Vec3::one();
	
	return h;
}


void Transform::setParent(Handle h, Handle newParent) {
	
}


inline void recalcModelMatrix(const math::Vec3& pos, const math::Quat& rot, const math::Vec3& scale, math::Mat4& modelMat) {
	modelMat = math::translationMatrix(pos) * rot.toMatrix4() * math::scaleMatrix(scale);
}


void Transform::setPosition(Handle h, const math::Vec3& newPosition) {
	positionBase_[h.ref] = newPosition;
	modelMatrixBase_[h.ref] = math::translationMatrix(position(h)) * rotation(h).toMatrix4() * math::scaleMatrix(scale(h));
	recalcModelMatrix(position(h), rotation(h), scale(h), modelMatrixBase_[h.ref]);
}


void Transform::setRotation(Handle h, const math::Quat& newRotation) {
	
}


void Transform::setScale(Handle h, const math::Vec3& newScale) {
	
}


/*
void Transform::lookAt(const math::Vec3& target, const math::Vec3& up) {
	rotation = lookAtImpl(target - position, up);
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
	auto m = math::translationMatrix(position) * rotation.toMatrix4() * math::scaleMatrix(scale);

	return m;
}
*/

} // ns scene
} // ns stardazed
