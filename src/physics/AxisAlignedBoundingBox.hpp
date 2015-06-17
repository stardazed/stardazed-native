// ------------------------------------------------------------------
// physics::AxisAlignedBoundingBox - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_AABB_H
#define SD_PHYSICS_AABB_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "physics/Units.hpp"

namespace stardazed {
namespace physics {


class AxisAlignedBoundingBox {
	Position3 min_ {}, max_ {};
	
public:
	AxisAlignedBoundingBox() = default;
	
	static AxisAlignedBoundingBox fromCenterAndSize(const Position3& center, const Position3& size);
	
	void setCenterAndSize(const physics::Position3& center, const Position3& size);
	
	

	void include(const math::Vec3& vec);
	void include(const Position3& pos) { include(pos.value); }
	void include(const AxisAlignedBoundingBox& aabb);
	
	constexpr bool contains(const Position3& vec) const {
		return vec.x >= min_.x && vec.y >= min_.y && vec.z >= min_.z &&
		       vec.x <= max_.x && vec.y <= max_.y && vec.z <= max_.z;
	}

	constexpr bool contains(const AxisAlignedBoundingBox& aabb) const {
		return aabb.min_.x >= min_.x && aabb.min_.y >= min_.y && aabb.min_.z >= min_.z &&
		       aabb.max_.x <= max_.x && aabb.max_.y <= max_.y && aabb.max_.z <= max_.z;
	}
	
	constexpr bool intersects(const AxisAlignedBoundingBox& aabb) const {
		return aabb.min_.x <= max_.x && aabb.max_.x >= min_.x &&
		       aabb.min_.y <= max_.y && aabb.max_.y >= min_.y &&
		       aabb.min_.z <= max_.z && aabb.max_.z >= min_.z;
	}
};


using AABB = AxisAlignedBoundingBox;


} // ns physics
} // ns stardazed

#endif
