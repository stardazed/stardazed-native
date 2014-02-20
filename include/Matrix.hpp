// ------------------------------------------------------------------
// math::Matrix - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_MATRIX_H
#define SD_MATH_MATRIX_H

#include "Vector.hpp"

namespace stardazed {
namespace math {


class Matrix {
public:
	static Matrix identity();
	static Matrix scale(float sx, float sy, float sz);
	static Matrix translate(float dx, float dy, float dz);
	static Matrix rotateX(float phi);
	static Matrix rotateY(float phi);
	static Matrix rotateZ(float phi);
	static Matrix rotate(float rx, float ry, float rz);
	
	const float* data() const { return &values[0]; }

private:
	Matrix();
	float values[16];
};


} // ns math
} // ns stardazed

#endif
