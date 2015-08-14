// ------------------------------------------------------------------
// math::Algorithm - stardazed
// (c) 2015 by Arthur Langereis
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


// sign
// get signed 1 value of a value or 0 if it's 0

template <typename T>
constexpr
T sign(T a) {
	return a > T{0} ? T{1} : (a < T{0} ? T{-1} : T{0});
}
	
	
// texCoord
// return fraction of number and 1-fract for negative numbers
// to have linear increasing fractions for all numbers for e.g. uv-coords

template <typename T>
float texCoord(T v) {
	v -= trunc(v);
	if (v < 0)
		v += 1.f;
	return v;
};


// roundUpPowerOf2
// return closest powerOf2 number that is >= n
// e.g.: 15 -> 16; 16 -> 16; 17 -> 32

constexpr uint32 roundUpPowerOf2(uint32 n) {
	if (n == 0) return 1;
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	return n + 1;
}


// alignUp
// round val up to closest alignmentPow2

constexpr uint32 alignUp(uint32 val, uint alignmentPow2) {
	return (val + alignmentPow2 - 1) & (~(alignmentPow2 - 1));
}


constexpr uint64 alignUp(uint64 val, uint alignmentPow2) {
	return (val + alignmentPow2 - 1) & (~(alignmentPow2 - 1));
}


// alignDown
// round val down to closest alignmentPow2

constexpr uint32 alignDown(uint32 val, uint32 alignmentPow2) {
	return val & (~(alignmentPow2 - 1));
}


constexpr uint64 alignDown(uint64 val, uint alignmentPow2) {
	return val & (~(alignmentPow2 - 1));
}



} // ns math
} // ns stardazed

#endif
