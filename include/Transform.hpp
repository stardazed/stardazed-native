// ------------------------------------------------------------------
// math::Transform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_TRANSFORM_H
#define SD_MATH_TRANSFORM_H

#include "Matrix.hpp"

#include <initializer_list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cassert>
#include <cmath>


namespace stardazed {
namespace math {


// ---- Projection Matrix generation

template <typename T>
Matrix<4, 4, T> perspective() {
	
}


// ---- View Matrix generation

template <typename T>
Matrix<4, 4, T> lookAt(const Vector<3, T>& eye, const Vector<3, T>& target, const Vector<3, T>& up) {
	auto f = normalize(target - eye),
	s = normalize(cross(f, up)),
	u = cross(s, f);
	
	using V4 = Vector<4, T>;
	
	return makeSquareMatrixFromRows<4>({
		V4{ s, -dot(s, eye) },
		V4{ u, -dot(u, eye) },
		V4{ -f, dot(f, eye) },
		V4{ 0,0,0,1 }
	});
}


} // namespace math
} // namespace stardazed

#endif
