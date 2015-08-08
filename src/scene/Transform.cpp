// ------------------------------------------------------------------
// scene::TransformComponent - stardazed
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


TransformComponent::TransformComponent()
: instanceData_{ memory::SystemAllocator::sharedInstance(), 512 }
{
	rebase();
}


void TransformComponent::rebase() {
	parentBase_ = instanceData_.elementsBasePtr<0>();
	positionBase_ = instanceData_.elementsBasePtr<1>();
	rotationBase_ = instanceData_.elementsBasePtr<2>();
	scaleBase_ = instanceData_.elementsBasePtr<3>();
	modelMatrixBase_ = instanceData_.elementsBasePtr<4>();
}


TransformComponent::Instance TransformComponent::assign(Entity linkedEntity, const Instance parent) {
	auto entIndex = linkedEntity.index();

	if (instanceData_.count() < entIndex) {
		auto newCount = math::roundUpPowerOf2(entIndex);
		if (instanceData_.resize(newCount) == container::InvalidatePointers::Yes) {
			rebase();
		}
	}
	
	Instance h { entIndex };

	parentBase_[h.ref] = parent;
	rotationBase_[h.ref] = math::Quat::identity();
	scaleBase_[h.ref] = math::Vec3::one();
	modelMatrixBase_[h.ref] = math::Mat4::identity();
	
	return h;
}


TransformComponent::Instance TransformComponent::assign(Entity linkedEntity, const TransformDescriptor& desc, const Instance parent) {
	auto entIndex = linkedEntity.index();
	
	if (instanceData_.count() < entIndex) {
		auto newCount = math::roundUpPowerOf2(entIndex);
		if (instanceData_.resize(newCount) == container::InvalidatePointers::Yes) {
			rebase();
		}
	}
	
	Instance h { entIndex };
	
	parentBase_[h.ref] = parent;
	positionBase_[h.ref] = desc.position;
	rotationBase_[h.ref] = desc.rotation;
	scaleBase_[h.ref] = desc.scale;
	recalcModelMatrix(desc.position, desc.rotation, desc.scale, modelMatrixBase_[h.ref]);
	
	return h;
}


TransformComponent::Instance TransformComponent::forEntity(Entity ent) const {
	return { ent.index() };
}


void TransformComponent::setParent(const Instance h, const Instance newParent) {
	assert(h.ref != 0);
	parentBase_[h.ref] = newParent;
}


void TransformComponent::setPosition(const Instance h, const math::Vec3& newPosition) {
	assert(h.ref != 0);

	positionBase_[h.ref] = newPosition;
	recalcModelMatrix(newPosition, rotation(h), scale(h), modelMatrixBase_[h.ref]);
}


void TransformComponent::setRotation(const Instance h, const math::Quat& newRotation) {
	assert(h.ref != 0);

	rotationBase_[h.ref] = newRotation;
	recalcModelMatrix(position(h), newRotation, scale(h), modelMatrixBase_[h.ref]);
}


void TransformComponent::setPositionAndRotation(const Instance h, const math::Vec3& newPosition, const math::Quat& newRotation) {
	assert(h.ref != 0);

	positionBase_[h.ref] = newPosition;
	rotationBase_[h.ref] = newRotation;
	recalcModelMatrix(newPosition, newRotation, scale(h), modelMatrixBase_[h.ref]);
}


void TransformComponent::setScale(const Instance h, const math::Vec3& newScale) {
	assert(h.ref != 0);

	scaleBase_[h.ref] = newScale;
	recalcModelMatrix(position(h), rotation(h), newScale, modelMatrixBase_[h.ref]);
}


void TransformComponent::lookAt(const Instance h, const math::Vec3& target, const math::Vec3& up) {
	setRotation(h, lookAtImpl(target - position(h), up));
}


} // ns scene
} // ns stardazed
