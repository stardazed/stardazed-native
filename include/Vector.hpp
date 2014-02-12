// ------------------------------------------------------------------
// Vector - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_VECTOR_H
#define SD_MATH_VECTOR_H

#include <array>
#include <iostream>
#include <type_traits>

namespace stardazed {
namespace math {


namespace /* detail */ {
	template <typename T>
	using TVec3 = std::array<T, 3>;

	template <typename T>
	using TVec4 = std::array<T, 4>;
}


using Vec3  = TVec3<float>;
using DVec3 = TVec3<double>;
using IVec3 = TVec3<int>;

using Vec4  = TVec4<float>;
using DVec4 = TVec4<double>;
using IVec4 = TVec4<int>;


// ---- component-wise addition by other vec or scalar
template <typename T>
constexpr TVec3<T> operator+ (const TVec3<T> lhs, const TVec3<T> rhs) {
	return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2] };
}
	
template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec3<T>>
operator+ (const TVec3<T> lhs, const S rhs) {
	return { lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs };
}

template <typename T>
constexpr TVec4<T> operator+ (const TVec4<T> lhs, const TVec4<T> rhs) {
	return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] };
}

template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec4<T>>
operator+ (const TVec4<T> lhs, const S rhs) {
	return { lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs, lhs[3] + rhs };
}


// ---- component-wise subtraction by other vec or scalar
template <typename T>
constexpr TVec3<T> operator- (const TVec3<T> lhs, const TVec3<T> rhs) {
	return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2] };
}
	
template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec3<T>>
operator- (const TVec3<T> lhs, const S rhs) {
	return { lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs };
}

template <typename T>
constexpr TVec4<T> operator- (const TVec4<T> lhs, const TVec4<T> rhs) {
	return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] };
}

template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec4<T>>
operator- (const TVec4<T> lhs, const S rhs) {
	return { lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs, lhs[3] - rhs };
}


// ---- component-wise multiplication by other vec or scalar
template <typename T>
constexpr TVec3<T> operator* (const TVec3<T> lhs, const TVec3<T> rhs) {
	return { lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2] };
}

template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec3<T>>
operator* (const TVec3<T> lhs, const S rhs) {
	return { lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs };
}

template <typename T>
constexpr TVec4<T> operator* (const TVec4<T> lhs, const TVec4<T> rhs) {
	return { lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2], lhs[3] * rhs[3] };
}

template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec4<T>>
operator* (const TVec4<T> lhs, const S rhs) {
	return { lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs, lhs[3] * rhs };
}


// ---- component-wise division by other vec or scalar
template <typename T>
constexpr TVec3<T> operator/ (const TVec3<T> lhs, const TVec3<T> rhs) {
	return { lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2] };
}

template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec3<T>>
operator/ (const TVec3<T> lhs, const S rhs) {
	return { lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs };
}

template <typename T>
constexpr TVec4<T> operator/ (const TVec4<T> lhs, const TVec4<T> rhs) {
	return { lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2], lhs[3] / rhs[3] };
}

template <typename T, typename S>
constexpr
std::enable_if_t<std::is_convertible<S, T>::value, TVec4<T>>
operator/ (const TVec4<T> lhs, const S rhs) {
	return { lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs, lhs[3] / rhs };
}


// ---- easily print vecs
template <typename T>
std::ostream& operator<< (std::ostream& os, const TVec3<T>& vec) {
	return os << '(' << vec[0] << ',' << vec[1] << ',' << vec[2] << ')';
}

template <typename T>
std::ostream& operator<< (std::ostream& os, const TVec4<T>& vec) {
	return os << '(' << vec[0] << ',' << vec[1] << ',' << vec[2] << ',' << vec[3] << ')';
}


} // ns math
} // ns stardazed

#endif
