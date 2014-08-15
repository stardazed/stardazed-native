// ------------------------------------------------------------------
// math::Algorithm - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_ALGORITHM_H
#define SD_MATH_ALGORITHM_H

#include <type_traits>
#include <algorithm>
#include <numeric>

#include "math/Vector.hpp"


namespace stardazed {
namespace math {


//   ____ _
//  / ___| | __ _ _ __ ___  _ __
// | |   | |/ _` | '_ ` _ \| '_ \
// | |___| | (_| | | | | | | |_) |
//  \____|_|\__,_|_| |_| |_| .__/
//                         |_|

// clamp single scalar value to specified range
template <typename T>
constexpr
std::enable_if_t<std::is_arithmetic<T>::value, T>
clamp(T val, T min, T max) {
	return std::max(min, std::min(max, val));
}


// clamp generic Vector elements to specified range
template <size_t N, typename T>
Vector<N, T> clamp(const Vector<N, T>& vec, T min, T max) {
	Vector<N, T> result;
	
	std::transform(vec.dataBegin(), vec.dataEnd(), result.dataBegin(), [=](T v){
		return clamp(v, min, max);
	});
	
	return result;
}


// clamp Vector2 elements to specified range (specialization)
template <typename T>
Vector<2, T> clamp(const Vector<2, T>& vec, T min, T max) {
	return {
		clamp(vec[0], min, max),
		clamp(vec[1], min, max)
	};
}


// clamp Vector3 elements to specified range (specialization)
template <typename T>
Vector<3, T> clamp(const Vector<3, T>& vec, T min, T max) {
	return {
		clamp(vec[0], min, max),
		clamp(vec[1], min, max),
		clamp(vec[2], min, max)
	};
}


// clamp Vector4 elements to specified range (specialization)
template <typename T>
Vector<4, T> clamp(const Vector<4, T>& vec, T min, T max) {
	return {
		clamp(vec[0], min, max),
		clamp(vec[1], min, max),
		clamp(vec[2], min, max),
		clamp(vec[3], min, max)
	};
}


// clamp generic Vector elements to ranges specified by min/max Vector pair
template <size_t N, typename T>
Vector<N, T> clamp(const Vector<N, T>& vec, const Vector<N, T>& mins, const Vector<N, T>& maxes) {
	Vector<N, T> result;

	for (size_t ix=0; ix < N; ++ix)
		result[ix] = clamp(vec[ix], mins[ix], maxes[ix]);
	
	return result;
}


// clamp Vector2 elements to ranges specified by min/max Vector pair (specialization)
template <typename T>
Vector<2, T> clamp(const Vector<2, T>& vec, const Vector<2, T>& mins, const Vector<2, T>& maxes) {
	return {
		clamp(vec[0], mins[0], maxes[0]),
		clamp(vec[1], mins[1], maxes[1])
	};
}


// clamp Vector3 elements to ranges specified by min/max Vector pair (specialization)
template <typename T>
Vector<3, T> clamp(const Vector<3, T>& vec, const Vector<3, T>& mins, const Vector<3, T>& maxes) {
	return {
		clamp(vec[0], mins[0], maxes[0]),
		clamp(vec[1], mins[1], maxes[1]),
		clamp(vec[2], mins[2], maxes[2])
	};
}


// clamp Vector4 elements to ranges specified by min/max Vector pair (specialization)
template <typename T>
Vector<4, T> clamp(const Vector<4, T>& vec, const Vector<4, T>& mins, const Vector<4, T>& maxes) {
	return {
		clamp(vec[0], mins[0], maxes[0]),
		clamp(vec[1], mins[1], maxes[1]),
		clamp(vec[2], mins[2], maxes[2]),
		clamp(vec[3], mins[3], maxes[3])
	};
}



//  __  __ _
// |  \/  (_)_  __
// | |\/| | \ \/ /
// | |  | | |>  <
// |_|  |_|_/_/\_\
//

// mix 2 values together according to ratio
template <typename T>
constexpr
std::enable_if_t<std::is_arithmetic<T>::value, T>
mix(T a, T b, T ratio) {
	return a * (T{1} - ratio) + b * ratio;
}


// mix generic Vector elements together according to ratio
template <size_t N, typename T>
Vector<N, T> mix(const Vector<N, T>& a, const Vector<N, T>& b, T ratio) {
	Vector<N, T> result;

	std::transform(a.dataBegin(), a.dataEnd(), b.dataBegin(), result.dataBegin(), [=](T va, T vb){
		return mix(va, vb, ratio);
	});
	
	return result;
}


// mix Vector2 elements together according to ratio
template <typename T>
Vector<2, T> mix(const Vector<2, T>& a, const Vector<2, T>& b, T ratio) {
	return {
		mix(a[0], b[0], ratio),
		mix(a[1], b[1], ratio)
	};
}


// mix Vector3 elements together according to ratio
template <typename T>
Vector<3, T> mix(const Vector<3, T>& a, const Vector<3, T>& b, T ratio) {
	return {
		mix(a[0], b[0], ratio),
		mix(a[1], b[1], ratio),
		mix(a[2], b[2], ratio)
	};
}


// mix Vector4 elements together according to ratio
template <typename T>
Vector<4, T> mix(const Vector<4, T>& a, const Vector<4, T>& b, T ratio) {
	return {
		mix(a[0], b[0], ratio),
		mix(a[1], b[1], ratio),
		mix(a[2], b[2], ratio),
		mix(a[3], b[3], ratio)
	};
}


// mix generic Vector elements together according to Vector of ratios
template <size_t N, typename T>
Vector<N, T> mix(const Vector<N, T>& a, const Vector<N, T>& b, const Vector<N, T>& ratios) {
	Vector<N, T> result;

	for (size_t ix=0; ix < N; ++ix)
		result[ix] = mix(a[ix], b[ix], ratios[ix]);
	
	return result;
}


// mix Vector2 elements together according to Vector2 of ratios
template <typename T>
Vector<2, T> mix(const Vector<2, T>& a, const Vector<2, T>& b, const Vector<2, T>& ratios) {
	return {
		mix(a[0], b[0], ratios[0]),
		mix(a[1], b[1], ratios[1])
	};
}


// mix Vector3 elements together according to Vector3 of ratios
template <typename T>
Vector<3, T> mix(const Vector<3, T>& a, const Vector<3, T>& b, const Vector<3, T>& ratios) {
	return {
		mix(a[0], b[0], ratios[0]),
		mix(a[1], b[1], ratios[1]),
		mix(a[2], b[2], ratios[2])
	};
}


// mix Vector4 elements together according to Vector4 of ratios
template <typename T>
Vector<4, T> mix(const Vector<4, T>& a, const Vector<4, T>& b, const Vector<4, T>& ratios) {
	return {
		mix(a[0], b[0], ratios[0]),
		mix(a[1], b[1], ratios[1]),
		mix(a[2], b[2], ratios[2]),
		mix(a[3], b[3], ratios[3])
	};
}
	

// special mix variant with Vector<N, bool> ratios, where false yields a and true yields b (unoptimized, not specialized)
template <size_t N, typename T>
Vector<N, T> mix(const Vector<N, T>& a, const Vector<N, T>& b, const Vector<N, bool>& selects) {
	Vector<N, T> result;
	T ratios[] = { false, true };
	
	for (size_t ix=0; ix < N; ++ix)
		result[ix] = mix(a[ix], b[ix], ratios[selects[ix]]);
	
	return result;
}


} // ns math
} // ns stardazed

#endif
