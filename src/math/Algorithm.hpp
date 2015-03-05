// ------------------------------------------------------------------
// math::Algorithm - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_ALGORITHM_H
#define SD_MATH_ALGORITHM_H

#include "system/Config.hpp"
#include <cmath>

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


// linearCoord
// return fraction of number and 1-fract for negative numbers
// to have linear increasing fractions for all numbers for e.g. uv-coords
template <typename T>
float texCoord(T v) {
	v -= trunc(v);
	if (v < 0)
		v += 1.f;
	return v;
};



} // ns math
} // ns stardazed

#endif
