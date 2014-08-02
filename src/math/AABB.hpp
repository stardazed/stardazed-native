// ------------------------------------------------------------------
// math::AABB (AxisAlignedBoundingBox) - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_AABB_H
#define SD_MATH_AABB_H

#include <math/Vector.hpp>
#include <limits>

namespace stardazed {
namespace math {


namespace detail {
	template <typename T>
	constexpr T limitMin() { return std::numeric_limits<T>::min(); }

	template <typename T>
	constexpr T limitMax() { return std::numeric_limits<T>::max(); }
} // ns detail


template <typename T = float>
struct AxisAlignedBoundingBox {
	T minX = detail::limitMax<T>(), minY = detail::limitMax<T>(), minZ = detail::limitMax<T>();
	T maxX = detail::limitMin<T>(), maxY = detail::limitMin<T>(), maxZ = detail::limitMin<T>();

	void includeVector3(const Vector<3, T>& vec) {
		if (vec.x < minX) minX = vec.x;
		if (vec.x > maxX) maxX = vec.x;

		if (vec.y < minY) minY = vec.y;
		if (vec.y > maxY) maxY = vec.y;

		if (vec.z < minZ) minZ = vec.z;
		if (vec.z > maxZ) maxZ = vec.z;
	}
	
	void includeAABB(const AxisAlignedBoundingBox& aabb) {
		if (aabb.minX < minX) minX = aabb.minX;
		if (aabb.maxX > maxX) maxX = aabb.maxX;

		if (aabb.minY < minY) minY = aabb.minY;
		if (aabb.maxY > maxY) maxY = aabb.maxY;

		if (aabb.minZ < minZ) minZ = aabb.minZ;
		if (aabb.maxZ > maxZ) maxZ = aabb.maxZ;
	}

	
	constexpr bool containsVector(const Vector<3, T>& vec) const {
		return vec.x >= minX && vec.y >= minY && vec.z >= minZ &&
		       vec.x <= maxX && vec.y <= maxY && vec.z <= maxZ;
	}

	constexpr bool fullyContainsAABB(const AxisAlignedBoundingBox& aabb) const {
		return aabb.minX >= minX && aabb.minY >= minY && aabb.minZ >= minZ &&
		       aabb.maxX <= maxX && aabb.maxY <= maxY && aabb.maxZ <= maxZ;
	}
	
	constexpr bool intersectsAABB(const AxisAlignedBoundingBox& aabb) const {
		return aabb.minX <= maxX && aabb.maxX >= minX &&
		       aabb.minY <= maxY && aabb.maxY >= minY &&
		       aabb.minZ <= maxZ && aabb.maxZ >= minZ;
	}
};


using AABB = AxisAlignedBoundingBox<>;


} // ns math
} // ns stardazed

#endif
