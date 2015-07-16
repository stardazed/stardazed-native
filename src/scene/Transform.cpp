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


inline void recalcModelMatrix(const math::Vec3& pos, const math::Quat& rot, const math::Vec3& scale, math::Mat4& modelMat) {
	modelMat = math::translationMatrix(pos) * rot.toMatrix4() * math::scaleMatrix(scale);
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


Transform::Handle Transform::append(const Handle parent) {
	if (__builtin_expect(instanceData_.append() == container::InvalidatePointers::Yes, 0)) {
		rebase();
	}
	
	Handle h { nextRef_++ };

	parentBase_[h.ref] = parent;
	rotationBase_[h.ref] = math::Quat::identity();
	scaleBase_[h.ref] = math::Vec3::one();
	modelMatrixBase_[h.ref] = math::Mat4::identity();
	
	return h;
}


Transform::Handle Transform::append(const TransformDescriptor& desc, const Handle parent) {
	if (__builtin_expect(instanceData_.append() == container::InvalidatePointers::Yes, 0)) {
		rebase();
	}
	
	Handle h { nextRef_++ };
	
	parentBase_[h.ref] = parent;
	positionBase_[h.ref] = desc.position;
	rotationBase_[h.ref] = desc.rotation;
	scaleBase_[h.ref] = desc.scale;
	recalcModelMatrix(desc.position, desc.rotation, desc.scale, modelMatrixBase_[h.ref]);
	
	return h;
	
}


void Transform::setParent(const Handle h, const Handle newParent) {
	assert(h.ref != 0);
	parentBase_[h.ref] = newParent;
}


void Transform::setPosition(const Handle h, const math::Vec3& newPosition) {
	assert(h.ref != 0);

	positionBase_[h.ref] = newPosition;
	recalcModelMatrix(newPosition, rotation(h), scale(h), modelMatrixBase_[h.ref]);
}


void Transform::setRotation(const Handle h, const math::Quat& newRotation) {
	assert(h.ref != 0);

	rotationBase_[h.ref] = newRotation;
	recalcModelMatrix(position(h), newRotation, scale(h), modelMatrixBase_[h.ref]);
}


void Transform::setPositionAndRotation(const Handle h, const math::Vec3& newPosition, const math::Quat& newRotation) {
	assert(h.ref != 0);

	positionBase_[h.ref] = newPosition;
	rotationBase_[h.ref] = newRotation;
	recalcModelMatrix(newPosition, newRotation, scale(h), modelMatrixBase_[h.ref]);
}


void Transform::setScale(const Handle h, const math::Vec3& newScale) {
	assert(h.ref != 0);

	scaleBase_[h.ref] = newScale;
	recalcModelMatrix(position(h), rotation(h), newScale, modelMatrixBase_[h.ref]);
}


/*
void Transform::lookAt(const math::Vec3& target, const math::Vec3& up) {
	rotation = lookAtImpl(target - position, up);
}
*/

} // ns scene
} // ns stardazed
