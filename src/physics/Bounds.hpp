// ------------------------------------------------------------------
// physics::Bounds - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_BOUNDS_H
#define SD_PHYSICS_BOUNDS_H

#include "system/Config.hpp"
#include "math/Vector.hpp"

namespace stardazed {
namespace physics {


class Bounds {
	math::Vec3 min_ {}, max_ {};

public:
	Bounds() = default;
	static Bounds fromCenterAndSize(const math::Vec3& center, const math::Vec3& size);
	static Bounds fromMinAndMax(const math::Vec3& min, const math::Vec3& max);
	
	// --
	
	void setCenterAndSize(const math::Vec3& center, const math::Vec3& size);
	void setMinAndMax(const math::Vec3& min, const math::Vec3& max);
	
	void include(const math::Vec3&);
	void include(const Bounds&);

	// --
	
	constexpr math::Vec3 min() const { return min_; }
	constexpr math::Vec3 max() const { return max_; }
	
	math::Vec3 size() const { return max_ - min_; }
	math::Vec3 extents() const { return size() / 2.f; }
	math::Vec3 center() const { return min() + extents(); }
	
	// --
	
	constexpr bool contains(const math::Vec3& vec) const {
		return vec.x >= min_.x && vec.y >= min_.y && vec.z >= min_.z &&
		       vec.x <= max_.x && vec.y <= max_.y && vec.z <= max_.z;
	}

	constexpr bool contains(const Bounds& bounds) const {
		return bounds.min_.x >= min_.x && bounds.min_.y >= min_.y && bounds.min_.z >= min_.z &&
		       bounds.max_.x <= max_.x && bounds.max_.y <= max_.y && bounds.max_.z <= max_.z;
	}
	
	constexpr bool intersects(const Bounds& bounds) const {
		return bounds.min_.x <= max_.x && bounds.max_.x >= min_.x &&
		       bounds.min_.y <= max_.y && bounds.max_.y >= min_.y &&
		       bounds.min_.z <= max_.z && bounds.max_.z >= min_.z;
	}
};


} // ns physics
} // ns stardazed

#endif
