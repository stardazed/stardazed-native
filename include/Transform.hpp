// ------------------------------------------------------------------
// math::Transform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_TRANSFORM_H
#define SD_MATH_TRANSFORM_H

#include "Angle.hpp"
#include "Matrix.hpp"

#include <cmath>


namespace stardazed {
namespace math {


// ---- Projection Matrix generation

template <typename T>
Matrix<4, 4, T> perspective(Angle fovy, T aspect, T zNear, T zFar) {
	auto tanHalfFovy = std::tan(fovy.rad().val() / T{2} );

	Matrix<4, 4, T> result;
	result[0][0] = T{1} / (aspect * tanHalfFovy);
	result[1][1] = T{1} / tanHalfFovy;
	result[2][2] = -(zFar + zNear) / (zFar - zNear);
	result[2][3] = -1;
	result[3][2] = -(T{2} * zFar * zNear) / (zFar - zNear);
	return result;
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


} // ns math
} // ns stardazed

#endif
