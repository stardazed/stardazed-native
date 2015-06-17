// ------------------------------------------------------------------
// physics::Transform - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/AxisAlignedBoundingBox.hpp"

namespace stardazed {
namespace physics {


AxisAlignedBoundingBox AxisAlignedBoundingBox::fromCenterAndSize(const math::Vec3& center, const math::Vec3& size) {
	AxisAlignedBoundingBox aabb;
	aabb.setCenterAndSize(center, size);
	return aabb;
}


void AxisAlignedBoundingBox::setCenterAndSize(const math::Vec3& center, const math::Vec3& size) {
	auto extents = size / 2;
	min_ = center - extents;
	max_ = center + extents;
}


void AxisAlignedBoundingBox::include(const math::Vec3& vec) {
	if (vec.x < min_.x) min_.x = vec.x;
	if (vec.x > max_.x) max_.x = vec.x;

	if (vec.y < min_.y) min_.y = vec.y;
	if (vec.y > max_.y) max_.y = vec.y;

	if (vec.z < min_.z) min_.z = vec.z;
	if (vec.z > max_.z) max_.z = vec.z;
}


void AxisAlignedBoundingBox::include(const AxisAlignedBoundingBox& aabb) {
	if (aabb.min_.x < min_.x) min_.x = aabb.min_.x;
	if (aabb.max_.x > max_.x) max_.x = aabb.max_.x;

	if (aabb.min_.y < min_.y) min_.y = aabb.min_.y;
	if (aabb.max_.y > max_.y) max_.y = aabb.max_.y;

	if (aabb.min_.z < min_.z) min_.z = aabb.min_.z;
	if (aabb.max_.z > max_.z) max_.z = aabb.max_.z;
}



} // ns physics
} // ns stardazed
