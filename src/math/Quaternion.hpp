// ------------------------------------------------------------------
// math::Quaternion - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_QUATERNION_H
#define SD_MATH_QUATERNION_H

#include "Angle.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

#include <cassert>
#include <cmath>

namespace stardazed {
namespace math {


template <typename T = float>
struct Quaternion {
	union {
		T data[4];
		Vector<4, T> xyzw;
		struct { Vector<3, T> xyz; T w; };
		struct { T x, y, z; };
	};

	constexpr Quaternion() : data{0,0,0,1} {}
	constexpr Quaternion(T x, T y, T z, T w) : data{x,y,z,w} {}
	constexpr Quaternion(Vector<3, T> xyz, T w) : xyz(xyz), w(w) {}
	constexpr explicit Quaternion(Vector<4, T> xyzw) : xyzw(xyzw) {}


	// -- factory methods
	static constexpr Quaternion identity() { return {}; }
	static constexpr Quaternion zero() { return {0,0,0,0}; }


	static constexpr Quaternion fromAxisAngle(const Vector<3, T>& axis, const Angle angle) {
		const auto halfAngle = angle.rad() * T{0.5};
		return { normalize(axis) * sin(halfAngle), cos(halfAngle) };
	}


	static Quaternion fromEuler(const Angle yaw, const Angle pitch, const Angle roll) {
		auto y = yaw.rad() * T{0.5};
		auto p = pitch.rad() * T{0.5};
		auto r = roll.rad() * T{0.5};
		
		T siny, cosy, sinp, cosp, sinr, cosr;
		sincos(y, siny, cosy);
		sincos(p, sinp, cosp);
		sincos(r, sinr, cosr);

		// evaluated form of 3 Quat multiplications (of yaw, pitch and roll)
		return normalize(Quaternion{
			sinr * cosp * cosy - cosr * sinp * siny,
			cosr * sinp * cosy + sinr * cosp * siny,
			cosr * cosp * siny - sinr * sinp * cosy,
			cosr * cosp * cosy + sinr * sinp * siny
		});
	}


	// -- export conversion methods
	constexpr Matrix<4, 4, T> toMatrix4() const {
		const T x2 = x * x,
			y2 = y * y,
			z2 = z * z,
			xy = x * y,
			xz = x * z,
			yz = y * z,
			wx = w * x,
			wy = w * y,
			wz = w * z;

		const T zero{0}, one{1}, two{2}; // constants of proper value type

		return {
			one - two * (y2 + z2), two * (xy + wz),       two * (xz - wy),       zero,
			two * (xy - wz),       one - two * (x2 + z2), two * (yz + wx),       zero,
			two * (xz + wy),       two * (yz - wx),       one - two * (x2 + y2), zero,
			zero,                  zero,                  zero,                  one
		};
	}


