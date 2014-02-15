// ------------------------------------------------------------------
// math::MatVecMethods - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_MATVECMETHODS_H
#define SD_MATH_MATVECMETHODS_H

#include "Vector.hpp"
#include "Matrix.hpp"
#include <cmath>


namespace stardazed {
namespace math {


template <typename T>
T length(const TVec3<T>& vec) {
	return std::sqrt((vec[0]*vec[0]) + (vec[1]*vec[1]) + (vec[2]*vec[2]));
}


template <typename T>
T length(const TVec4<T>& vec) {
	return std::sqrt((vec[0]*vec[0]) + (vec[1]*vec[1]) + (vec[2]*vec[2]) + (vec[3]*vec[3]));
}


template <typename T>
TVec3<T> normalize(const TVec3<T>& vec) {
	return vec / length(vec);
}


template <typename T>
TVec4<T> normalize(const TVec4<T>& vec) {
	return vec / length(vec);
}


} // ns math
} // ns stardazed


#endif
