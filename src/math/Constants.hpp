// ------------------------------------------------------------------
// math::Constants - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_CONSTANTS_H
#define SD_MATH_CONSTANTS_H

#include "system/Config.hpp"
#include <cmath>
#include <limits>

namespace stardazed {
namespace math {


// ---- Epsilon

template <typename T>
constexpr const T Epsilon = std::numeric_limits<T>::epsilon();

template constexpr const float Epsilon<float>;
template constexpr const double Epsilon<double>;


// ---- Near-Equal Functions

template <typename T>
constexpr bool nearEqual(const T a, const T b, const T maxDiff = Epsilon<T>) {
	return std::abs(a - b) < maxDiff;
}


template <typename T>
constexpr bool nearNonEqual(const T a, const T b, const T maxDiff = Epsilon<T>) {
	return std::abs(a - b) >= maxDiff;
}


} // ns math
} // ns stardazed

#endif
