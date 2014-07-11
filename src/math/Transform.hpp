// ------------------------------------------------------------------
// math::Transform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_TRANSFORM_H
#define SD_MATH_TRANSFORM_H

#include "math/Angle.hpp"
#include "math/Matrix.hpp"
#include "math/Quaternion.hpp"

#include <cmath>



// TODO: move this code out to Matrix object



namespace stardazed {
namespace math {


// ---- Projection Matrix generation



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


} // ns math
} // ns stardazed

#endif
