// ------------------------------------------------------------------
// math::Bounds - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_BOUNDS_H
#define SD_MATH_BOUNDS_H

#include "system/Config.hpp"
#include "math/Vector.hpp"

#include <cfloat>

namespace stardazed {
namespace math {


class Bounds {
	// initialize min-max to extreme invalid bounds so it can be be used
	// as a starting point with def ctor and then using include(…) calls
	Vec3 min_ { FLT_MAX, FLT_MAX, FLT_MAX };
	Vec3 max_ { FLT_MIN, FLT_MIN, FLT_MIN };

public:
	Bounds() = default;
	static Bounds fromCenterAndSize(const Vec3& center, const Vec3& size);
	static Bounds fromMinAndMax(const Vec3& min, const Vec3& max);
	
	// --
	
	void setCenterAndSize(const Vec3& center, const Vec3& size);
	void setMinAndMax(const Vec3& min, const Vec3& max);
	
	void include(const Vec3&);
	void include(const Bounds&);

	// --
	
	constexpr const Vec3& min() const { return min_; }
	constexpr const Vec3& max() const { return max_; }
	
	Vec3 size() const { return max_ - min_; }
	Vec3 extents() const { return size() / 2.f; }
	Vec3 center() const { return min() + extents(); }
	
	// --
	
	constexpr bool contains(const Vec3& pt) const {
		return pt.x >= min_.x && pt.y >= min_.y && pt.z >= min_.z &&
		       pt.x <= max_.x && pt.y <= max_.y && pt.z <= max_.z;
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
	
	Vec3 closestPoint(const Vec3&) const;
};


} // ns math
} // ns stardazed

#endif
