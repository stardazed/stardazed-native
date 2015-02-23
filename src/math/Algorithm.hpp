// ------------------------------------------------------------------
// math::Algorithm - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_ALGORITHM_H
#define SD_MATH_ALGORITHM_H

#include "system/Config.hpp"


namespace stardazed {
namespace math {


// max
// return largest of 2 values

template <typename T>
constexpr
T max(T a, T b) {
	return a > b ? a : b;
}


// min
// return smallest of 2 values

template <typename T>
constexpr
T min(T a, T b) {
	return a < b ? a : b;
}

	
// lerp
// return linearly interpolated value on range [from, to] at ratio t

template <typename T>
constexpr
T lerp(T from, T to, T t) {
	return from + ((to - from) * t);
}


// clamp
// clamp value to lower and upper bound

// clamp single scalar value to specified range
template <typename T>
constexpr
T clamp(T val, T lower, T upper) {
	return max(lower, min(upper, val));
}


// mix
// return weighted average of 2 values

template <typename T>
constexpr
T mix(T a, T b, T ratio) {
	return a * (T{1} - ratio) + b * ratio;
}


} // ns math
} // ns stardazed

#endif
