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

	constexpr Quaternion() : data{0,0,0,0} {}
	constexpr Quaternion(T x, T y, T z, T w) : data{x,y,z,w} {}
	constexpr Quaternion(Vector<3, T> xyz, T w) : xyz(xyz), w(w) {}
	constexpr explicit Quaternion(Vector<4, T> xyzw) : xyzw(xyzw) {}
	
	static constexpr Quaternion identity() { return { 0,0,0,1 }; }
	static constexpr Quaternion zero() { return {}; }
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


// ---- Division

template <typename T>
constexpr Quaternion<T> operator /(const Quaternion<T>& a, const Quaternion<T>& b) {
	return inverse(a) * b;
}


template <typename T>
Quaternion<T>& operator /=(Quaternion<T>& a, const Quaternion<T>& b) {
	a = a / b;
	return a;
}


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
