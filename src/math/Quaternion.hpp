// ------------------------------------------------------------------
// math::Quaternion - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_QUATERNION_H
#define SD_MATH_QUATERNION_H

#include "Angle.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

#include <algorithm>
#include <functional>
#include <numeric>
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


	// -- common constant Quats
	static constexpr Quaternion identity() { return {}; }
	static constexpr Quaternion zero() { return {0,0,0,0}; }


	// -- factory methods
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
	
	
	// -- conversion to Mat4
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
			one - two * (y2 + z2), two * (xy - wz), two * (xz + wy), zero,
			two * (xy + wz), one - two * (x2 + z2), two * (yz - wx), zero,
			two * (xz - wy), two * (yz + wx), one - two * (x2 + y2), zero,
			zero, zero, zero, one
		};
	}


	void toAxisAngle(Vector<3, T>& axis, Angle& angle) const {
		auto oneOverLength = T{1} / length(*this);
		axis = xyz * oneOverLength;
		angle = Radians{std::acos(w) * T{2}};
	}
};


// ---- default type alias
using Quat = Quaternion<>;


// ---- forwards
template <typename T>
constexpr Quaternion<T> inverse(const Quaternion<T>&);


// ---- Addition

template <typename T>
constexpr Quaternion<T> operator +(const Quaternion<T>& a, const Quaternion<T>& b) {
	return { a.xyzw + b.xyzw };
}


template <typename T>
Quaternion<T>& operator +=(Quaternion<T>& a, const Quaternion<T>& b) {
	a.xyzw += b.xyzw;
	return a;
}


// ---- Subtraction

template <typename T>
constexpr Quaternion<T> operator -(const Quaternion<T>& a, const Quaternion<T>& b) {
	return { a.xyzw - b.xyzw };
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


// ---- Division (scalar only)

template <typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Quaternion<T>>
constexpr operator /(const Quaternion<T>& a, const S scalar) {
	return { a.xyzw / scalar };
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


// ---- Non-member algorithms

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


// ---- Non-member make functions




} // ns math
} // ns stardazed


#endif