	void toAxisAngle(Vector<3, T>& axis, Angle& angle) const {
		T tmp1 = T{1} - w * w;
		if (tmp1 <= T{0})
			axis = Vec3{0, 0, 1};
		else {
			T tmp2 = T{1} / std::sqrt(tmp1);
			axis = Vec3{x * tmp2, y * tmp2, z * tmp2};
		}

		angle = Radians{std::acos(w) * T{2}};
	}
};


// ---- default type alias
using Quat = Quaternion<>;


// ---- forwards
template <typename T>
constexpr Quaternion<T> inverse(const Quaternion<T>&);


// ---- Equality (strict)

template <typename T>
constexpr bool operator ==(const Quaternion<T>& a, const Quaternion<T>& b) {
	return a.xyzw == b.xyzw;
}


template <typename T>
constexpr bool operator !=(const Quaternion<T>& a, const Quaternion<T>& b) {
	return a.xyzw != b.xyzw;
}


// ---- Negation

template <typename T>
constexpr Quaternion<T> operator -(const Quaternion<T>& q) {
	return Quaternion<T>{ -q.xyzw };
}


// ---- Addition

template <typename T>
constexpr Quaternion<T> operator +(const Quaternion<T>& a, const Quaternion<T>& b) {
	return Quaternion<T>{ a.xyzw + b.xyzw };
}


template <typename T>
Quaternion<T>& operator +=(Quaternion<T>& a, const Quaternion<T>& b) {
	a.xyzw += b.xyzw;
	return a;
}


// ---- Subtraction

template <typename T>
constexpr Quaternion<T> operator -(const Quaternion<T>& a, const Quaternion<T>& b) {
	return Quaternion<T>{ a.xyzw - b.xyzw };
}


template <typename T>
Quaternion<T>& operator -=(Quaternion<T>& a, const Quaternion<T>& b) {
	a.xyzw -= b.xyzw;
	return a;
}


// ---- Multiplication

template <typename T>
constexpr Quaternion<T> operator *(const Quaternion<T>& a, const Quaternion<T>& b) {
	return {
		a.y * b.z - a.z * b.y + a.w * b.x + a.x * b.w,
		a.z * b.x - a.x * b.z + a.w * b.y + a.y * b.w,
		a.x * b.y - a.y * b.x + a.w * b.z + a.z * b.w,
		a.w * b.w - dot(a.xyz, b.xyz)
	};
}


template <typename T>
Quaternion<T>& operator *=(Quaternion<T>& a, const Quaternion<T>& b) {
	a = a * b;
	return a;
}


template <typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Quaternion<T>>
constexpr operator *(const Quaternion<T>& a, const S scalar) {
	return Quaternion<T>{ a.xyzw * scalar };
}


template <typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Quaternion<T>>
constexpr operator *(const S scalar, const Quaternion<T>& a) {
	return Quaternion<T>{ scalar * a.xyzw };
}


template <typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Quaternion<T>&>
operator *=(Quaternion<T>& a, const S scalar) {
	a.xyzw *= scalar;
	return a;
}



// ---- Division (scalar only, lhs only)

template <typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Quaternion<T>>
constexpr operator /(const Quaternion<T>& a, const S scalar) {
	return Quaternion<T>{ a.xyzw / scalar };
}


template <typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Quaternion<T>&>
operator /=(Quaternion<T>& a, const S scalar) {
	a.xyzw /= scalar;
	return a;
}


// ---- Quaternion-Vector multiplication

template <typename T>
constexpr Vector<3, T> operator *(const Quaternion<T>& quat, const Vector<3, T>& vec) {
	Quaternion<T> vq { normalize(vec), 0 };
	return (quat * vq * conjugate(quat)).xyz;
}


// ---- Non-member basic algorithms

template <typename T>
constexpr T lengthSquared(const Quaternion<T>& quat) {
	return lengthSquared(quat.xyzw);
}


template <typename T>
constexpr T length(const Quaternion<T>& quat) {
	return length(quat.xyzw);
}


template <typename T>
constexpr Quaternion<T> normalize(const Quaternion<T>& q) {
	return q / length(q);
}


template <typename T>
Quaternion<T>& normalizeInPlace(Quaternion<T>& q) {
	return q /= length(q);
}


template <typename T>
constexpr Quaternion<T> conjugate(const Quaternion<T>& q) {
	return { -q.xyz, q.w };
}


template <typename T>
constexpr Quaternion<T> inverse(const Quaternion<T>& q) {
	return normalize(conjugate(q));
}


template <typename T>
constexpr Quaternion<T> dot(const Quaternion<T>& a, const Quaternion<T>& b) {
	return dot(a.xyzw, b.xyzw);
}


template <typename T>
constexpr Quaternion<T> exp(const Quaternion<T>& q) {
	auto th = length(q.xyz);
	T sinth = sin(th), costh = cos(th);
	
	return { sinth * q.xyz / th, costh };
}


template <typename T>
constexpr Quaternion<T> log(const Quaternion<T>& q) {
	auto th = acos(q.w),
		sinth = sin(th);
	
	return { th * q.xyz * sinth, 0 };
}


// ---- Non-member interpolation algorithms

template <typename T>
constexpr Quaternion<T> slerp(const Quaternion<T>& q1, const Quaternion<T>& q2, float t) {
	auto costh = dot(q1, q2);
	auto angle = std::acos(costh);
	return (q1 * std::sin(angle * (1.f - t)) + q2 * std::sin(angle * t)) / std::sin(angle);
}


namespace detail {

	template <typename T>
	constexpr Quaternion<T> slerpPreCalced(const Quaternion<T>& q1, const Quaternion<T>& q2, T angleRad, T oneOverSinA, float t) {
		return (q1 * std::sin(angleRad * (1.f - t)) + q2 * std::sin(angleRad * t)) * oneOverSinA;
	}

} // ns detail


template <typename T>
auto makeSmoothSlerp(Quaternion<T> q1, Quaternion<T> q2) {
	auto costh = dot(q1, q2);
	auto angle = costh < 0 ? acos(-costh) : acos(costh);

	return [
		angle = angle,
		oneOverSinA = T{1} / std::sin(angle),
		q1 = q1,
		q2 = costh < 0 ? -q2 : q2
	](float t) {
		return detail::slerpPreCalced(q1, q2, angle, oneOverSinA, t);
	};
}


template <typename T>
Quaternion<T> squad(const Quaternion<T>& q1, const Quaternion<T>& a,
					const Quaternion<T>& b, const Quaternion<T>& c,
					float t)
{
	return slerp(
		slerp(q1, c, t),
		slerp(a, b, t),
		2.f * t * (1.f - t)
	);
}


template <typename T>
auto makeSmoothSquad(Quaternion<T> q0, Quaternion<T> q1, Quaternion<T> q2, Quaternion<T> q3) {
	q0 = length(q0 + q1) < length(q0 - q1) ? -q0 : q0;
	q2 = length(q1 + q2) < length(q1 - q2) ? -q2 : q2;
	q3 = length(q2 + q3) < length(q2 - q3) ? -q3 : q3;
	
	return [
		q1 = q1,
		a = q1 * exp(-0.25 * (log(exp(q1) * q2) + log(exp(q1) * q0))),
		b = q2 * exp(-0.25 * (log(exp(q2) * q3) + log(exp(q2) * q1))),
		c = q2
	](float t) {
		return squad(q1, a, b, c, t);
	};
}


// ---- Swappable

template <typename T>
void swap(Quaternion<T>& a, Quaternion<T>& b) {
	swap(a.xyzw, b.xyzw);
}


} // ns math
} // ns stardazed


#endif
