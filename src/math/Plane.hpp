// ------------------------------------------------------------------
// math::Plane - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_PLANE_H
#define SD_MATH_PLANE_H

#include "system/Config.hpp"
#include "math/Vector.hpp"

namespace stardazed {
namespace math {


class Plane {
	Vec3 normal_;
	float constant_;
	
public:
	constexpr Plane(const Vec3& normal, float constant)
	: normal_(normal)
	, constant_{constant}
	{}
	
	constexpr bool isFacing(const Vec3& normDirection) {
		return dot(normal_, normDirection) <= 0;
	}
	
	constexpr float distanceToPoint(const Vec3& point) {
		return dot(point, normal_) + constant_;
	}
	
	static constexpr Plane fromOriginAndNormal(const Vec3& origin, const Vec3& normal) {
		return { normal, -dot(normal, origin) };
	}
	
	static Plane fromClockWiseTriangle(const Vec3& p1, const Vec3& p2, const Vec3& p3) {
		auto normal = normalize(cross(p2 - p1, p3 - p1));
		return fromOriginAndNormal(p1, normal);
	}
	
	static Plane fromCounterClockWiseTriangle(const Vec3& p1, const Vec3& p2, const Vec3& p3) {
		auto normal = normalize(cross(p3 - p1, p2 - p1));
		return fromOriginAndNormal(p1, normal);
	}
	
	constexpr Vec3 normal() const { return normal_; }
	constexpr float constant() const { return constant_; }
};


} // ns math
} // ns stardazed
