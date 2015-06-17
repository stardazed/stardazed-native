// ------------------------------------------------------------------
// physics::Bounds - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "physics/Bounds.hpp"

namespace stardazed {
namespace physics {


Bounds Bounds::fromCenterAndSize(const math::Vec3& center, const math::Vec3& size) {
	Bounds bounds;
	bounds.setCenterAndSize(center, size);
	return bounds;
}


Bounds Bounds::fromMinAndMax(const math::Vec3& min, const math::Vec3& max) {
	Bounds bounds;
	bounds.setMinAndMax(min, max);
	return bounds;
}


void Bounds::setCenterAndSize(const math::Vec3& center, const math::Vec3& size) {
	assert(size.x >= 0);
	assert(size.y >= 0);
	assert(size.z >= 0);

	auto extents = size / 2;
	min_ = center - extents;
	max_ = center + extents;
}


void Bounds::setMinAndMax(const math::Vec3& min, const math::Vec3& max) {
	assert(min.x <= max.x);
	assert(min.y <= max.y);
	assert(min.z <= max.z);
	
	min_ = min;
	max_ = max;
}


void Bounds::include(const math::Vec3& vec) {
	if (vec.x < min_.x) min_.x = vec.x;
	if (vec.x > max_.x) max_.x = vec.x;

	if (vec.y < min_.y) min_.y = vec.y;
	if (vec.y > max_.y) max_.y = vec.y;

	if (vec.z < min_.z) min_.z = vec.z;
	if (vec.z > max_.z) max_.z = vec.z;
}


void Bounds::include(const Bounds& bounds) {
	if (bounds.min_.x < min_.x) min_.x = bounds.min_.x;
	if (bounds.max_.x > max_.x) max_.x = bounds.max_.x;

	if (bounds.min_.y < min_.y) min_.y = bounds.min_.y;
	if (bounds.max_.y > max_.y) max_.y = bounds.max_.y;

	if (bounds.min_.z < min_.z) min_.z = bounds.min_.z;
	if (bounds.max_.z > max_.z) max_.z = bounds.max_.z;
}


} // ns physics
} // ns stardazed
