// ------------------------------------------------------------------
// math::Algorithm - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_ALGORITHM_H
#define SD_MATH_ALGORITHM_H

#include <type_traits>
#include <algorithm>
#include <numeric>

#include "Vector.hpp"


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
	Vector<N, T> result { vec };
	
	std::transform(vec.dataBegin(), vec.dataEnd(), [=](T v){
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
	Vector<N, T> result { vec };

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



} // ns math
} // ns stardazed

#endif
